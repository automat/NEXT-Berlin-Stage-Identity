uniform sampler2D uPositionMap;
uniform float     uTime;

varying vec4 vColor;

void main(){
    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 positionData = vec4(texture2D(uPositionMap,gl_TexCoord[0].st));
    
    vColor        = vec4(positionData.xyz,1.0);
    gl_Position   = gl_ModelViewProjectionMatrix * positionData;
}