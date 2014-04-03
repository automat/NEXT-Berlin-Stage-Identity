uniform sampler2D uTexture;
uniform vec2      uScreenSize;

vec4 getRGBA_255(int r, int g, int b, float a){
    return vec4(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, a);
}

vec4 color0 = vec4(getRGBA_255(17, 7, 22,1.0));
vec4 color1 = vec4(getRGBA_255(0,0,0,1.0));

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}




float scaleRandom   = 0.75;
float scaleGradient = 1250.0;
float intrplGradientColor = 0.15;


void main(){
    // get radial coord
    vec2 coord    = gl_FragCoord.xy;
    vec2 center   = vec2(uScreenSize.x * 0.5, uScreenSize.y * 0.5);
    vec2 coordRel = vec2(coord - center);
    // uniform scaled
    coordRel.x   /= scaleGradient;
    coordRel.y   /= scaleGradient;

    // get noise
    float intrplGrad    = max(0.0, min(length(coordRel) + rand(coord) * scaleRandom, 1.0));
    vec4  colorGradient = mix(color0, color1, intrplGrad);
    vec4  colorTexture  = texture2D(uTexture, vec2(gl_TexCoord[0].x, 1.0 - gl_TexCoord[0].y));
    
    gl_FragColor = mix(colorGradient,colorTexture,intrplGradientColor);
}