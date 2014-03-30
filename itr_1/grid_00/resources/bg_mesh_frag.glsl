varying vec3 vNormal;
varying vec3 vVertex;

vec3 lightPos      = vec3(-3,20,3);

vec4 lightAmbient  = vec4(0.35,0.45,0.45,1);
vec4 lightDiffuse  = vec4(0.85,0.85,0.85,1);
vec4 lightSpecular = vec4(1,1,1,1);

vec4  matAmbient   = vec4(17.0/255.0, 7.0/255.0,  22.0/255.0, 1);
vec4  matDiffuse   = vec4(52.0/255.0, 22.0/255.0, 66.0/255.0, 1);
vec4  matSpecular  = vec4(1,1,1, 1);
float matShininess = 10.0;

float scalar = 0.2;

void main(){
	vec4 frontLightProduct_ambient  = matAmbient  * lightAmbient;
	vec4 frontLightProduct_diffuse  = matDiffuse  * lightDiffuse;
	vec4 frontLightProduct_specular = matSpecular * lightSpecular;

	vec3 L = normalize(lightPos - vVertex);   
	vec3 E = normalize(-vVertex); 
	vec3 R = normalize(-reflect(L,vNormal));  

	vec4 ambient = frontLightProduct_ambient;
	
	vec4 diffuse = frontLightProduct_diffuse;
	diffuse *= max(dot(vNormal,L),0.0);
	diffuse  = clamp(diffuse,0.0,1.0);

	vec4 specular = frontLightProduct_specular;
	specular *= pow(max(dot(R,E),0.0), matShininess);
	specular = clamp(specular, 0.0, 1.0); 

	vec4 resNormal = vec4(normalize(vNormal) * 0.5 + vec3(0.5),1.0);
	vec4 resLight  = (ambient + diffuse + specular);

	gl_FragColor = resNormal * (1.0 - scalar) + resLight * scalar;
}