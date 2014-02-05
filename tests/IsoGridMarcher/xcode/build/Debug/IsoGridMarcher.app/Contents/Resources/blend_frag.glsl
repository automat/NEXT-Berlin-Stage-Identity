uniform sampler2D uTex0;
uniform sampler2D uTex1;

void main(){
    vec2 uv   = gl_TexCoord[0].st;
    vec4 dest = texture2D(uTex0, uv);
    vec4 src  = texture2D(uTex1, uv);
    
    //src = (src * 0.5) - 0.5;
    //gl_FragColor = min(src + dest, 1.0);
    gl_FragColor = (src + dest) - (src * dest);
}