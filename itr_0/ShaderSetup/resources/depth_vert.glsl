uniform float uNear;
uniform float uFar;

varying float vDepth;


void main( void ){
    vec4 pos = gl_ModelViewMatrix * gl_Vertex;
    //vDepth = clamp((-pos.z - uNear) / (uFar - uNear), 0.0, 1.0);
    vDepth = (-pos.z-uNear) / (uFar- uNear);
    gl_Position = ftransform();
}