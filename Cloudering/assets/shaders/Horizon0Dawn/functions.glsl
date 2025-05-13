

float Remap(float orig_val, float orig_min, float orig_max, float new_min, float new_max)
{
    return new_min + (((orig_val - orig_min) / (orig_max - orig_min)) * (new_max - new_min));
}

vec3 Remap(vec3 orig_val, float orig_min, float orig_max, float new_min, float new_max)
{
    float x = Remap(orig_val.x, orig_min, orig_max, new_min, new_max);
    float y = Remap(orig_val.y, orig_min, orig_max, new_min, new_max);
    float z = Remap(orig_val.z, orig_min, orig_max, new_min, new_max);

    return vec3(x, y, z);
}

bool RaySphereIntersect(vec3 ro, vec3 rd, float sphereR, out float t0, out float t1) {
    float h_s = dot(rd, -ro);
    vec3 P_s = ro+rd*h_s;
    float v = length(P_s);
    if (sphereR < v) return false;
    float d = sqrt(sphereR*sphereR - v*v);
    t0 = (h_s - d);
    t1 = (h_s + d);
    return true;
}

vec3 ComputeIncidentLight(vec3 ro, vec3 rd, float tmin, float tmax){
    float t0, t1;
    if (!RaySphereIntersect(ro, rd, ATMOS_R, t0, t1) || t1 < 0.0) return vec3(0.0);
    if (t0 > tmin) tmin = t0;
    if (t1 < tmax) tmax = t1;
    float segmentLength = (tmax - tmin) / float(NUM_SAMPLES);

    vec3 sumR = vec3(0.0);
    vec3 sumM = vec3(0.0);

    float opticalDepthR = 0., opticalDepthM = 0.;
    float mu = dot(rd, SunDir);
    float phaseR = 3. / (16. * PI) * (1. + mu * mu); 
    float phaseM = 3. / (8.  * PI) * ((1. - G * G) * (1. + mu * mu)) / ((2. + G * G) * pow(1. + G * G - 2. * G * mu, 1.5));

    for (float i = 0; i < float(NUM_SAMPLES); i++){
        vec3 rayPos = ro + rd*(i*segmentLength + segmentLength*0.5);
        float height = length(rayPos) - EARTH_R;

        float hr = exp(-height / HEIGHT_R) * segmentLength; 
        float hm = exp(-height / HEIGHT_M) * segmentLength; 

        opticalDepthR += hr; 
        opticalDepthM += hm;

        float t0Light, t1Light; 
        RaySphereIntersect(rayPos, SunDir, ATMOS_R, t0Light, t1Light);

        float segmentLengthLight = t1Light / float(NUM_SAMPLES_LIGHT); 
        float opticalDepthLightR = 0, opticalDepthLightM = 0; 

        uint j = 0;
        for (; j < NUM_SAMPLES_LIGHT; j++){
            vec3 lightRayPos = rayPos + SunDir * (segmentLengthLight * (float(j) + 0.5));
            float heightLight = length(lightRayPos) - EARTH_R; 
            if (heightLight < 0) break; 
            opticalDepthLightR += exp(-heightLight / HEIGHT_R) * segmentLengthLight; 
            opticalDepthLightM += exp(-heightLight / HEIGHT_M) * segmentLengthLight; 
        }
        if (j == NUM_SAMPLES_LIGHT) { 
            vec3 tau = BETA_R * (opticalDepthR + opticalDepthLightR) + BETA_M * 1.1f * (opticalDepthM + opticalDepthLightM); 
            vec3 attenuation = exp(-tau);
            sumR += attenuation * hr; 
            sumM += attenuation * hm; 
        } 
    }
    return (sumR * BETA_R * phaseR + sumM * BETA_M * phaseM) * 20.0;
}

vec2 cubeMapUV(vec3 pos) {
    vec3 p = normalize(pos);
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

vec3 ComputeIncidentLight(vec3 ro, vec3 rd) {
    return ComputeIncidentLight(ro, rd, 0.0, 1./0.);
}

float HenyeyGreenstein(vec3 inLightVector, vec3 inViewVector, float inG) {
    float cos_a = dot(normalize(inLightVector), normalize(inViewVector));
    return ((1.0 - inG * inG) / pow((1.0 + inG * inG - 2.0 * inG * cos_a), 3. / 2.)) / 4. * PI;
}
