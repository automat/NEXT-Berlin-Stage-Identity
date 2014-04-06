varying vec3 vNormal;

void main(){
    gl_FragColor = vec4(normalize(vNormal) * 0.5 + vec3(0.5),1.0);
}