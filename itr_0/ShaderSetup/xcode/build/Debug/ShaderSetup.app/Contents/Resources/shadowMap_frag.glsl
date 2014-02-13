uniform sampler2DShadow uLightDepthTexture0;
uniform sampler2DShadow	uLightDepthTexture1;

varying vec3 vN; 	// vertex normal	
varying vec3 vV; 	// vertex position in eye space, seen from 0,0,0
varying vec4 vVEye;	// vertex position in eye space

varying vec3 vL0;	// direction of light 0 vertex in eye space
varying vec3 vL1;	// direction of light 1 vertex in eye space;

varying vec4 vSc0;	// coord of shadow 0 in eye space
varying vec4 vSc1;	// coord of shadow 1 in eye space

/*----------------------------------------------------------------------------------------------------*/


vec4 materialEmissive= vec4(0,0,0,0);
vec4 materialAmbient = vec4(0.25,0,0,1);
vec4 materialDiffuse = vec4(1,0,1,1);
vec4 materialSpecular= vec4(1,1,1,1);
float materialShininess = 2.0; 


gl_LightSourceParameters light0;
gl_LightSourceParameters light1;
vec4 light0Ambient;
vec4 light0Diffuse;
vec4 light0Specular;
vec4 light1Ambient;
vec4 light1Diffuse;
vec4 light1Specular;

float light0ConstAtt = 0.125;
float light0LinAtt = 5.0;
float light0QuadAtt = 0.01;


/*----------------------------------------------------------------------------------------------------*/
/*
vec3 modelPhong(vec4 position, vec3 normal){
	//gl_LightSourceParameters light0 = gl_LightSource[0];
	
	vec3 dir = light0.position.xyz - position.xyz;

	vec3 s = normalize(dir);
	vec3 v = normalize(-position.xyz);
	vec3 r = reflect(-s,normal);
	
	float sDotN	= max(dot(s,normal),0.0);
	float mag   = length(dir.xyz); // distance vertex / light
	float att   = 1.0 / (light0ConstAtt + light0LinAtt * mag + light0QuadAtt * mag * mag);

	vec3 ambient  = light0Ambient.rgb * materialAmbient.rgb;
	vec3 diffuse  = light0Diffuse.rgb * materialDiffuse.rgb * sDotN;
	vec3 specular = (sDotN > 0.0) ? (light0Specular.rgb * materialSpecular.rgb * pow(max(dot(r,v),0.0),materialShininess)) : vec3(0.0);

	return ambient * att + diffuse * att + specular * att;
}
*/

float getShadow(in sampler2DShadow shadowData,in vec4 coord){
	return shadow2D(shadowData,
					0.5 * (coord.xyz / coord.w + 1.0)).z;
}

vec4 getShadowFactor(in float shadow, 
				 in float shadowIntensity, 
				 in vec4 ambient, in vec4 diffuse, in vec4 specular){
	return (ambient + (1.0 - shadowIntensity + shadow * shadowIntensity) * diffuse) + specular * shadow;
}

/*----------------------------------------------------------------------------------------------------*/

//Remove shadow acne

void main(void){
	light0 = gl_LightSource[0];
	light1 = gl_LightSource[1];



	vec3 R = -normalize( reflect( vL0, vN ) );
	
	// calc material components light0
	vec4 ambient0  = light0.ambient  * materialAmbient;
	vec4 diffuse0  = light0.diffuse  * materialDiffuse  * max(dot( vN, vL0), 0.0);
	vec4 specular0 = light0.specular * materialSpecular * pow(max(dot(R, vV), 0.0), materialShininess);
	
	R = -normalize( reflect( vL1, vN ) );

	vec4 ambient1  = light1.ambient * materialAmbient;
	vec4 diffuse1  = light1.diffuse * materialDiffuse * max(dot( vN, vL1), 0.0);
	vec4 specular1 = light1.specular * materialSpecular * pow(max(dot(R, vV), 0.0), materialShininess);
	

	//float shadow = getShadow(depthTexture, vSc0);
	float shadowIntensity = 0.275;

	vec4  result = vec4(0,0,0,0);
	result += getShadowFactor(getShadow(uLightDepthTexture0, vSc0), 0.195, ambient0, diffuse0, specular0);
	result += getShadowFactor(getShadow(uLightDepthTexture1, vSc1), 0.195, ambient1, diffuse1, specular1);

//	result = ambient + diffuse + specular;
	gl_FragColor = result;
}
