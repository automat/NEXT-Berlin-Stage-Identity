#include "world/board/path/PathSurface.h"
#include "Config.h"
#include "world/Oscillator.h"
#include "world/board/path/PathSlice.h"

#include <OpenGL/OpenGL.h>
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include <cassert>

using namespace boost::assign;

PathSurface::PathSurface(){mNumSlices = -1;}

PathSurface::PathSurface(const Vec3f& pos, int numSlices, int width) :
mPos(pos),mNumSlices(numSlices),mWidth(width){
    //
    //  Init points
    //
    mDistance       = pos.z + float(width) + 1;             // cell border left to cell border right + width
    mNumPointsSlice = mDistance * 0.5f * PATH_SLICE_NUM_POINTS;    // number of points per slice, relative to distance
    mNumPoints      = mNumSlices * mNumPointsSlice;
    
    int   numPointsSlice_1 = mNumPointsSlice - 1;
    float sliceWidth       = 1.0f/ float(mNumSlices);
    float marginX          = -0.5f - sliceWidth * 0.5f;
    
    Vec3f start = Vec3f(marginX,0,mWidth-0.5f); // to border right
    Vec3f end   = Vec3f(marginX,0,-0.5f);       // to border left
    float a;
    
    
    int i,j;
    i = -1;
    while (++i < mNumSlices){
        start.x = end.x = end.x + sliceWidth;
        
        mPoints += start;
        j = 0;
        while (++j < numPointsSlice_1) {
            a = float(j) / float(numPointsSlice_1);
            mPoints += start * (1.0f - a) + end * a;
        }
        mPoints += end;
        
        mSlices += PathSlice(this,i,sliceWidth,mNumPointsSlice);
    }
}

PathSurface& PathSurface::operator=(const PathSurface &rhs){
    if(this != &rhs){
        mPos = rhs.mPos;
        mNumSlices = rhs.mNumSlices;
        mWidth = rhs.mWidth;
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

void PathSurface::update(Oscillator* osc, const Vec3f& pos, float amplitude, float offset){
    static const float scale = 0.75f;
    for (vector<Vec3f>::iterator itr = mPoints.begin(); itr != mPoints.end(); ++itr) {
        itr->y = osc->getValue(pos.x + itr->x * amplitude,
                               pos.z + itr->z * amplitude,
                               offset) * scale;
    }
}

void PathSurface::debugDraw(){
    static const Vec3f zero;
    
    const float verticesArea[12] = {
        -0.5f,0,-0.5f,
        0.5f,0,-0.5f,
        0.5f,0,  mWidth - 0.5f,
        -0.5f,0, mWidth - 0.5f
    };
    
    glLineWidth(3);
    glColor3f(0,0,1);
    glPushMatrix();
    glTranslatef(mPos.x, mPos.y, mPos.z);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &verticesArea[0]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glVertexPointer(3, GL_FLOAT, 0, &zero.x);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
    glLineWidth(1);
    
    glColor3f(0,0,1);
    glPointSize(2);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0].x);
    glDrawArrays(GL_POINTS, 0, mPoints.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glPointSize(1);
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
