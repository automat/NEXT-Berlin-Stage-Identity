/*-----------------------------------------------------------------------------------------*/
// RESSOURCES
/*-----------------------------------------------------------------------------------------*/

#define M_PI 3.1415926535897932384626433832795

uniform vec4      uDataApplication; // source data application
uniform vec4      uDataMouse;       // source data mouse
uniform vec3      uDataArray;       // source data particle array

uniform sampler2D uDataPosition;    // source data prev position
uniform sampler2D uDataIndex;       // source data index
uniform float     uCount;           // source data count

varying vec2 vTexCoord2d;

vec4 getParticleAt(in int indexX, in int indexY){
    return texture2D(uDataPosition,vec2(float(indexX),float(indexY)));
}

/*-----------------------------------------------------------------------------------------*/
// PROGRAM - BEGIN
/*-----------------------------------------------------------------------------------------*/

void step(in float index,      // index overall of current particle
          in float indexX,     // index X in 2d array of current particle
          in float indexY,     // index Y in 2d array of current particle
          in float count,      // overall particle count
          in vec2  windowSize, // window size x & y
          in float time,       // time elapsed since start
          in float frame,      // frames elapsed since start
          in vec2  mouse,      // mouse x & y
          in float mouseDown,  // mouse down 0.0 / 1.0
          in float mouseWheel, // mouse wheel
          in vec4  prevPos,    // previous particle position
          out vec4 nextPos){   // next particle position

    float is = mod(index,10.0);

    float t = index / count;
    float t1 = t * 1000.0;
    float r = mouseWheel * 10.0;


    nextPos.x = sin(t1 + time) * t * 0.5 * r * 1.;
    nextPos.y = t * (sin(t * 10. - time) + sin(t1 * 10. + time )) * 0.25 * is;
    nextPos.z = cos(t1 + time) * t * 0.5 * r;


}

/*-----------------------------------------------------------------------------------------*/
// PROGRAM - END
/*-----------------------------------------------------------------------------------------*/

void main(){
    step(texture2D(uDataIndex,vTexCoord2d).r,  // data indices
         vTexCoord2d.x,                        //
         vTexCoord2d.y,                        //
         uDataArray.z,                         //
         uDataApplication.xy,                  // data application                  
         uDataApplication.z,                   // 
         uDataApplication.w,                   //
         uDataMouse.xy,                        // data mouse
         uDataMouse.z,                         // 
         uDataMouse.w,                         // 
         texture2D(uDataPosition,vTexCoord2d), // data position
         gl_FragColor);                        //
    gl_FragColor.w = 1.0;
}





