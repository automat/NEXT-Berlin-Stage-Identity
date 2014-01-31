/*-----------------------------------------------------------------------------------------*/
// RESSOURCES
/*-----------------------------------------------------------------------------------------*/

#define M_PI 3.1415926535897932384626433832795

uniform vec4      uDataApplication; // source data application
uniform vec4      uDataMouse;       // source data mouse
uniform vec3      uDataArray;       // source data particle array

uniform sampler2D uDataPosition;    // source data prev position
uniform sampler2D uDataColor;       // source data prev color
uniform sampler2D uDataSize;        // source data prev size
uniform sampler2D uDataIndex;       // source data index
uniform float     uCount;           // source data count

uniform int     uAttachmentIndex;

varying vec2 vTexCoord2d;

vec4 getParticleAt(in int indexX, in int indexY){
    return texture2D(uDataPosition,vec2(float(indexX),float(indexY)));
}

/*-----------------------------------------------------------------------------------------*/
// PROGRAM - BEGIN
/*-----------------------------------------------------------------------------------------*/

void step(in float  dataIndex, // index overall of current particle
          in float  dataIndexX, // index X in 2d array of current particle (normalized)
          in float  dataIndexY, // index Y in 2d array of current particle (normalized)
          in float  dataWidth,  // data particle width
          in float  dataHeight, // data particle height
          in float  dataCount,  // overall particle count
          in vec2   windowSize, // window size x & y
          in float  time,       // time elapsed since start
          in float  frame,      // frames elapsed since start
          in vec2   mouse,      // mouse x & y
          in float  mouseDown,  // mouse down 0.0 / 1.0
          in float  mouseWheel, // mouse wheel
          in vec4   prevColor,  // previous particle color
          out vec4  nextColor,  // next particle color
          in float  prevSize,
          out float nextSize,
          in vec4   prevPos,    // previous particle position
          out vec4  nextPos){   // next particle position

    nextPos     = prevPos;

    nextColor.r = 0.5 + nextPos.r;
    nextColor.g = 0.0;
    nextColor.b = 0.5 + nextPos.b;
    nextColor.a = 1.0;
    
    nextSize    = 1.0;
}

/*-----------------------------------------------------------------------------------------*/
// PROGRAM - END
/*-----------------------------------------------------------------------------------------*/

void main(){

    step(texture2D(uDataIndex,vTexCoord2d).r,  // data indices
         vTexCoord2d.x,                        //
         vTexCoord2d.y,                        //
         uDataArray.x,                         // data size
         uDataArray.y,                         //
         uDataArray.z,                         //
         uDataApplication.xy,                  // data application                  
         uDataApplication.z,                   // 
         uDataApplication.w,                   //
         uDataMouse.xy,                        // data mouse
         uDataMouse.z,                         // 
         uDataMouse.w,                         // 
         texture2D(uDataColor,vTexCoord2d),    // data prev color
         gl_FragData[1],                       // write next data color
         texture2D(uDataSize, vTexCoord2d).r,    // data prev size
         gl_FragData[2].r,                       // data next size
         texture2D(uDataPosition,vTexCoord2d), // data prev position
         gl_FragData[0]);                      // write next data position
    
    gl_FragData[0].w = 1.0;
    gl_FragData[2].a = 1.0;

}





