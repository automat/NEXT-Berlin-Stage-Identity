varying vec3 vN; 	// vertex normal
varying vec3 vV; 	// vertex position in eye space, seen from 0,0,0
varying vec4 vVEye;	// vertex position in eye space

varying vec3 vL0;	// direction of light 0 vertex in eye space
varying vec3 vL1;	// direction of light 1 vertex in eye space;

/*----------------------------------------------------------------------------------------------------*/


vec4 materialEmissive= vec4(0,0,0,0);
vec4 materialAmbient = vec4(0.25,0,0,1);
vec4 materialDiffuse = vec4(1,0,1,1);
vec4 materialSpecular= vec4(1,1,1,1);
float materialShininess = 100.0;


/*----------------------------------------------------------------------------------------------------*/

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
    
    vec4 result = vec4(0,0,0,0);
         result += ambient0 + diffuse0 + specular0;
         result += ambient1 + diffuse1 + specular1;
	gl_FragColor = result;
}
