varying float vDepth;

void main(){
    //float linearDepth = length(vPosition) * uLinearDepthConst;
    //gl_FragColor.r = linearDepth;
    //gl_FragColor = vec4(1,0,0,1);
    //gl_FragColor = vec4(vDepth,vDepth,vDepth,1.0);
    gl_FragColor = mix(vec4(0.0),vec4(1.0),vDepth);
}