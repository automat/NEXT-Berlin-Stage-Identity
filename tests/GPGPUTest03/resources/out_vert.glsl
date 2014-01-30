uniform sampler2D uDataPosition;

varying vec4 vColor;
varying vec2 vTexCoord2d;

void main(){
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vTexCoord2d    = gl_TexCoord[0].st;
    
    
    vec4 positionData = vec4(texture2D(uDataPosition,vTexCoord2d));
    
    vColor        = vec4(positionData.xyz,1.0);
    gl_Position   = gl_ModelViewProjectionMatrix * positionData;
}