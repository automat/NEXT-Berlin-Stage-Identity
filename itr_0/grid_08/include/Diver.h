//
//  GridDiver.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridDiver_h
#define grid_01_GridDiver_h

#include "Config.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "Path.h"


using namespace ci;
using namespace std;
using namespace config;

class Diver {
    Path*  mPath;
    Vec3f  mPos;
    
    float  mSpeed;
    float  mOffset;
    float  mLength;
    float  mLengthStep;
    
    float  mWidth;
    float  mHeight;
    
    bool   mIsHidden;   // diver wont loop anymore
    bool   mIsOut;      // diver is outside of visible area
    bool   mIsOutPrev;  // diver previous visibility state

    int           mNumPoints;
    vector<Vec3f> mPoints;
    
    
public:
    Diver(Path* path,
          int   numPoints,
          float offset,
          float speed,
          float width,
          float length,
          float height = 0.125f) :
        mPath(path),
        mOffset(offset),
        mSpeed(speed),
        mWidth(width),
        mLength(length),
        mHeight(height){
            
            float pathLength = path->getLength();
            mNumPoints = numPoints * pathLength;
            
            mIsOut = mIsOutPrev = false;
            mPoints.resize(mNumPoints);
            mLengthStep = mLength / (float)(mNumPoints-1) * 1.0f/path->getLength(); //scale unit to path length
    }
    
    inline const Vec3f& getPos() const{
        return mPos;
    }
    
    inline const vector<Vec3f>& getPoints() const{
        return mPoints;
    }
    
    inline int getNumPoints(){
        return mPoints.size();
    }
    
    //! check if the points summed distance is within visible range
    inline bool isOut(){
        return mIsOut;
    }
    //! check previous out status
    inline bool isOutPrev(){
        return mIsOutPrev;
    }
    
    
    inline void update(){
        if (mIsOut && mIsHidden) { //should hide and is hidden
            return;
        }
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
    }
    
    inline void hide(){
        mIsHidden = true;
    }
    
    inline void show(){
        mIsHidden = false;
    }
    
    inline void updateInOut(){
        mIsOutPrev = mIsOut;
        mIsOut = (mOffset >= (1.0f + mLength)) || (mOffset <= 0.0f);
    }
    
    inline float getWidth(){
        return mWidth;
    }
    
    inline float getHeight(){
        return mHeight;
    }
    
    
    inline void debugDraw(){
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

    
    
  
};


#endif
