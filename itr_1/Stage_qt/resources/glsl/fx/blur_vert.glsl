varying vec2 vTexcoord;

void main(){
    vec2 Pos    = sign(gl_Vertex.xy);
    vTexcoord   = Pos;
    
    gl_Position = vec4(Pos, 0.0, 1.0) - 0.5;
}