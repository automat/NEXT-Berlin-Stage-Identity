uniform sampler2D uDataPosition; //source data position

void main(){
    gl_FragColor = texture2D(uDataPosition, gl_TexCoord[0].st);
}