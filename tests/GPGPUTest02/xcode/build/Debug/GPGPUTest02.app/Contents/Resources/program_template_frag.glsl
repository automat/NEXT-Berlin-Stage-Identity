uniform sampler2D uDataPosition; // source data prev position
uniform sampler2D uDataIndex;    // source data index
uniform float     uCount;        // source data count
uniform float     uTime;         // source data time
uniform float     uFrame;        // source data frame
uniform float     uAttIndex;     // source attachment index for pingpong

#define M_PI 3.1415926535897932384626433832795

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
    
    
    
    nextPos.x = prevPos.x + 0.0001;
    nextPos.y = prevPos.y;
    nextPos.z = prevPos.z;
    //nextPos   = prevPos;
    //nextPos.x = 0.5 * cos(index/count * M_PI * 2.);
    //nextPos.y = 0.0;
    //nextPos.z = 0.5 * sin(index/count * M_PI * 2.);
    //nextColor = prevColor;
}


/*-----------------------------------------------------------------------------------------*/
// PROGRAM - END
/*-----------------------------------------------------------------------------------------*/

void main(){
    step(texture2D(uDataIndex,gl_TexCoord[0].st).r, // index
         uCount, //count
         uTime, // time
         uFrame, // frame
         texture2D(uDataPosition,gl_TexCoord[0].st), // prev position
         gl_FragColor); // send result
}