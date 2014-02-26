uniform float uLinearDepthConstant;
varying vec4 vPosition;

void main() { 
    float linearDepth = length(vPosition) * 0.025;
    gl_FragColor.r = linearDepth;
}
