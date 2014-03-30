uniform float uScreenWidth;
uniform float uScreenHeight;
uniform vec4  uColor0;
uniform vec4  uColor1;



float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float scalar = 2.0;
float randScalar = 0.25;


void main(){
    vec2 coord = gl_FragCoord.xy;
	float xmid = uScreenWidth * 0.5;
	float ymid = uScreenHeight * 0.5;
	float x    = (coord.x - xmid) / uScreenWidth * scalar;
	float y    = (coord.y - ymid) / uScreenWidth * scalar;
	float n    = max(0.0,min(sqrt(x * x + y * y) + rand(coord) * randScalar,1.0));
    
    gl_FragColor = uColor1 * (1.0 - n) + uColor0 * n;
}