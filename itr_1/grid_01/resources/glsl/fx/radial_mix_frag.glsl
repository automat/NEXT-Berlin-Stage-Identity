uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform vec2      uScreenSize;

float scaleGradient = uScreenSize.y / 1.0;

void main(){
	vec2 coord    = gl_FragCoord.xy;
	vec2 center   = vec2(uScreenSize.x * 0.5, uScreenSize.y * 0.5);
	vec2 coordRel = vec2(coord - center) / vec2(scaleGradient);
	vec2 coordInv = vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y);

	gl_FragColor = mix(texture2D(uTexture0,coordInv), 
					   texture2D(uTexture1,coordInv),
					   clamp(length(coordRel),0.0,1.0));
}