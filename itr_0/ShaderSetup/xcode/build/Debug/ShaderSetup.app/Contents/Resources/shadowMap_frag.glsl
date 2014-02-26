uniform sampler2DShadow uLightDepthTexture0;
uniform sampler2DShadow	uLightDepthTexture1;

varying vec3 vN; 	// vertex normal	
varying vec3 vV; 	// vertex position in eye space, seen from 0,0,0
varying vec4 vVEye;	// vertex position in eye space

varying vec3 vL0;	// direction of light 0 vertex in eye space
varying vec3 vL1;	// direction of light 1 vertex in eye space;

varying vec4 vSc0;	// coord of shadow 0 in eye space
varying vec4 vSc1;	// coord of shadow 1 in eye space

uniform float uTexelSize; // size of texel size

#define SAMPLES_COUNT 32
#define INV_SAMPLES_COUNT (1.0 / SAMPLES_COUNT)

/*----------------------------------------------------------------------------------------------------*/


vec4 materialEmissive= vec4(0,0,0,0);
vec4 materialAmbient = vec4(0.25,0,0,1);
vec4 materialDiffuse = vec4(1,0,1,1);
vec4 materialSpecular= vec4(1,1,1,1);
float materialShininess = 2.0; 


/*----------------------------------------------------------------------------------------------------*/

float shadowLookup(in sampler2DShadow shadowData, in vec4 coord, float x, float y){
	return shadow2DProj(shadowData, coord + vec4(x,y,0.0,1.0) * uTexelSize).r;
}

float getShadow(in sampler2DShadow shadowData, in vec4 coord){
	return shadowLookup(shadowData, coord, 0.0, 0.0);

}

vec4 getShadowFactor(in float shadow, 
				 	 in float shadowIntensity, 
				 	 in vec4 ambient, in vec4 diffuse, in vec4 specular){
	return (ambient + (1.0 - shadowIntensity + shadow * shadowIntensity) * diffuse) + specular * shadow;
}

/*----------------------------------------------------------------------------------------------------*/

//Remove shadow acne

void main(void){
	vec3 R = -normalize( reflect( vL0, vN ) );
	
	// calc material components light0
	vec4 ambient0  = gl_LightSource[0].ambient  * materialAmbient;
	vec4 diffuse0  = gl_LightSource[0].diffuse  * materialDiffuse  * max(dot( vN, vL0), 0.0);
	vec4 specular0 = gl_LightSource[0].specular * materialSpecular * pow(max(dot(R, vV), 0.0), materialShininess);
	
	R = -normalize( reflect( vL1, vN ) );

	vec4 ambient1  = gl_LightSource[1].ambient * materialAmbient;
	vec4 diffuse1  = gl_LightSource[1].diffuse * materialDiffuse * max(dot( vN, vL1), 0.0);
	vec4 specular1 = gl_LightSource[1].specular * materialSpecular * pow(max(dot(R, vV), 0.0), materialShininess);
	

	float shadowIntensity = 0.25;

	vec4  result = vec4(0,0,0,0);
	result += getShadowFactor(getShadow(uLightDepthTexture0, vSc0), shadowIntensity, ambient0, diffuse0, specular0);
	result += getShadowFactor(getShadow(uLightDepthTexture1, vSc1), shadowIntensity, ambient1, diffuse1, specular1);

	gl_FragColor = result;
}
