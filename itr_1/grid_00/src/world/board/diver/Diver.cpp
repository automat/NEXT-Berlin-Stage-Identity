#include "world/board/diver/Diver.h"

Diver::Diver(PathSlice* pathSlice,
             int        numPoints,
             float      offset,
             float      speed,
             float      length,
             float      height) :
    mNumPoints(numPoints),
    mPathSlice(pathSlice),
    mOffset(offset),
    mSpeed(speed),
    mWidth(pathSlice->getWidth()),
    mLength(length),
    mHeight(height){
        mPathLength = mPathSlice->getSurfaceSize();
        mPathLengthInv = 1.0f / mPathLength;
        
        mIsOut = mIsOutPrev = false;
        mPoints.resize(mNumPoints);
     
        
        mLengthStep = float(mLength) / float(mNumPoints-1);
        
        //mLengthStep = mLength / float(mNumPoints-1);// * mPathLengthInv; //scale unit to path length
    
    }


/*
void Diver::update(){
    if (mIsOut && mIsHidden) { //should hide and is hidden
        return;
    }
    
    // reset if offset reaches double the distance from start
    if(mOffset >= 2.0f){
        mOffset = -1;
    }
    
    mOffset += mSpeed;
    //cout << mOffset << " / " << mLength * mPathLengthInv << endl;
    //cout << mOffset << endl;

    float offsetInv = 1.0f - mOffset;
    int i = 0;
    float mmm = mLength / float(mPoints.size() - 1);
    float step = mLength / float(mNumPoints - 1);
    cout << mLengthStep << endl;
    float intrpl;
    for(vector<Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); itr++){
        //mPathSlice->getPointOn(offsetInv + mLengthStep * float(i++), &(*itr));
        intrpl = mOffset + mLengthStep * float(i++);
        
        mPathSlice->getPointOn(1.0f - intrpl, &(*itr));
    }
}*/


void Diver::update(){
    if (mIsOut && mIsHidden) {
        return;
    }
    
    if(mOffset >= 2.0f){
        mOffset = -1;
    }
    
    mOffset += mSpeed;
    
    int i = -1;
    for(vector<Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); itr++){
        mPathSlice->getPointOn(1.0 - mOffset + mLengthStep * float(++i), &(*itr));
    }
}

void Diver::updateInOut(){
    mIsOutPrev = mIsOut;
    mIsOut = (mOffset >= (1.0f + mLength)) || (mOffset <= 0.0f);
}

void Diver::debugDraw(){
    if (isOut()){
        return;
    }
    
    float prevPointSize;
    glGetFloatv(GL_CURRENT_POINT_SIZE_APPLE, &prevPointSize);
    
    glPointSize(3);
    glColor3f(1, 1, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
    glDrawArrays(GL_POINTS, 0, mPoints.size());
    
    Vec3f line[4];
    float pathWidth = mWidth * 0.5f;
    float height    = mHeight * 0.5f;
    
    glColor3f(0.75f, 0, 0.15f);
    int i = 0;
    while(i < mNumPoints){
        line[0].set(mPoints[i]);
        line[1].set(line[0]);
        line[0].y += height;
        line[1].y += height;
        line[0].x += pathWidth;
        line[1].x -= pathWidth;
        
        line[2].set(mPoints[i]);
        line[3].set(line[2]);
        line[2].y -= height;
        line[3].y -= height;
        line[2].x += pathWidth;
        line[3].x -= pathWidth;
        
        
        glVertexPointer(3, GL_FLOAT, 0, &line[0].x);
        glColor3f(0.75f,0,0.15f);
        glDrawArrays(GL_LINES, 0, 4);
        glColor3f(1, 1, 1);
        glDrawArrays(GL_POINTS, 0, 4);
        i++;
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glPointSize(prevPointSize);
}