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
    ci::Vec3f* mP0;
    ci::Vec3f* mP1;
    
    
public:
    LightBulb() :
    mOn(false),
    mActive(true){}
    
    LightBulb(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn = false) :
    mOn(isOn),
    mActive(true){
        mP0 = p0;
        mP1 = p1;
    }
    
    void drawOcclusive(){
        if (mOn) return;
        ci::gl::color(0, 0, 0);
        glLineWidth(0.5);
        ci::gl::drawLine(*mP0,*mP1);
        glLineWidth(1);
    }
    
    void drawEmissive(){
        if (!mOn)return;
        ci::gl::color(1,1,1);
        glLineWidth(5);
        ci::gl::drawLine(*mP0,*mP1);
        glLineWidth(1);
    }
    
    void switchOn(){
        if(!mActive)return;
        mOn = true;
    }
    
    void switchOff(){
        mOn = false;
    }
    
    void switchRandom(float triggerTolerance = 0.0015f){
        if(ci::randFloat()<triggerTolerance)this->switchOn();
        else this->switchOff();
    }
    
    void getMid(ci::Vec3f* vec){
        *vec = (*mP0 + *mP1) * 0.5f;
    }
    
    bool isOn(){
        return mOn;
    }
    
    bool isActive(){
        return mActive;
    }
    
};

#endif
