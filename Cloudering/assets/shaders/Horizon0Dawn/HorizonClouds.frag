#version 430 core

#include "define.glsl"
#include "functions.glsl"
#include "cloudSampler.glsl"

in vec3 fragPos;
out vec4 FragColor;


vec4 Cloud_RayMarch(vec3 ro, vec3 rd){
    //top of the cloudSphereStrip intersection.
    float t0, t1;
    if(!RaySphereIntersect(ro, rd, EARTH_R + CLOUD_MIN_MAX.y, t0, t1) || t1 < .0) {
        //if the upper cloud border is behind you or you are not looking towards it at all (in case you are above clouds only), skip the whole raymarcher
        return vec4(.0);
    }
    t0 = max(0., t0); //in case we are inside of the cloudUpperSphere, we dont need to start from behind us
    
    //same for the bottom of the CloudSphereStrip
    float b0, b1;
    if(!RaySphereIntersect(ro, rd, EARTH_R + CLOUD_MIN_MAX.x, b0, b1) || b1 < .0) { //if we miss the smaller sphere, we are using the upper bounderies for raymarch
        b1 = t0;
    }
    else {
        b1 = (b0 > 0.) ? b0 : b1; // use the closer intersection, (if it is positive)
        t1 = (t0 > 0.) ? t0 : t1;
    }

    t1 = min(t1, 80000.);
    b1 = min(b1, 80000.);
    float step_size = abs(t1 - b1) / float(CLOUD_VOLUME_STEPS + 1);
    if (step_size < 1.) return vec4(.0);

    vec3 p = ro + rd * b1;
    vec3 step = rd * step_size;
    p += step*0.5;

    float extinction = 1.;
    vec3 scattering = vec3(0.);

    float cloud_test = .0;
    int zero_density_sample_count = 0;

    for(int i = 0; i < CLOUD_VOLUME_STEPS; i++){
        
        vec3 weather_data = texture2D(weatherDataTex, fract(cubeMapUV(p)*100.)).rgb;
        if (cloud_test > ZERO){
            float sampled_density = SampleCloudDensity(p, weather_data, false);
            float fade = 1.0 - smoothstep(10000.0, 80000.0, distance(ro, p));
            sampled_density *= fade;
            if (sampled_density <= ZERO){
                zero_density_sample_count += 1;
            }
            if (zero_density_sample_count < 6){
                if (sampled_density > .0){

                    extinction *= exp(-EXTINCTION_F * sampled_density * step_size);

                    float light_extinction = SampleCloudDensityAlongRay(p, SunDir * RAYMARCH_TO_SUN_SIZE);
                    

                    vec3 step_scattering = SCATTERING_F * sampled_density * step_size * light_extinction * SunCol;
                    scattering += extinction * step_scattering;

                    if(extinction < 0.001) break;

                }
                p += step;
            } else {
                cloud_test = .0;
                zero_density_sample_count = 0;
            }
        } 
        else {
            cloud_test = SampleCloudDensity(p, weather_data, true);
            if(cloud_test <= ZERO){
                p += step * 2.;
                i++;
            } else {
            i--;
            }
        }
    }
    return clamp(vec4(scattering, extinction), 0., 1.);
}

void main() {
    vec3 rd = normalize(fragPos);
    vec3 ro = view[3].xyz + vec3(.0, EARTH_R + 1., .0);

    //sky model by Nishita 
    vec3 skyColor = ComputeIncidentLight(ro, rd);

    //guerrila games inspired clouds
    vec4 clouds = Cloud_RayMarch(ro, rd);

    vec3 skybox = mix(clouds.rgb, skyColor, clouds.a);

    FragColor = vec4(skybox, 1.);
}