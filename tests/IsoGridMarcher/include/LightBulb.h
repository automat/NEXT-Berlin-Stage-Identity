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
    
    float         mDistance;
    ci::Vec3f     mTangent;
    ci::Vec3f     mNormal;
    ci::Vec3f     mBinormal;
    ci::Matrix44f mMatrix;
    
    
public:
    LightBulb() :
    mOn(false),
    mActive(true){}
    
    LightBulb(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn = false) :
    mOn(isOn),
    mActive(true){
        mP0 = p0;
        mP1 = p1;
        
        ci::Vec3f xAxis(1,0,0);
        ci::Vec3f yAxis(0,1,0);
        
        mDistance = p0->distance(*p1);
        mTangent  = (*p1 - *p0).normalized();
        mBinormal = mTangent.cross(mTangent.dot(yAxis) > 0.998f ? xAxis : yAxis).normalized();
        mNormal   = mBinormal.cross(mTangent).normalized();
        
        mMatrix = ci::Matrix44f::createRotationOnb(mTangent, mNormal, mBinormal);
        //mMatrix.rotate(ci::Vec3f(0,0,(float)M_PI_2));
        mMatrix.setTranslate(*p0);
    }
    
    void drawOcclusive(){
        if (mOn) return;
      
        glPushMatrix();
        glMultMatrixf(&(mMatrix)[0]);
        ci::gl::drawLine(*mP0, *mP1);
        //ci::gl::drawCylinder(0.00025f, 0.00025f, -mDistance);
        //float dist = mDistance * 0.75;
        //glTranslatef(0,mDistance*0.25f - dist*0.5f ,0);
        //ci::gl::drawCylinder(0.005f, 0.005f, -(dist));
        //ci::gl::drawCube(ci::Vec3f(mDistance*0.5f,0,0), ci::Vec3f(mDistance,0.00125f,0.00125f));
        glPopMatrix();
    }
   
    void drawEmissive(){
        if (!mOn)return;
        
        glPushMatrix();
        glMultMatrixf(&(mMatrix)[0]);
        //float dist = mDistance * 0.75;
        //ci::gl::drawCylinder(0.003f, 0.003f, -mDistance);
        ci::gl::drawCube(ci::Vec3f(mDistance*0.5f,0,0), ci::Vec3f(mDistance,0.025f,0.025f));
        glPopMatrix();
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
