uniform sampler2D uDataPosition;
varying vec4 vColor;

void main(){
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 position = vec4(texture2D(uDataPosition,gl_TexCoord[0].st).rgb,1.0);
    
    vColor        = position;
    gl_Position   = gl_ModelViewProjectionMatrix * position;
}