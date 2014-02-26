varying vec4 vVertex;
varying vec3 vNormal;
varying vec4 vColor;
varying vec2 vTexcoord;



varying vec3 vLightIntensity;
varying vec2 vTexCoord2d;

/*------------------------------------------------------------------------*/

// for live edit
uniform float uTime;

vec4 materialEmissive= vec4(0,0,0,0);
vec4 materialAmbient = vec4(0.75,0.75,0.75,1);
vec4 materialDiffuse = vec4(0.75,0.75,0.75,1);
vec4 materialSpecular= vec4(1,1,1,1);
float materialShininess = 1.0; 

vec4  light0Ambient  = vec4(1,1,1,1);
vec4  light0Diffuse  = vec4(1,1,1,1);
vec4  light0Specular = vec4(1,1,1,1);
float light0ConstAtt = 0.125;
float light0LinAtt = 5.0;
float light0QuadAtt = 0.01;




void toEyeSpace(out vec3 normal, out vec4 position){
	normal   = normalize(gl_NormalMatrix * gl_Normal);
	position = gl_ModelViewMatrix * gl_Vertex;
}

vec3 modelPhong(vec4 position, vec3 normal){
	gl_LightSourceParameters light0 = gl_LightSource[0];

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



void main(){
	//light0QuadAtt = (0.5 + sin(uTime*1000.0) * 0.5) * 5.0;


	vec3 normalEye;
	vec4 positionEye;
	toEyeSpace(normalEye,positionEye);


	vLightIntensity = modelPhong(positionEye,normalEye);

    gl_Position    = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vTexCoord2d    = gl_TexCoord[0].st;

    
}