








float GetHeightFractionForPoint(vec3 p, vec2 inCloudMinMax){
    float h = (length(p) - EARTH_R - inCloudMinMax.x) / inCloudMinMax.y;

    return clamp(h, 0., 1.);
}


float GetHightDensity(vec3 p, float type){
    float h = GetHeightFractionForPoint(p, CLOUD_MIN_MAX);
    
    float minMax = mix(0.1, 0.3, clamp(type*2., 0., 1.));
    minMax = mix(minMax, 0.1, clamp(type*2.-1., 0., 1.));
    
    float upMax = 0.1 + 0.56*type;
    float top = 0.12 + 0.88*type;
    
    return smoothstep(0., minMax, h) - smoothstep(upMax, top, h);
}


float SampleCloudDensity(vec3 p, vec3 weather_data, bool cheapSample) {
    vec4 lowFreqTex = texture(lowFreqNoises, fract(p*0.0001)).rgba;

    float lowFreqFBM = (lowFreqTex.g * 0.625) + (lowFreqTex.b * 0.25) + (lowFreqTex.a * 0.125);

    float base_cloud = Remap( lowFreqTex.r, -(1. - lowFreqFBM), 1. , 0., 1. );
    
    float coverage = weather_data.r;
    float cloud_density = weather_data.g;
    float cloud_type = weather_data.b;

    float densityHeightGradient = GetHightDensity(p, cloud_type);
    base_cloud = Remap(base_cloud, coverage, 1., 0., 1.) * coverage;
    
    base_cloud *= densityHeightGradient;
    if (cheapSample) {
         return base_cloud * cloud_density;
    }

    vec3 highFreqTex = texture(highFreqNoises, fract(p*.0002)).rgb;

    float highFreqFBM = (highFreqTex.r * 0.625) + (highFreqTex.g * 0.25) + (highFreqTex.b * 0.125);

    float height_fraction = GetHeightFractionForPoint(p, CLOUD_MIN_MAX);

    float high_freq_noise_modifier = mix(highFreqFBM, 1.-highFreqFBM, clamp(height_fraction * 10., 0., 1.));

    float final_cloud = Remap(base_cloud, high_freq_noise_modifier*.3, 1., 0., 1.);

    return final_cloud * cloud_density;

    //todo: wind_offset and curl_noise;
    
}


float SampleCloudDensityAlongRay(vec3 p, vec3 light_step){
    float extinction = 1.0;

    for (int i = 0; i < SUN_IN_SCATTERING_STEPS; i++){
        p += light_step;
        vec3 weather_data = texture2D(weatherDataTex, fract(cubeMapUV(p)*100.)).rgb;
        float cloud_density = SampleCloudDensity(p, weather_data, true);
        extinction *= exp(-EXTINCTION_F * cloud_density * length(light_step)); //beer-lambert law
    }
    return extinction * length(light_step); 
}