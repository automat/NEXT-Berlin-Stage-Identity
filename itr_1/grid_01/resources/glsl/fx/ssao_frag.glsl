//http://www.gamerendering.com/2009/01/14/ssao/

uniform sampler2D uTextureRandom;
uniform sampler2D uTextureNormalMap;

varying vec2 uv;

const float totStrength = 0.0138;
const float strength = 0.13;
const float offset = 0.0012;
const float falloff = 0.0;
const float rad = 0.00125;

#define SAMPLES 10 // 10 is good
const float invSamples = -0.45/10.0;


float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

int randInt(int start, int end){
    return int(fract(sin(dot(vec2(start, end),vec2(12.9898,78.233))) * 43758.5453));
}

void main(void){
    vec3 pSphere[10];
    pSphere[0] = vec3(0.13790712, 0.24864247, 0.44301823);
    pSphere[1] = vec3(0.33715037, 0.56794053, -0.005789503);
    pSphere[2] = vec3(0.06896307, -0.15983082, -0.85477847);
    pSphere[3] = vec3(-0.014653638, 0.14027752, 0.0762037);
    pSphere[4] = vec3(0.010019933, -0.1924225, -0.034443386);
    pSphere[5] = vec3(-0.35775623, -0.5301969, -0.43581226);
    pSphere[6] = vec3(-0.3169221, 0.106360726, 0.015860917);
    pSphere[7] = vec3(0.010350345, -0.58698344, 0.0046293875);
    pSphere[8] = vec3(-0.053382345, 0.059675813, -0.5411899);
    pSphere[9] = vec3(0.035267662, -0.063188605, 0.54602677);

   // grab a normal for reflecting the sample rays later on
   vec3 fres = normalize((texture2D(uTextureRandom,uv*offset).xyz*2.0) - vec3(1.0));
 
   vec4 currentPixelSample = texture2D(uTextureNormalMap,uv);
 
   float currentPixelDepth = currentPixelSample.a;
 
   // current fragment coords in screen space
   vec3 ep = vec3(uv.xy,currentPixelDepth);
  // get the normal of current fragment
   vec3 norm = currentPixelSample.xyz;
 
   float bl = 0.0;
   // adjust for the depth ( not shure if this is good..)
   float radD = rad/currentPixelDepth;
 
   //vec3 ray, se, occNorm;
   float occluderDepth, depthDifference;
   vec4 occluderFragment;
   vec3 ray;
   for(int i=0; i < SAMPLES;++i)
   {
      // get a vector (randomized inside of a sphere with radius 1.0) from a texture and reflect it
      ray = radD*reflect(pSphere[i],fres);
 
      // get the depth of the occluder fragment
      occluderFragment = texture2D(uTextureNormalMap,ep.xy + sign(dot(ray,norm) )*ray.xy);
    // if depthDifference is negative = occluder is behind current fragment
      depthDifference = currentPixelDepth-occluderFragment.a;
 
      // calculate the difference between the normals as a weight
 // the falloff equation, starts at falloff and is kind of 1/x^2 falling
      bl += step(falloff,depthDifference)*(1.0-dot(occluderFragment.xyz,norm))*(1.0-smoothstep(falloff,strength,depthDifference));
   }
 
   // output the result
   gl_FragColor.r = 1.0+bl*invSamples;
}