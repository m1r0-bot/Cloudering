
//for a clear day sky-dome
#define EARTH_R 6360e3
#define ATMOS_R 6420e3
#define HEIGHT_R 7994.0
#define HEIGHT_M 1200.0
#define G 0.76
#define PI 3.14159265
#define BETA_R vec3(3.8e-6, 13.5e-6, 33.1e-6) 
#define BETA_M vec3(21e-6) 
#define DELTA 0.001
#define NUM_SAMPLES 16
#define NUM_SAMPLES_LIGHT 8

//HORIZON CLOUDS
#define CLOUD_MIN_MAX vec2(1500., 8000.)
#define CLOUD_DENSITY .01
#define EXTINCTION_F .1
#define SCATTERING_F .0001
#define ZERO .0000001


//raymarch constants
#define CLOUD_VOLUME_STEPS 100
#define SUN_IN_SCATTERING_STEPS 6
#define RAYMARCH_TO_SUN_SIZE 1000.





//UNIFORMS
uniform mat4 view;

uniform vec3 SunDir;
uniform vec3 SunCol;

//textures
uniform sampler3D lowFreqNoises;
uniform sampler3D highFreqNoises;
uniform sampler2D weatherDataTex;
