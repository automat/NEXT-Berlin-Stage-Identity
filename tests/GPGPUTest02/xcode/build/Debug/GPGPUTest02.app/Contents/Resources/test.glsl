uniform sampler2D uPrevPositionMap; //prev position
uniform sampler2D uIndexMap; //overall index of current item
uniform float     uCount; //overall size
uniform float     uTime; //seconds elapsed
uniform float     uFrame; //frames elapsed

#define M_PI 3.1415926535897932384626433832795

void stepPos(in float time, in float index, in vec4 prevPos, out vec4 nextPos){
	nextPos.x = 0.5 * cos(index / uCount * M_PI * 2. + time);
	nextPos.y = 0.0;
	nextPos.z = 0.5 * sin(index / uCount * M_PI * 2. + time);	
}


void main(){
    vec4 dataPrevPosition = texture2D(uPrevPositionMap, gl_TexCoord[0].st);
    vec4 dataIndexMap     = texture2D(uIndexMap,        gl_TexCoord[0].st);
    float index = dataIndexMap.r;

    vec4 position = vec4(0,0,0,1);
    //stepPos(uTime,dataIndexMap.r,dataPrevPosition,position);
    position.x = 0.5 * cos(index / uCount * M_PI * 2. + uTime);
	position.y = 0.5 * sin(index / uCount * M_PI * 128.0);
	position.z = 0.5 * sin(index / uCount * M_PI * 2. + uTime);

	//stepPos(uTime,dataIndexMap.r,dataPrevPosition,gl_FragColor);
	gl_FragColor = position;
}





