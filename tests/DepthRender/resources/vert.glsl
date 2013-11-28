varying float vDepth;
uniform float uFarClip;

void main(){
    gl_Position = ftransform();
    vDepth = 1.0 - gl_Position.z / uFarClip;
}