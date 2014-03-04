//
//  GridDiver.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridDiver_h
#define grid_01_GridDiver_h

#include "Settings.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "Path.h"


using namespace ci;
using namespace std;

class Diver {
    Path* mPath;
    
    Vec3f  mStart;
    Vec3f  mEnd;
    Vec3f  mPos;
    
    Vec3f  mVel;
    float  mSpeed;
    
    float  mOffset;
    float  mLength;
    float  mLengthStep;
    
    float  mWidth;
    float  mHeight;
    
    vector<Vec3f> mNormals;
    vector<Vec3f> mPoints;
    
    bool   mIsOut;
    bool   mIsOutPrev;
    
    
    
    
    
public:
    Diver(Path* path, float height = 0.125f) :
        mPath(path),
        mOffset(Rand::randFloat(DIVER_MIN_OFFSET,DIVER_MAX_OFFSET)),
        mSpeed(Rand::randFloat(DIVER_MIN_SPEED,DIVER_MAX_SPEED)),
        mLength(Rand::randFloat(DIVER_MIN_LENGTH,DIVER_MAX_LENGTH)),
        mHeight(height){
            mIsOut = mIsOutPrev = false;
            
            mNormals.resize(DIVER_NUM_POINTS);
            mPoints.resize(DIVER_NUM_POINTS);
            mLengthStep = mLength / (float)(DIVER_NUM_POINTS-1);
    }
    
    inline void debugDraw(){
        
        glColor3f(0, 0, 1);
        gl::drawLine(mStart, mEnd);
        
        if (isOut())return;
       
        float prevPointSize;
        glGetFloatv(GL_CURRENT_POINT_SIZE_APPLE, &prevPointSize);
        
        
        glPointSize(3);
        glColor3f(1, 1, 1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
        glDrawArrays(GL_POINTS, 0, mPoints.size());
        
        Vec3f line[4];
        float pathWidth  = mPath->getWidth() * 0.5f;
        float height     = mHeight * 0.5f;
        
        glColor3f(0.75f, 0, 0.15f);
        int i = 0;
        while(i < DIVER_NUM_POINTS){
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
    
    inline const Vec3f& getStart() const{
        return mStart;
    }
    
    inline const Vec3f& getEnd() const{
        return mEnd;
    }
    
    inline const Vec3f& getPos() const{
        return mPos;
    }
    
    //! check if the points summed distance is within visible range
    inline bool isOut(){
        return mIsOut;
    }
    //! check previous out status
    inline bool isOutPrev(){
        return mIsOutPrev;
    }
    
    inline void hide(){
        
    }
    
    
    
    inline void update(){
        // reset if offset reaches double the distance from start
        if(mOffset >= 2.0f){
            mOffset = -1;
        }

        mOffset += mSpeed;
        float offsetInv = 1.0f - mOffset;
        int i = 0;
        for(vector<Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); itr++){
            mPath->getPointOn(offsetInv + mLengthStep * float(i++), &(*itr));
        }
    };
    
    inline void updateInOut(){
        mIsOutPrev = mIsOut;
        mIsOut = (mOffset >= (1.0f + mLength)) || (mOffset <= 0.0f);
    }
    
    inline void draw(){
    }
    
    float getHeight(){
        return mHeight;
    }
    
    const vector<Vec3f>& getPoints(){
        return mPoints;
    }
    
    inline int getNumPoints(){
        return mPoints.size();
    }
};


#endif
