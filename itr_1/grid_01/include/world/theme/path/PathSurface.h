//
//  PathSurface.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_PathSurface_h
#define grid_00_PathSurface_h

#include <vector>
#include "cinder/Vector.h"
#include "world/theme/path/PathSlice.h"

using namespace ci;
using namespace std;


class Oscillator;

class PathSurface {
    Vec3f mPos;             //  center position in cell
    int   mSize;            //  size of surface to the left
    int   mNumSlices;       //  overall num slices
    int   mNumPointsSlice;  //  num points per slices
    int   mNumPoints;       //  mNumSlices * mNumPointsSlice
    float mDistance;        //  distance from left to right
    float mSliceWidth;      //  width of slice
    
    vector<Vec3f>     mPoints;
    vector<Vec3f>     mTexcoords;
    vector<PathSlice> mSlices;
    
    
public:
    PathSurface();
    PathSurface(const Vec3f& pos, int numSlices = 1, int size = 1);
    
    void set(const Vec3f& pos, int numSlices = 1, int size = 1);
    
    PathSurface& operator=(const PathSurface& rhs);
    
    void update(Oscillator* osc, const Vec3f& pos, float density, float amplitude, float offset);
    void debugDraw();
    void drawSurface();
    
    inline int getNumSlices(){
        return mNumSlices;
    }
    
    //! get size to the left
    inline int getSize(){
        return mSize;
    }
    
    inline const vector<Vec3f>& getPoints(){
        return mPoints;
    }
    
    //! get width of a single slice
    inline float getSliceWidth(){
        return mSliceWidth;
    }
    
    
    const vector<PathSlice>& getSlices();
    const Vec3f& getStart(int slice);
    const Vec3f& getEnd(int slice);
    const Vec3f& getPoint(int slice, int index);
    const PathSlice& getSlice(int index);
    
    PathSlice* getSlicePtr(int index);
    
    
    
    
    
};

#endif
