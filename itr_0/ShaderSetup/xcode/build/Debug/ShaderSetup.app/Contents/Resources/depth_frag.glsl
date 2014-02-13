uniform float uNear;
uniform float uFar;

varying vec4 vPosition;

vec4 pack(in float depth){
	const vec4 bias = vec4(1.0 / 255.0,1.0 / 255.0,1.0 / 255.0,0.0);

	float r = depth;
	float g = fract(r * 255.0);
	float b = fract(g * 255.0);
	float a = fract(b * 255.0);
	vec4 colour = vec4(r, g, b, a);
	
	return colour - (colour.yzww * bias);
}


void main(){
    //float linearDepth = length(vPosition) * uLinearDepthConst;
    //gl_FragColor.r = linearDepth;
    //gl_FragColor = vec4(1,0,0,1);
    //gl_FragColor = vec4(vDepth,vDepth,vDepth,1.0);
    //gl_FragColor = mix(vec4(0.0),vec4(1.0),vDepth);
    float linearDepth = length(vPosition) * (1.0 / (uFar - uNear));
    gl_FragColor = pack(linearDepth);

}