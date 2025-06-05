#version 430 core

in vec3 fragPos;
out vec4 FragColor;

#define EARTH_R 6360e3
#define ATMOS_R 6420e3
#define HEIGHT_R 7994.0
#define HEIGHT_M 1200.0
#define HEIGHT_C 1000.0
#define G 0.76f
#define PI 3.14159265
#define BETA_R vec3(3.8e-6, 13.5e-6, 33.1e-6) 
#define BETA_M vec3(21e-6) 
#define DELTA 0.001
#define MAP_SCALE 750.0


//uniform mat4 view; //to get relative camera position
uniform vec3 sunDir;
uniform uint numSamples;
uniform uint numSamplesLight;

uniform sampler2D perlinNoise;
uniform sampler2D worleyNoise;

uniform float L;
uniform float k1;
uniform float k2;
uniform float m;

uniform float cloudMin;
uniform float cloudMax;

uniform float RAYMARCH_2SUN_COUNT;
uniform float RAYMARCH_2SUN_SIZE;
uniform float EXTINCTION_F;


bool raySphereIntersect(vec3 ro, vec3 rd, float sphereR, out float t0, out float t1) {
    float h_s = dot(rd, -ro);
    vec3 P_s = ro+rd*h_s;
    float v = length(P_s);
    if (sphereR < v) return false;
    float d = sqrt(sphereR*sphereR - v*v);
    t0 = (h_s - d);
    t1 = (h_s + d);
    return true;
}

vec3 computeIncidentLight(vec3 ro, vec3 rd, float tmin, float tmax){
    float t0, t1;
    if (!raySphereIntersect(ro, rd, ATMOS_R, t0, t1) || t1 < 0.0) return vec3(0.0);
    if (t0 > tmin) tmin = t0;
    if (t1 < tmax) tmax = t1;
    float segmentLength = (tmax - tmin) / float(numSamples);

    vec3 sumR = vec3(0.0);
    vec3 sumM = vec3(0.0);

    float opticalDepthR = 0, opticalDepthM = 0;
    float mu = dot(rd, sunDir);
    float phaseR = 3.f / (16. * PI) * (1 + mu * mu); 
    float phaseM = 3.f / (8.f * PI) * ((1. - G * G) * (1. + mu * mu)) / ((2. + G * G) * pow(1. + G * G - 2. * G * mu, 1.5));

    for (float i = 0; i < float(numSamples); i++){
        vec3 rayPos = ro + rd*(i*segmentLength + segmentLength*0.5);
        float height = length(rayPos) - EARTH_R;

        float hr = exp(-height / HEIGHT_R) * segmentLength; 
        float hm = exp(-height / HEIGHT_M) * segmentLength; 

        opticalDepthR += hr; 
        opticalDepthM += hm;

        float t0Light, t1Light; 
        raySphereIntersect(rayPos, sunDir, ATMOS_R, t0Light, t1Light);

        float segmentLengthLight = t1Light / float(numSamplesLight); 
        float opticalDepthLightR = 0, opticalDepthLightM = 0; 

        uint j = 0;
        for (; j < numSamplesLight; j++){
            vec3 lightRayPos = rayPos + sunDir * (segmentLengthLight * (float(j) + 0.5));
            float heightLight = length(lightRayPos) - EARTH_R; 
            if (heightLight < 0) break; 
            opticalDepthLightR += exp(-heightLight / HEIGHT_R) * segmentLengthLight; 
            opticalDepthLightM += exp(-heightLight / HEIGHT_M) * segmentLengthLight; 
        }
        if (j == numSamplesLight) { 
            vec3 tau = BETA_R * (opticalDepthR + opticalDepthLightR) + BETA_M * 1.1f * (opticalDepthM + opticalDepthLightM); 
            vec3 attenuation = exp(-tau);
            sumR += attenuation * hr; 
            sumM += attenuation * hm; 
        } 
    }
    return (sumR * BETA_R * phaseR + sumM * BETA_M * phaseM) * 20.0;
}

vec3 computeIncidentLight(vec3 ro, vec3 rd) {
    return computeIncidentLight(ro, rd, 0.0, 1./0.);
}

vec2 cubeMapUV(vec3 p) {
    vec3 absP = abs(p);
    vec2 uv;

    if (absP.x >= absP.y && absP.x >= absP.z) {
        uv = vec2(p.z, p.y) / absP.x;
        if (p.x < 0.0) uv.x = -uv.x;
    } else if (absP.y >= absP.x && absP.y >= absP.z) {
        uv = vec2(p.x, p.z) / absP.y;
        if (p.y < 0.0) uv.y = -uv.y;
    } else {
        uv = vec2(p.x, p.y) / absP.z;
        if (p.z < 0.0) uv.x = -uv.x;
    }

    return uv * 0.5 + 0.5;
}

float Worley_FBM(vec2 p){
    vec4  worley = texture2D(worleyNoise, p);
    float o = 0.;

    o += worley.x * 0.625;
    o += worley.y * 0.25;
    o += worley.z * 0.125;

    return o;
}

float Perlin_FBM(vec2 p){
    vec4 perlin = texture2D(perlinNoise, p);
    float o = 0.;
    float amp = 0.5;
    
    o += amp * perlin.x; amp *= 0.55;
    o += amp * perlin.y; amp *= 0.55;
    o += amp * perlin.z; amp *= 0.55;
    o += amp * perlin.w; amp *= 0.55;

    return o;
}

float remap(float orig_val, float orig_min, float orig_max, float new_min, float new_max)
{
    return new_min + (((orig_val - orig_min) / (orig_max - orig_min)) * (new_max - new_min));
}

float map(vec2 uv) {
    float w = Worley_FBM(uv*25.);
    float p = Perlin_FBM(uv);

    float f = remap(p, -(1.-w), 1., 0., 1.);
    
    return remap(f, cloudMin, cloudMax, .0, 1.);
}

vec3 approximateNormal(vec2 p) {
    float v = map(p);
    float vR = map(vec2(p.x + DELTA, p.y));
    float vU = map(vec2(p.x, p.y + DELTA));

    vec3 dx = vec3(DELTA, vR - v, .0);
    vec3 dz = vec3(.0, vU - v, DELTA);

    return normalize(cross(dz, dx));
}

float selfShadowing(vec2 ro){
    
    float ext = 1.;
    for (float f = 0.; f < RAYMARCH_2SUN_COUNT; f++){
        ro += sunDir.xz * RAYMARCH_2SUN_SIZE / MAP_SCALE;
        float cloudDensity = map(ro);
        ext *= exp(-EXTINCTION_F * cloudDensity * RAYMARCH_2SUN_SIZE);
        if(ext < 0.001) break;
    }

    return 1.-clamp(ext, .0, 1.);
}

vec4 computeClouds(vec3 ro, vec3 rd){
    float t0, t1;
    if (!raySphereIntersect(ro, rd, EARTH_R + HEIGHT_C, t0, t1) || t1 < 0.0) return vec4(.0);
    
    vec3 sPos = ro + rd * t1;

    vec2 p = cubeMapUV(normalize(sPos)) * MAP_SCALE;
    
    float noise = map(p);
    //return vec4(noise * min(1., 5000./length(sPos - ro)));
    if (noise == .0) return vec4(.0);
    
    vec3 n = approximateNormal(p);


    float Lt = clamp(dot(n, sunDir)*L, 0., 1.);
    float Ls = clamp(pow(k1 + k2 * dot(rd, sunDir), m) * L, 0., 1.);
    float Sh = selfShadowing(p);

    float F = (Ls + Lt) * Sh;
    F = clamp(F, .0, 1.);
    
    float horizon = smoothstep(40000., 5000., length(sPos - ro));
    return vec4(vec3(F), Sh) * horizon;
}

bool isGround(vec3 ro, vec3 rd) {
    float t0, t1;
    return (raySphereIntersect(ro, rd, EARTH_R, t0, t1) && t1 >= 0.0);
}



void main() {
    vec3 rd = normalize(fragPos);
    vec3 ro = vec3(0, EARTH_R + 100, 0);

    if(isGround(ro, rd)){
        FragColor = vec4(0.2, 0.12, 0.1, 1.0);
        return;
    }



    vec3 clearSky = computeIncidentLight(ro, rd);
    clearSky.x = clearSky.x < 1.413f ? pow(clearSky.x * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-clearSky.x);
    clearSky.y = clearSky.y < 1.413f ? pow(clearSky.y * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-clearSky.y);
    clearSky.z = clearSky.z < 1.413f ? pow(clearSky.z * 0.38317f, 1.0f / 2.2f) : 1.0f - exp(-clearSky.z);

    vec4 clouds = computeClouds(ro, rd);
    

    vec3 col = mix(clearSky, clouds.xyz, clouds.w);
    
    FragColor = vec4(col, 1.0);
    //FragColor = vec4(clouds, 1.);
    //FragColor = texture2D(perlinNoise, fragPos.xz);
}