uniform sampler2DShadow		depthTexture;

varying vec3				N, V, L;
varying vec4				q;

vec4 materialEmissive= vec4(0,0,0,0);
vec4 materialAmbient = vec4(0.25,0,0,1);
vec4 materialDiffuse = vec4(1,0,1,1);
vec4 materialSpecular= vec4(0,1,1,1);
float materialShininess = 2.0; 


gl_LightSourceParameters light0;
vec4 light0Ambient;
vec4 light0Diffuse;
vec4 light0Specular;
float light0ConstAtt = 0.125;
float light0LinAtt = 5.0;
float light0QuadAtt = 0.01;


/*----------------------------------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------------------------------*/

//Remove shadow acne

void main(void){

	light0 = gl_LightSource[0];

	vec4 light0Ambient  = light0.ambient;
	vec4 light0Diffuse  = light0.diffuse;
	vec4 light0Specular = light0.specular;



	vec3 normal = normalize( N );
	vec3 R 		= -normalize( reflect( L, normal ) );
	
	vec4 ambient  = light0.ambient  * materialAmbient;
	vec4 diffuse  = light0.diffuse  * materialDiffuse  * max(dot( normal, L), 0.0);
	vec4 specular = light0.specular * materialSpecular * pow(max(dot(R, V), 0.0), materialShininess);
	

	//shadow
	vec3  shadowTexcoord = 0.5 * (q.xyz / q.w + 1.0);
	float shadow = shadow2D( depthTexture, shadowTexcoord ).z ;
	float shadowIntensity = 0.25;
	gl_FragColor = (ambient + (1.0 - shadowIntensity + shadowIntensity * shadow) * diffuse) + specular * shadow;
}
