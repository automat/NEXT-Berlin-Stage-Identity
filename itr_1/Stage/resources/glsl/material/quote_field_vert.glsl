varying vec3 vNormal;
varying vec3 vVertex;
varying vec3 vLightPos;
varying vec4 vColor;

void main(){
	vVertex   = vec3(gl_ModelViewMatrix * gl_Vertex);
	vLightPos = vec3(gl_ModelViewMatrix * gl_LightSource[1].position);
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vColor  = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}