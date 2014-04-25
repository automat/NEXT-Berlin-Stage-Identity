#include "util/gl/FxResources.h"

gl::GlslProgRef FxResources::sShaderBlurH;
gl::GlslProgRef FxResources::sShaderBlurV;

gl::GlslProgRef FxResources::GetBlurH(){
    if (sShaderBlurH == NULL) {
        sShaderBlurH = gl::GlslProg::create("varying vec2 vTexcoord; void main(){ vec2 Pos = sign(gl_Vertex.xy); vTexcoord = Pos; gl_Position = vec4(Pos, 0.0, 1.0) - 0.5; }",
                                            "uniform sampler2D uTexture; uniform float uTexelSize; uniform float uScale; varying vec2 vTexcoord; void main(){ float offset = uTexelSize * uScale; vec4 sum = vec4(0.0); sum += texture2D(uTexture, vec2(vTexcoord.x - 4.0 * offset, vTexcoord.y)) * 0.05; sum += texture2D(uTexture, vec2(vTexcoord.x - 3.0 * offset, vTexcoord.y)) * 0.09; sum += texture2D(uTexture, vec2(vTexcoord.x - 2.0 * offset, vTexcoord.y)) * 0.12; sum += texture2D(uTexture, vec2(vTexcoord.x - 1.0 * offset, vTexcoord.y)) * 0.15; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y)) * 0.16; sum += texture2D(uTexture, vec2(vTexcoord.x + 1.0 * offset, vTexcoord.y)) * 0.15; sum += texture2D(uTexture, vec2(vTexcoord.x + 2.0 * offset, vTexcoord.y)) * 0.12; sum += texture2D(uTexture, vec2(vTexcoord.x + 3.0 * offset, vTexcoord.y)) * 0.09; sum += texture2D(uTexture, vec2(vTexcoord.x + 4.0 * offset, vTexcoord.y)) * 0.05; gl_FragColor = sum; }");
    }
    return sShaderBlurH;
}

gl::GlslProgRef FxResources::GetBlurV(){
    if(sShaderBlurV == NULL){
        sShaderBlurV = gl::GlslProg::create("varying vec2 vTexcoord; void main(){ vec2 Pos = sign(gl_Vertex.xy); vTexcoord = Pos; gl_Position = vec4(Pos, 0.0, 1.0) - 0.5; }",
                                            "uniform sampler2D uTexture; uniform float uTexelSize; varying vec2 vTexcoord; uniform float uScale; void main(){ float offset = uTexelSize * uScale; vec4 sum = vec4(0.0); sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 4.0 * offset)) * 0.05; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 3.0 * offset)) * 0.09; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 2.0 * offset)) * 0.12; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y - 1.0 * offset)) * 0.15; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y )) * 0.16; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 1.0 * offset)) * 0.15; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 2.0 * offset)) * 0.12; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 3.0 * offset)) * 0.09; sum += texture2D(uTexture, vec2(vTexcoord.x, vTexcoord.y + 4.0 * offset)) * 0.05; gl_FragColor = sum; }");
    }
    return sShaderBlurV;
}

void FxResources::Delete(){
    delete sShaderBlurH.get();
    delete sShaderBlurV.get();
}

