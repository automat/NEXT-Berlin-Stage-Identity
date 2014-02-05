uniform sampler2D uTex;

uniform int   uOrientation;
uniform vec2  uTexelSize;
uniform float uBlurScale;
uniform float uBlurStrength;
uniform int   uBlurAmount;

float Gaussian(float x, float deviation){
    return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
}

void main(){
  //  gl_FragColor = texture2D(uTex,gl_TexCoord[0].st);
    
    vec2  uv       = gl_TexCoord[0].st;
    float halfBlur = float(uBlurAmount) * 0.5;
    
    vec4  color    = vec4(0.0);
    vec4  texColor = vec4(0.0);
    
    //Gaussian deviation;
    float deviation = halfBlur * 0.35;
    deviation *= deviation;
    float strength = 1.0 - uBlurStrength;
    
    float offset;
    float gaussian;
    
    float orientation    = float(uOrientation);
    float orientationInv = 1.0 - orientation;
    
    for (int i = 0; i < 20; ++i) {
        if (i >= uBlurAmount) {
            break;
        }
        
        offset   = float(i) - halfBlur;
        gaussian = Gaussian(offset * strength, deviation);
        texColor = texture2D(uTex, uv + vec2(offset * uTexelSize.x * uBlurScale * orientation,
                                             offset * uTexelSize.y * uBlurScale * orientationInv)) * gaussian;
        color   += texColor;
    }

    
    color = clamp(color,0.0,1.0);
    gl_FragColor = vec4(color.r,color.g,color.b,1.0);
    
}