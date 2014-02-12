varying vec3		N, V, L;
varying vec4		q;

uniform mat4		shadowTransMatrix;

varying vec4 	vLightIntensity;

void main(void)
{
	vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex; // position to eye space

	
	V = normalize( -eyeCoord.xyz );
	L = normalize( gl_LightSource[0].position.xyz - eyeCoord.xyz );
	N = normalize(gl_NormalMatrix * gl_Normal);
    


	q = shadowTransMatrix * eyeCoord;
    gl_Position = ftransform();
}
