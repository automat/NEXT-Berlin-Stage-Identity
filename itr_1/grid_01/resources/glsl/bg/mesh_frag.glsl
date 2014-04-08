varying vec3 vNormal;
varying vec3 vVertex;

vec4 toColor_255(int r, int g, int b, float a){
	return vec4(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, a);
}

vec3 lightPos      = vec3(-2,-5,-5);

vec4 lightAmbient  = vec4(0,0,0,1);
vec4 lightDiffuse  = vec4(0.25,0.45,0.5,1);
vec4 lightSpecular = vec4(0.75,0.125,0.5,1);

vec4  matAmbient   = toColor_255(120,7,22,1.0);
vec4  matDiffuse   = toColor_255(125,22,75,1.0);
vec4  matSpecular  = toColor_255(75,22,126,1.0);
float matShininess = 2.0; // 1.0

float scalar = 0.75;
float colorScalar = 3.0;

void main(){
	vec4 frontLightProduct_ambient  = matAmbient  * lightAmbient;
	vec4 frontLightProduct_diffuse  = matDiffuse  * lightDiffuse;
	vec4 frontLightProduct_specular = matSpecular * lightSpecular;

	vec3 L = normalize(lightPos - vVertex);   
	vec3 E = normalize(-vVertex); 
	vec3 R = normalize(-reflect(L,vNormal));  

	vec4 ambient = frontLightProduct_ambient;
	
	vec4 diffuse  = frontLightProduct_diffuse;
         diffuse *= max(dot(vNormal,L),0.0);
         diffuse  = clamp(diffuse,0.0,1.0);

	vec4 specular  = frontLightProduct_specular;
         specular *= pow(max(dot(R,E),0.0), matShininess);
         specular  = clamp(specular, 0.0, 1.0);

    
	vec4 resNormal = vec4(normalize(vNormal) * 0.5 + vec3(0.5),1.0);
		 resNormal.r*=0.5;
		 resNormal.g*=0.5;
	vec4 resLight  = (ambient + diffuse + specular) * colorScalar;

	//gl_FragColor = mix(resNormal,resLight,scalar);//resNormal * (1.0 - scalar) + resLight * scalar;
    gl_FragColor = resLight;
}