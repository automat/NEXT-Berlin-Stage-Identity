uniform sampler2D uPrevPositionMap; //prev position
uniform sampler2D uIndexMap; //overall index of current item
uniform float     uNumItems; //overall size
uniform float     uTime; //seconds elapsed

#define M_PI 3.1415926535897932384626433832795

void main(){
    vec4 dataPrevPosition = texture2D(uPrevPositionMap, gl_TexCoord[0].st);
    vec4 dataIndexMap     = texture2D(uIndexMap,        gl_TexCoord[0].st);
    vec4 position         = dataPrevPosition; //+ sin(dataIndexMap.r / (float)uNumItemms * M_PI + uTime);

    gl_FragColor = position;
}