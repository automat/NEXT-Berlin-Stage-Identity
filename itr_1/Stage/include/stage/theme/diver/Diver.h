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
#include "cinder/Camera.h"

#include "Config.h"
#include "stage/theme/path/PathSlice.h"

using namespace ci;
using namespace std;

namespace next{

    class Diver{
    protected:

        PathSlice* mPathSlice;
        int        mPathSurfaceSize;

        float      mSpeed;
        float      mOffset;
        float      mOffsetInitial;
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
        vector<float> mTexcoords;

    public:
        Diver(PathSlice* pathSlice,
                int        numPoints,
                float      offset,
                float      speed,
                float      length,
                float      height = 0.125f);

        void reset(PathSlice* pathSlice,
                int        numPoints,
                float      offset,
                float      speed,
                float      length,
                float      height);

        inline const vector<Vec3f>& getPoints() const{
            return mPoints;
        }

        inline const vector<float>& getTexcoords() const{
            return mTexcoords;
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

        void setOffset(float offset);
        void increaseOffset(float value);
        
        void update();
        void updateInOut();
        void updateTexcoords();

        void debugDraw();
        void debugDrawIndices(const CameraOrtho& camera);

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
        
        inline float getSpeed(){
            return mSpeed;
        }
        
        inline float getOffset(){
            return mOffset;
        }
        
        inline float getOffsetInitial(){
            return mOffsetInitial;
        }
        
        inline float getLength(){
            return mLength;
        }
    };
}

#endif
