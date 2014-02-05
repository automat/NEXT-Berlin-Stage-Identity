uniform sampler2D uDataPosition;
uniform sampler2D uDataSize;

varying vec2 vTexCoord2d;

void main(){
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vTexCoord2d    = gl_TexCoord[0].st;
    
    gl_Position  = gl_ModelViewProjectionMatrix * texture2D(uDataPosition,vTexCoord2d);
    gl_PointSize = texture2D(uDataSize,vTexCoord2d).r;
}