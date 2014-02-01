attribute vec3 aVertex;
attribute vec3 aNormal;

varying vec3 vNormal;

void main(){
    vNormal =  aNormal;
    
    gl_Position    = ftransform();
    
}