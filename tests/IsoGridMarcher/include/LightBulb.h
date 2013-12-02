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
#include <OpenGL/OpenGL.h>

class LightBulb {
private:
    bool mIsOn;
    ci::Vec3f* mP0;
    ci::Vec3f* mP1;
    
    
public:
    LightBulb() :
    mIsOn(false){}
    
    LightBulb(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn = false) 
    {
        mIsOn = false;
        mP0 = p0;
        mP1 = p1;
    }
    
    void draw(){
        if(!mIsOn){
            ci::gl::color(1, 0, 0);
            glLineWidth(0.5);
        }else{
            ci::gl::color(1, 1, 1);
            glLineWidth(10);
        }
        ci::gl::drawLine(*mP0, *mP1);
        glLineWidth(1);
    }
    
    void switchOn(){
        mIsOn = true;
    }
    
    void switchOff(){
        mIsOn = false;
    }
    
    void getMid(ci::Vec3f* vec){
        *vec = (*mP0 + *mP1) * 0.5f;
    }
}; 

#endif
