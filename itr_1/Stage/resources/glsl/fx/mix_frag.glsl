uniform sampler2D uTextureSSAO;
uniform sampler2D uTextureBase;

void main()
{
	vec4  ssaoTex = texture2D( uTextureSSAO, gl_TexCoord[0].st );
	vec4  baseTex = texture2D( uTextureBase, gl_TexCoord[0].st );
	float ssaoVal = 1.0 - ssaoTex.r;
	
	vec4 result	= vec4( baseTex.r - ssaoVal,
                        baseTex.g - ssaoVal,
                        baseTex.b - ssaoVal,
                        baseTex.a - ssaoVal );
	
	gl_FragColor = result;
}