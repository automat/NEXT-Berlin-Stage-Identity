uniform sampler2D uPrevPositionMap; //prev position
uniform sampler2D uIndexMap; //overall index of current item
uniform float     uNumItems; //overall size
uniform float     uTime; //seconds elapsed

#define M_PI 3.1415926535897932384626433832795

void main(){
    vec4 dataPrevPosition = texture2D(uPrevPositionMap, gl_TexCoord[0].st);
    vec4 dataIndexMap     = texture2D(uIndexMap,        gl_TexCoord[0].st);
    vec4 position = dataPrevPosition; //+ sin(dataIndexMap.r / (float)uNumItemms * M_PI + uTime);

    float norm   = dataIndexMap.r / uNumItems;
    float angle  = norm * M_PI * 2.0 + uTime;
    float radius = 0.5;
    
    position.y = sin(norm * M_PI * 128.0) * 0.25;
    position.x = (radius - sin(norm * M_PI * 512.0) * cos(norm * M_PI * 16.0) * 0.125 ) * cos(angle);// / uNumItemms;
    position.z = radius*sin(angle);//sin(dataIndexMap.r/uNumItemms * M_PI + uTime);
    gl_FragColor = position;
}