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
    mSizeOff(0.01f),
    mSizeOn(0.015f),
    ci::TriMesh(){
        this->init();
    }


LightBulb::LightBulb(ci::Vec3f* p0, ci::Vec3f* p1, bool isOn) :
    mOn(isOn),
    mActive(true),
    mSizeOff(0.01f),
    mSizeOn(0.015f),
    mP0(p0),
    mP1(p1),
    ci::TriMesh(){
        this->init();
        this->updateGeometry();
}

void LightBulb::init(){
    this->getVertices().resize(24);
    this->getNormals().resize(24);
    
    float tindices[36] = {
        0,1,2,
        0,2,3,
        4,5,6,
        4,6,7,
        8,9,10,
        8,10,11,
        12,13,14,
        12,14,15,
        16,17,18,
        16,18,19,
        20,21,22,
        20,22,23
    };
    
    std::vector<uint>& indices = this->getIndices();
    
    int i = 0;
    while (i < 36) {
        indices.push_back(tindices[i  ]);
        indices.push_back(tindices[i+1]);
        indices.push_back(tindices[i+2]);
        i+=3;
    }


}

void LightBulb::updateGeometry(){
    static const ci::Vec3f xAxis(1,0,0);
    static const ci::Vec3f yAxis(0,1,0);
    
    mDistance = mP0->distance(*mP1);
    
    ci::Vec3f tangent  = (*mP1 - *mP0).normalized();
    ci::Vec3f binormal = tangent.cross(tangent.dot(yAxis) > 0.998f ? xAxis : yAxis).normalized();
    ci::Vec3f normal   = binormal.cross(tangent).normalized();
    
    mMatrix = ci::Matrix44f::createRotationOnb(tangent, normal, binormal);
    //mMatrix.rotate(ci::Vec3f(0,0,(float)M_PI_2));
    mMatrix.setTranslate(*mP0);
    
    static const float normVertices[72] =
    {
        0.5f, 0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        0.5f,-0.5f,-0.5f,
        
        0.5f, 0.5f,-0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f,-0.5f,
        
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        0.5f,-0.5f, 0.5f,
        
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        
        0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        0.5f,-0.5f,-0.5f,
        
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
        0.5f, 0.5f,-0.5f
    };
    
    std::vector<ci::Vec3f>& vertices         = this->getVertices();
    std::vector<ci::Vec3f>::iterator itrObjV = vertices.begin();
    
    ci::Vec3f temp;
    
    int i = 0;
    int i3;
    while (itrObjV != vertices.end()) {
        i3 = i * 3;
        temp.set((0.5f+normVertices[i3  ]) * mDistance,
                 normVertices[i3+1] * mSizeOff ,
                 normVertices[i3+2] * mSizeOff);
        
        *itrObjV  = mMatrix.transformPoint(temp);
        ++itrObjV;
        ++i;
    }
    
    this->recalculateNormals();
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


