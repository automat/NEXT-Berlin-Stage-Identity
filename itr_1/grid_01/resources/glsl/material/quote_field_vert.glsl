varying vec3 vNormal;
varying vec3 vVertex;

void main(){
	vNormal = gl_Normal;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position    = ftransform();
}