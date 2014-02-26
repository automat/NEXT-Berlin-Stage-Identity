uniform sampler2D		u_ShadowMap;
uniform float               u_LinearDepthConstant;

varying vec3    v_Normal;
varying vec4	v_VertInLightSpace;
varying vec3    v_Vertex;
varying vec3    v_LightDir;


/*----------------------------------------------------------------------------------------------------*/




//Remove shadow acne

void main(void){
	
	vec4  ambient   = gl_FrontLightProduct[0].ambient;
	vec4  diffuse   = gl_FrontLightProduct[0].diffuse;
	vec4  specular  = gl_FrontLightProduct[0].specular;
	float shininess = gl_FrontMaterial.shininess;

	vec3 depth = v_VertInLightSpace.xyz / v_VertInLightSpace.w;
	
	float shadow = texture2D(u_ShadowMap, depth.xy).r;

	gl_FragColor = vec4(vec3(shadow),1.0);


	/*
    vec3 lightDir = normalize(v_Vertex.xyz - gl_LightSource[0].position.xyz);
	vec3 normal = normalize(v_Normal);
    
    vec3 R = -normalize( reflect( v_LightDir, normal ) );
    vec3 V = normalize(v_Vertex);

    float lambert = max(dot(normal, v_LightDir), 0.0);
    
    
    vec4 ambient = gl_FrontLightProduct[0].ambient;
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    
   
    if ( lambert > 0.0 ) {
        ambient += gl_FrontLightProduct[0].ambient;
        diffuse += gl_FrontLightProduct[0].diffuse * gl_FrontMaterial.diffuse * lambert;
        specular += gl_FrontLightProduct[0].specular * pow(max(dot(R, V), 0.0), gl_FrontMaterial.shininess);
    }

    // get projected shadow value
    vec3 depth = v_VertInLightSpace.xyz / v_VertInLightSpace.w;
    float lightDepth = length(v_Vertex.xyz - gl_LightSource[0].position.xyz) * u_LinearDepthConstant;

    float shadow = 0.0;
    
    if ( depth.z > 0.0 ) {
        float c = 6.0; // shadow coeffecient - change this to to affect shadow darkness/fade
        float texel = texture2D( u_ShadowMap, depth.xy ).r;
        shadow = clamp( exp( -c * (lightDepth - texel)), 0.0, 1.0 );
    }
    
    vec4 final_color = ambient + diffuse * shadow + specular * shadow;
    
	gl_FragColor = final_color;
	*/
	

}