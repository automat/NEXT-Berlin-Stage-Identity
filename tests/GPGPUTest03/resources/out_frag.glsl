uniform sampler2D uDataColor;
varying vec2 vTexCoord2d;

void main(){
    gl_FragColor = texture2D(uDataColor,vTexCoord2d);
}