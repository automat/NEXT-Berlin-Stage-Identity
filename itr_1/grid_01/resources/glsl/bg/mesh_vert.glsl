varying vec3 vNormal;
varying vec3 vVertex;

void main(){
    vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);       
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = ftransform();
}