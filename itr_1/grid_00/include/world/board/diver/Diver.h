//
//  Diver.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Diver_h
#define grid_00_Diver_h

#include <vector>

#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "Config.h"
#include "world/board/path/PathSlice.h"

using namespace ci;
using namespace std;


class Diver{
    PathSlice* mPathSlice;
    Vec3f      mPos;
    
    float      mSpeed;
    float      mOffset;
    float      mLength;
    float      mLengthStep;
    
    float      mPathLength;
    float      mPathLengthInv;
    
    float      mWidth;
    float      mHeight;
    
    bool   mIsHidden;   // diver wont loop anymore
    bool   mIsOut;      // diver is outside of visible area
    bool   mIsOutPrev;  // diver previous visibility state
    
    int           mNumPoints;
    vector<Vec3f> mPoints;
    
public:
    Diver(PathSlice* pathSlice,
          int        numPoints,
          float      offset,
          float      speed,
          float      length,
          float      height = 0.125f);
    
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
    
    void update();
    void updateInOut();
    void debugDraw();
    
    
    inline void hide(){
        mIsHidden = true;
    }
    
    inline void show(){
        mIsHidden = false;
    }
    
    inline float getWidth(){
        return mWidth;
    }
    
    inline float getHeight(){
        return mHeight;
    }
    
    
    
};

#endif
