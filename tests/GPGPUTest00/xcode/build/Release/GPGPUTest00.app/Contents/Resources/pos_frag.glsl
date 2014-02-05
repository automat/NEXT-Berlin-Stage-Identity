uniform sampler2D uPrevPositionMap; //prev position
/*uniform sampler2D uVelocityMap;*/
uniform float     uTime;
void main(){
    vec4 dataPrevPosition = texture2D(uPrevPositionMap, gl_TexCoord[0].st);
    //vec4 dataVelocity = texture2D(uVelocityMap,     gl_TexCoord[0].st);
    gl_FragColor = dataPrevPosition;
}