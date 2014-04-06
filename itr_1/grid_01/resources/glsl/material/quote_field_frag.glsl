uniform sampler2D uTexture;
varying vec3 vNormal;


void main(){
	vec4 texture = texture2D(uTexture,gl_TexCoord[0].xy);
	vec4 result  = texture + vec4(normalize(vNormal) * 0.5 + vec3(0.5),1.0);;
    gl_FragColor = result;
}