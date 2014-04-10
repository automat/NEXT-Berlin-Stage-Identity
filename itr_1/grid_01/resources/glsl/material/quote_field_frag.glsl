uniform sampler2D uTexture;
varying vec3 vNormal;
varying vec3 vVertex;
varying vec3 vLightPos;
varying vec4 vColor;


void main(){
	vec3  dir  = vLightPos - vVertex;
	float dist = length(dir.xyz);

	vec4 texture      = texture2D(uTexture,gl_TexCoord[0].xy);
	vec4 textureBlend = vec4(texture.rgb,normalize(length(texture.rgb)));
	

	float att = 1.0 / (gl_LightSource[1].constantAttenuation + 
					   gl_LightSource[1].linearAttenuation * dist + 
					   gl_LightSource[1].quadraticAttenuation * dist * dist);

	vec3 L = normalize(dir);
	vec3 E = normalize(-vVertex);
	vec3 R = normalize(-reflect(L,vNormal));

	float textureBlendScalar = vColor.r;

	vec4 ambient = gl_FrontLightProduct[1].ambient + textureBlend * textureBlendScalar;
	vec4 diffuse = gl_FrontLightProduct[1].diffuse + textureBlend;
		 diffuse*= max(dot(vNormal,L),0.0);
		 diffuse = clamp(diffuse,0.0,1.0);

	vec4 specular = gl_FrontLightProduct[0].specular;
		 specular*= pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);
		 specular = clamp(specular,0.0,1.0);

	//vec4 result  = texture + vec4(0,0,0.125,1);//vec4(normalize(vNormal) * 0.5 + vec3(0.5),1.0);;

	//ambient  += textureBlend;
	//diffuse  += textureBlend;
	//specular += textureBlend;
    
	//gl_FragColor = ambient + diffuse + specular;
   	gl_FragColor = ambient + diffuse + specular;
   	//vec4(gl_Color.rgb,1);
	//gl_FragColor = gl_Color;
   // gl_FragColor = vec4(gl_Color.r,gl_Color.g,gl_Color.b,1.0);
    //gl_FragColor = ambient + diffuse + specular;
}