uniform float uTime;
varying vec4 vertexInImageSpace;
float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main()
{
    //just get a random number, using the vertex's screen-space position as a seed along with the time.
    float r = rand(vec2(vertexInImageSpace.x/2.0 + sin(uTime), vertexInImageSpace.y/2.0 + cos(uTime)));
    //play with the noise a bit; clamp it based on its value and scale the alpha.
    if(r < 0.3) r = 0.0;
    vec4 finalColor = vec4(r,r,r,r*0.7);
    gl_FragColor = texture2D(m_Texture, vertexInImageSpace.xy) - clamp(finalColor, 0.0, 1.0);
}