uniform sampler2D uTexture;
uniform float     uScreenWidth;
uniform float     uScreenHeight;
uniform vec4      uColor0;
uniform vec4      uColor1;



float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float scalar = 2.0;
float randScalar = 0.25;

void main(){
    vec2 coord    = gl_FragCoord.xy;
	float xmid    = uScreenWidth * 0.5;
	float ymid    = uScreenHeight * 0.5;
	float x    	  = (coord.x - xmid) / uScreenWidth * scalar;
	float y    	  = (coord.y - ymid) / uScreenWidth * scalar;
	float n0   	  = max(0.0,min(sqrt(x * x + y * y) + rand(coord) * randScalar,1.0));
	float n1      = 0.5;
	vec4 texColor = texture2D(uTexture,vec2(gl_TexCoord[0].x,1.0 - gl_TexCoord[0].y));

	gl_FragColor = (texColor * (1.0 - n1) + uColor1 * n1) * (1.0 - n0) + uColor0 * n0;
}