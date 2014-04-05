#include "world/board/path/PathSurface.h"
#include "Config.h"
#include "world/Oscillator.h"
#include "world/board/path/PathSlice.h"

#include <OpenGL/OpenGL.h>
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include <cassert>

using namespace boost::assign;

PathSurface::PathSurface(){}

PathSurface::PathSurface(const Vec3f& pos, int numSlices, int size){
    set(pos,numSlices,size);
}

void PathSurface::set(const Vec3f &pos, int numSlices, int size){
    mSlices.clear();
    mPoints.clear();
    
    mPos       = pos;
    mNumSlices = numSlices;
    mSize      = size;
    
    //
    //  Init points
    //
    mDistance       = float(mSize);                                // cell border left to cell border right + width
    mNumPointsSlice = mDistance * 0.5f * PATH_SLICE_NUM_POINTS;    // number of points per slice, relative to distance
    mNumPoints      = mNumSlices * mNumPointsSlice;
    
    int   numPointsSlice_1 = mNumPointsSlice - 1;
    mSliceWidth            = 1.0f/ float(mNumSlices);
    float marginX          = -0.5f - mSliceWidth * 0.5f;
    
    Vec3f start = Vec3f(marginX,0, 0.5f); // to border right
    Vec3f end   = Vec3f(marginX,0,-mSize+0.5f);       // to border left
    float a;
    
    
    int i,j;
    i = -1;
    while (++i < mNumSlices){
        start.x = end.x = end.x + mSliceWidth;
        
        mPoints += start; // add start
        j = 0;
        while (++j < numPointsSlice_1) {
            a = float(j) / float(numPointsSlice_1);
            mPoints += start * (1.0f - a) + end * a;  // add between start end
        }
        mPoints += end; // add end
        
        mSlices += PathSlice(this,i,mSliceWidth,mNumPointsSlice);
    }
}

PathSurface& PathSurface::operator=(const PathSurface &rhs){
    if(this != &rhs){
        mPos = rhs.mPos;
        mNumSlices = rhs.mNumSlices;
        mSize = rhs.mSize;
        mDistance = rhs.mDistance;
        mNumPointsSlice = rhs.mNumPointsSlice;
        mPoints = rhs.mPoints;
        mSlices = rhs.mSlices;
        for (vector<PathSlice>::iterator itr = mSlices.begin(); itr != mSlices.end(); ++itr) {
            itr->mSurface = this;
        }
    }
    return *this;
}

void PathSurface::update(Oscillator* osc, const Vec3f& pos, float density, float amplitude, float offset){
    for (vector<Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); ++itr) {
        itr->y = osc->getValue(pos.x + itr->x * density,
                               pos.z + itr->z * density,
                               offset) * amplitude;
    }
}

void PathSurface::debugDraw(){
    static const Vec3f zero;

    glColor3f(0,0,1);
    glPointSize(2);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
    glDrawArrays(GL_POINTS, 0, mPoints.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glPointSize(1);
}


PathSlice* PathSurface::getSlicePtr(int index){
    return &mSlices[index];
}

const vector<PathSlice>& PathSurface::getSlices(){
    return mSlices;
}

const Vec3f& PathSurface::getPoint(int slice, int index){
    return mPoints[slice * mNumPointsSlice + index];
}

const Vec3f& PathSurface::getStart(int slice){
    return mPoints[slice * mNumPointsSlice];
}

const Vec3f& PathSurface::getEnd(int slice){
    return mPoints[slice * mNumPointsSlice + mNumPointsSlice - 1];
}

const PathSlice& PathSurface::getSlice(int slice){
    return mSlices[slice];
}
