varying vec3 vNormal;
varying vec3 vVertex;
varying vec3 vLightPos;

void main(){
	vVertex   = vec3(gl_ModelViewMatrix * gl_Vertex);
	vLightPos = vec3(gl_ModelViewMatrix * gl_LightSource[0].position);
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = ftransform();
}