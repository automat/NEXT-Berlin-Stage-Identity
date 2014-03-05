varying vec3 vNormal;

void main(){
    //vNormal = normalize(gl_Normal);
    vNormal = gl_Normal;
    gl_Position = ftransform();
}