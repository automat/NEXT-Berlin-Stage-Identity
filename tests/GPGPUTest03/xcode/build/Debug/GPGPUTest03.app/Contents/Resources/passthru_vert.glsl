varying vec2 vTexCoord2d;
void main(){
    gl_Position    = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vTexCoord2d    = gl_TexCoord[0].st;
}