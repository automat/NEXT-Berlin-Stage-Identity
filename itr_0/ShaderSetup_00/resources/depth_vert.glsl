varying vec4 vPosition;

void main( void ){
    vPosition = gl_ModelViewMatrix * gl_Vertex;
    gl_Position = ftransform();
}