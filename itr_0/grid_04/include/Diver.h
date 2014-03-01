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
    Path* mPath;
    
    
    Vec3f  mStart;
    Vec3f  mEnd;
    Vec3f  mPos;
    
    Vec3f  mVel;
    float  mSpeed;
    
    float  mOffset;
    float  mLength;
    float  mWidth;
    
    
    
public:
    Diver(Path* path ) :
    mVel(0,0,-1),
    mLength(0.5f){
    }
    
    inline void debugDraw(){
        
        glColor3f(0, 0, 1);
        gl::drawLine(mStart, mEnd);
        
        if (isOut())return;
        
        glColor3f(1, 1, 1);
        gl::drawVector(mPos, mPos + mVel * 0.0125f,0.05f, 0.0125f);
        
        glColor3f(0.125, 0, 0);
        gl::drawLine(mPos, Vec3f(mPos.x,0,mPos.z));
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
    
    inline bool isOut(){
        return ((mPos.z + mLength) <= (mEnd.z + mLength)) || ((mPos.z - mLength) >= (mEnd.z + mLength));
    }
    
    inline void setVel(float vel){
        mVel.y = vel;
    }
    
    inline void hide(){
        
    }
    
    inline void update(){
        // reset if pos z trigger
        if((mPos.z + mLength) <= (mEnd.z - mLength)){
            mPos.z = mStart.z + mLength;
        }
        
        mPos += mVel * mSpeed;
    };
    
    inline void draw(){
        
        
    }
};


#endif
