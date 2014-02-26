
varying vec3 vLightIntensity;

void main(){
    gl_FragColor = vec4(vLightIntensity,1.0);
}