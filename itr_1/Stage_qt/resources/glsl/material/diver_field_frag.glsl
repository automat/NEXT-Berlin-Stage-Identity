varying vec3 vNormal;
varying vec3 vVertex;
varying vec3 vLightPos;

void main(){
	vec3  dir  = vLightPos - vVertex;
	float dist = length(dir.xyz);

	float att = 1.0 / (gl_LightSource[0].constantAttenuation + 
					   gl_LightSource[0].linearAttenuation * dist + 
					   gl_LightSource[0].quadraticAttenuation * dist * dist);

	vec3 L = normalize(dir);
	vec3 E = normalize(-vVertex);
	vec3 R = normalize(-reflect(L,vNormal));


	vec4 ambient = gl_FrontLightProduct[0].ambient;
	vec4 diffuse = gl_FrontLightProduct[0].diffuse;
		 diffuse*= max(dot(vNormal,L),0.0);
		 diffuse = clamp(diffuse,0.0,1.0);

	vec4 specular = gl_FrontLightProduct[0].specular;
		 specular*= pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);
		 specular = clamp(specular,0.0,1.0);


	//gl_FragColor = ambient + diffuse + specular;
	gl_FragColor =  ambient * att + diffuse * att + specular * att;
}