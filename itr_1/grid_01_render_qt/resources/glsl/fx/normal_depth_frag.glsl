varying vec3  vNormal;
varying float vDepth;

void main(){
    gl_FragColor = vec4(normalize(vNormal),1.0);
}