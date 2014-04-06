uniform sampler2D uTexture;
uniform float     uTexelSize;
varying vec2      vTexcoord;

const float scale = 2.0;

void main(){
    float offset = uTexelSize * scale;
    
    vec4 sum = vec4(0.0);
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 4.0 * offset)) * 0.05;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 3.0 * offset)) * 0.09;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 2.0 * offset)) * 0.12;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 1.0 * offset)) * 0.15;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y               )) * 0.16;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 1.0 * offset)) * 0.15;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 2.0 * offset)) * 0.12;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 3.0 * offset)) * 0.09;
    sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 4.0 * offset)) * 0.05;
    
    gl_FragColor = sum;
}