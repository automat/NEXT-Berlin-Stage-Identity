uniform sampler2D uPrevVelocityMap;
uniform float uTime;

void main(){
    vec4 prevVelocityData = texture2D(uPrevVelocityMap,gl_TexCoord[0].st);
    gl_FragColor = vec4(1,1,1,1);
}