//
//  GridEdge.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef IsoGridMarcher_GridEdge_h
#define IsoGridMarcher_GridEdge_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Matrix44.h"
#include "cinder/Quaternion.h"
#include "cinder/Rand.h"
#include <OpenGL/OpenGL.h>



class LightBulb {
private:
    bool mOn;
    bool mActive;
    float mSizeOff;
    float mSizeOn;
    
    ci::Vec3f* mP0;
    ci::Vec3f* mP1;
    
    float         mDistance;
    ci::Matrix44f mMatrix;
    
    
   
    
public:
    LightBulb();
    LightBulb(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn = false);
    
    void setSizeOn(float size) { mSizeOn = size; }
    void setSizeOff(float size){ mSizeOff = size; }
    
    void drawOcclusive();
    void drawEmissive();
    
    void switchOn(){if(!mActive)return;mOn = true;}
    
    void switchOff(){mOn = false;}
    
    void switchRandom(float triggerTolerance = 0.0015f);
    void getMid(ci::Vec3f* vec);
    
    bool isOn()    { return mOn; }
    bool isActive(){ return mActive; }
    
};

#endif
