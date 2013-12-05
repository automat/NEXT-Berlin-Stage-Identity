//
//  LightBulb.cpp
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 05/12/13.
//
//

#include "LightBulb.h"

LightBulb::LightBulb() :
    mOn(false),
    mActive(true),
    mSizeOff(0.0001f),
    mSizeOn(0.0015f){}


LightBulb::LightBulb(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn) :
    mOn(isOn),
    mActive(true),
    mSizeOff(0.0001f),
    mSizeOn(0.0015f),
    mP0(p0),
    mP1(p1)
{
    ci::Vec3f xAxis(1,0,0);
    ci::Vec3f yAxis(0,1,0);
    
    mDistance = p0->distance(*p1);
    
    ci::Vec3f tangent  = (*p1 - *p0).normalized();
    ci::Vec3f binormal = tangent.cross(tangent.dot(yAxis) > 0.998f ? xAxis : yAxis).normalized();
    ci::Vec3f normal   = binormal.cross(tangent).normalized();
    
    mMatrix = ci::Matrix44f::createRotationOnb(tangent, normal, binormal);
    //mMatrix.rotate(ci::Vec3f(0,0,(float)M_PI_2));
    mMatrix.setTranslate(*p0);
}

void LightBulb::drawOcclusive(){
    if (mOn) return;
    
    glPushMatrix();
    glMultMatrixf(&(mMatrix)[0]);
    //ci::gl::drawLine(*mP0, *mP1);
    //ci::gl::drawCylinder(0.00025f, 0.00025f, -mDistance);
    //float dist = mDistance * 0.75;
    //glTranslatef(0,mDistance*0.25f - dist*0.5f ,0);
    //ci::gl::drawCylinder(0.005f, 0.005f, -(dist));
    ci::gl::drawCube(ci::Vec3f(mDistance*0.5f,0,0), ci::Vec3f(mDistance,mSizeOff,mSizeOff));
    glPopMatrix();
}

void LightBulb::drawEmissive(){
    if (!mOn)return;
    
    glPushMatrix();
    glMultMatrixf(&(mMatrix)[0]);
    //float dist = mDistance * 0.75;
    //ci::gl::drawCylinder(0.003f, 0.003f, -mDistance);
    ci::gl::drawCube(ci::Vec3f(mDistance*0.5f,0,0), ci::Vec3f(mDistance,mSizeOn,mSizeOn));
    glPopMatrix();
}


void LightBulb::switchRandom(float triggerTolerance){
    if(ci::randFloat()<triggerTolerance)this->switchOn();
    else this->switchOff();
}

void LightBulb::getMid(ci::Vec3f* vec){
    *vec = (*mP0 + *mP1) * 0.5f;
}


