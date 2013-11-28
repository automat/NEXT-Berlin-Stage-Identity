varying float vDepth;
void main(){
    gl_FragColor = vec4(vDepth,vDepth*0.5,vDepth*0.25,1.0);
}