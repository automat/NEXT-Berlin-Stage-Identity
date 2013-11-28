//
//  Skeleton.cpp
//  TriangleGeometry
//
//  Created by Henryk Wollik on 25/11/13.
//
//

#include "Skeleton.h"


Skeleton::Skeleton(int numPoints):
ci::TriMesh(){
    this->init(numPoints);
}

Skeleton::Skeleton(std::vector<ci::Vec3f>* path, int numPoints) :
ci::TriMesh(){
    mPath = path;
    this->init(numPoints);
}

Skeleton::~Skeleton(){}

/*----------------------------------------------------------------------------------*/


void Skeleton::init(int numPoints){
    numPoints   = std::max(2,numPoints);
    mRangeBegin = 0.0f;
    mRangeEnd   = 1.0f;
    mRangeMin   = 0.0f;
    mDirection  = -1.0f;
    mObjScale.x = 1.0f;
    mObjScale.y = 1.0f;
    mObjScale.z = 1.0f;
    
    mPoints.resize(numPoints);
    mRotTangents.resize(numPoints);
    mRotNormals.resize(numPoints);
    mRotBinormals.resize(numPoints);
    mOffsets.resize(numPoints);
    
    this->update();
}


void Skeleton::setPath(std::vector<ci::Vec3f>* path){
    mPath = path;
    this->update();
}


void Skeleton::setPathRange(float rangeMin, float rangeMax){
    mRangeBegin = rangeMin;
    mRangeEnd   = rangeMax;
}


void Skeleton::setOffset(float offsetX, float offsetY, int index){
    if (index == -1) {
        for (std::vector<ci::Vec2f>::iterator itr = mOffsets.begin(); itr != mOffsets.end(); itr++) {
            itr->set(offsetX, offsetY);
        }
        return;
    }
    
    mOffsets[index].set(offsetX, offsetY);
}


void Skeleton::setScale(float x, float y, float z){
    mObjScale.set(x,y,z);
}


void Skeleton::setScale(float x){
    mObjScale.set(x, x, x);
}

/*----------------------------------------------------------------------------------*/

void Skeleton::recalculateRotFrames(){
    int numRotComp = mPoints.size();
    static const ci::Vec3f xAxis = ci::Vec3f::xAxis();
    static const ci::Vec3f yAxis = ci::Vec3f::yAxis();
    int i;
    
    if(mDirection > 0){
        //! update tangents
        i = -1;
        while (++i < numRotComp - 1) {
            mRotTangents[i] = mPoints[i+1] - mPoints[i];
            mRotTangents[i].normalize();
        }
        
        mRotTangents[numRotComp - 1] = mRotTangents[numRotComp - 2];
        
        //! calc initial
        ci::Vec3f& tangentB = mRotTangents[0];
        mRotBinormals[0] = tangentB.cross(tangentB.dot(yAxis) > 0.998f ?
                                          xAxis :
                                          yAxis);
        mRotBinormals[0].normalize();
        mRotNormals[0] = mRotBinormals[0].cross(tangentB);
        
        i = 0;
        while (++i < numRotComp) {
            mRotBinormals[i] = mRotTangents[i].cross(mRotTangents[i].dot(yAxis)>0.998f ?
                                                     xAxis :
                                                     yAxis);
            mRotBinormals[i].normalize();
            mRotNormals[i] = mRotBinormals[i].cross(mRotTangents[i]);
        }
        
        
    }else{
        int numRotComo_1 = numRotComp - 1;
        i = numRotComp;
        while (--i > 0) {
            mRotTangents[i] = mPoints[i] - mPoints[i-1];
            mRotTangents[i].normalize();
        }
        
        mRotTangents[0] = mRotTangents[1];
        
        //! calc initial
        ci::Vec3f& tangentB = mRotTangents[numRotComo_1];
        mRotBinormals[numRotComo_1] = tangentB.cross(tangentB.dot(yAxis) > 0.998f ?
                                                     xAxis :
                                                     yAxis);
        mRotBinormals[numRotComo_1].normalize();
        mRotNormals[numRotComo_1] = mRotBinormals[numRotComo_1].cross(tangentB);
        
        i = numRotComp-1;
        while (--i > -1) {
            mRotBinormals[i] = mRotTangents[i].cross(mRotTangents[i].dot(yAxis)>0.998f?
                                                     xAxis :
                                                     yAxis);
            mRotBinormals[i].normalize();
            mRotNormals[i] = mRotBinormals[i].cross(mRotTangents[i]);
        }

        
    }
}

void Skeleton::getVecOnPath(float intrpl, ci::Vec3f *vec){
    intrpl = std::max(0.0f,std::min(intrpl, 1.0f));

    int len    = mPath->size() - 1;
    int index  = (int)std::floorf(intrpl * len);
    int index1 = (int)std::min(index + 1, len);
    
    float localIntrpl    = fmodf(intrpl, 1.0f / (float)len) * (float)len;
    float localIntrplInv = 1.0f - localIntrpl;
    
    *vec = (*mPath)[index] * localIntrplInv + (*mPath)[index1] * localIntrpl;
}


void Skeleton::update(){
    float range     = std::abs(mRangeEnd - mRangeBegin);
    int   numPoints = mPoints.size();
    float rangeStep = range / (float)(numPoints-1);

    int  i = -1;
    while(++i < numPoints){
        this->getVecOnPath(mRangeBegin + rangeStep * i, &mPoints[i]);
        mPoints[i].x += mOffsets[i].x;
        mPoints[i].y += mOffsets[i].y;
    }

    this->recalculateRotFrames();
    this->update_Internal();
}

void Skeleton::update_Internal(){};


void Skeleton::inverseDirection(){
    mDirection *= -1;
}

/*----------------------------------------------------------------------------------*/

void Skeleton::getRotationMatrix(int index, ci::Matrix44f* matrix){
    *matrix = ci::Matrix44f::createRotationOnb(mRotTangents[index], mRotNormals[index], mRotBinormals[index]);
}


void Skeleton::draw(){
    glColor3f(1.0f,1.0f,1.0f);
    glLineWidth(4);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
    glDrawArrays(GL_LINE_STRIP, 0, mPoints.size());
    glPointSize(10);
    glDrawArrays(GL_POINTS, 0, mPoints.size());
    glPointSize(1);
    glDisableClientState(GL_VERTEX_ARRAY);
    glLineWidth(1);
}


void Skeleton::drawDebug(){
    int numRotComp  = mPoints.size();
    int numRotComp6 = numRotComp * 6;
    
    ci::Vec3f* vertices = new ci::Vec3f[numRotComp6];
    ci::Colorf*colors   = new ci::Colorf[numRotComp6];
   
    int i, i6;
    i = -1;
    while (++i < numRotComp) {
        const ci::Vec3f& point = mPoints[i];
        i6   = i * 6;
        
        colors[i6  ].set(1, 0, 0);
        colors[i6+1].set(colors[i6  ]);
        colors[i6+2].set(0, 1, 0);
        colors[i6+3].set(colors[i6+2]);
        colors[i6+4].set(0, 0, 1);
        colors[i6+5].set(colors[i6+4]);
        
        vertices[i6]   = vertices[i6+2] = vertices[i6+4] = point;
      
        vertices[i6+1] = point + mRotTangents[i] * 0.25f;
        vertices[i6+3] = point + mRotNormals[i] * 0.25f;
        vertices[i6+5] = point + mRotBinormals[i] * 0.25f;
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, &colors[0].r);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
    glDrawArrays(GL_LINES, 0, numRotComp6);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    delete[] vertices;
    delete[] colors;
}


void Skeleton::drawPath(){
    glColor3f(0.5f,0.5f,0.5f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &((*mPath)[0].x));
    glDrawArrays(GL_LINE_STRIP, 0, mPath->size());
    glPointSize(4);
    glDrawArrays(GL_POINTS, 0, mPath->size());
    glPointSize(1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/*----------------------------------------------------------------------------------*/