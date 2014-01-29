uniform sampler2D uPrevPositionMap; //prev position
uniform sampler2D uIndexMap; //overall index of current item
uniform float     uNumItems; //overall size
uniform float     uTime; //seconds elapsed

#define M_PI 3.1415926535897932384626433832795

void main(){
    vec4 dataPrevPosition = texture2D(uPrevPositionMap, gl_TexCoord[0].st);
    vec4 dataIndexMap     = texture2D(uIndexMap,        gl_TexCoord[0].st);
    float index = dataIndexMap.r;

    vec4 position = vec4(1,1,1,1);
    position.x = 0.5 * cos(index / uNumItems * M_PI * 2.0) * mod(index,2.0) * (1.0-mod(index,2.0) * sin(index/uNumItems * M_PI * 128.0) * 0.5);
    position.y = 0.5 * sin(index/uNumItems * M_PI * 32.0 + uTime) * cos(index/uNumItems * M_PI * 8.0 * sin(uTime) * 128.0 + uTime * 16.0);
    position.z = 0.5 * sin(index / uNumItems * M_PI * 2.0) * mod(index,2.0);


    gl_FragColor = position;
}