varying vec4 vColor;
void main(){
    gl_FragColor = vec4(0.5 + vColor.x, 0.5 + vColor.y, 0.5 + vColor.z, vColor.a);
}