//
//  GridDiver.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridDiver_h
#define grid_01_GridDiver_h

#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "Path.h"


using namespace ci;
using namespace std;

class Diver {
    /*------------------------------------------------------------------------------------*/
     //header lazyness
    const float MIN_SPEED  = 0.005f;
    const float MAX_SPEED  = 0.007f;
    const float MIN_LENGTH = 0.5f;
    const float MAX_LENGTH = 1.0f;
    const int   NUM_POINTS = 10;
    /*------------------------------------------------------------------------------------*/
    
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
    
    vector<Vec3f> mNormals;
    vector<Vec3f> mPoints;
    
    
    
    
    
public:
    Diver(Path* path ) :
        mPath(path),
        mOffset(1.0f),
        mSpeed(Rand::randFloat(MIN_SPEED,MAX_SPEED)),
        mLength(Rand::randFloat(MIN_LENGTH,MAX_LENGTH)),
        mWidth(){
            mNormals.resize(NUM_POINTS);
            mPoints.resize(NUM_POINTS);
            mLengthStep = mLength / (float)(NUM_POINTS-1);
      
            
            
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
        
        Vec3f line[2];
        float pathWidth = mPath->getWidth() * 0.5f;
        
        glColor3f(0.75f, 0, 0.15f);
        int i = 0;
        while(i < NUM_POINTS){
            line[0].set(mPoints[i]);
            line[1].set(line[0]);
            line[0].x += pathWidth;
            line[1].x -= pathWidth;
            
            glVertexPointer(3, GL_FLOAT, 0, &line[0].x);
            glDrawArrays(GL_LINE_STRIP, 0, 2);
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
        return (mOffset >= (1.0f + mLength)) || (mOffset <= 0.0f);
    }
    
    inline void hide(){
        
    }
    
    
    
    inline void update(float t){
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
    
    inline void draw(){
    }
    
    const vector<Vec3f>& getPoints(){
        return mPoints;
    }
    
    inline int getNumPoints(){
        return mPoints.size();
    }
};


#endif
