varying vec3 vN; 	// vertex normal
varying vec3 vV; 	// vertex position in eye space, seen from 0,0,0
varying vec4 vVEye;	// vertex position in eye space

varying vec3 vL0;	// direction of light 0 vertex in eye space
varying vec3 vL1;	// direction of light 1 vertex in eye space;

/*----------------------------------------------------------------------------------------------------*/

void main(void){
	vec3 R = -normalize( reflect( vL0, vN ) );
	
	// calc material components light0
	vec4 ambient0  = gl_LightSource[0].ambient  * gl_FrontMaterial.ambient;
	vec4 diffuse0  = gl_LightSource[0].diffuse  * gl_FrontMaterial.diffuse  * max(dot( vN, vL0), 0.0);
	vec4 specular0 = gl_LightSource[0].specular * gl_FrontMaterial.specular * pow(max(dot(R, vV), 0.0), gl_FrontMaterial.shininess);
	
	R = -normalize( reflect( vL1, vN ) );
    
	vec4 ambient1  = gl_LightSource[1].ambient * gl_FrontMaterial.ambient;
	vec4 diffuse1  = gl_LightSource[1].diffuse * gl_FrontMaterial.diffuse * max(dot( vN, vL1), 0.0);
	vec4 specular1 = gl_LightSource[1].specular * gl_FrontMaterial.specular * pow(max(dot(R, vV), 0.0), gl_FrontMaterial.shininess);
    
    vec4 result = vec4(0,0,0,0);
         result += ambient0 + diffuse0 + specular0;
         result += ambient1 + diffuse1 + specular1;
	gl_FragColor = result;
}
