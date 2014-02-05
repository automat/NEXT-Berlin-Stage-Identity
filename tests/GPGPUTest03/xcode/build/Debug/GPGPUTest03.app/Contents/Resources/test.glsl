uniform sampler2D uDataPosition; // source data prev position
uniform sampler2D uDataIndex;    // source data index
uniform float     uCount;        // source data count
uniform float     uTime;         // source data time
uniform float     uFrame;        // source data frame

varying vec2 vTexCoord2d;


#define M_PI 3.1415926535897932384626433832795

vec4 getPosModel(in int index){
    return vec4(1,1,1,1);
}

/*-----------------------------------------------------------------------------------------*/
// PROGRAM - BEGIN
/*-----------------------------------------------------------------------------------------*/

void step(in float index,     // particle index
          in float count,     // overall particle count
          in float time,      // time elapsed in seconds
          in float frame,     // frame num
          in vec4  prevPos,   // previous particle position
          out vec4 nextPos    // next particle position
          //in vec4  prevColor, // previous particle color
          //out vec4 nextColor  // color of particle
          ){
    nextPos.x = prevPos.x + 0.001;
    nextPos.y = prevPos.y;
    nextPos.z = prevPos.z;
}

void test(in vec4 prev, out vec4 next){
    next.x = prev.x;
    next.y = prev.y;
    next.z = prev.z;
}


/*-----------------------------------------------------------------------------------------*/
// PROGRAM - END
/*-----------------------------------------------------------------------------------------*/

void main(){
    
    step(texture2D(uDataIndex,vTexCoord2d).r,
         uCount,
         uTime,
         uFrame,
         texture2D(uDataPosition,vTexCoord2d),
         gl_FragColor);
    gl_FragColor.w = 1.0;
}





