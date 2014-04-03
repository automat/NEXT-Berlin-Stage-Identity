#include "world/board/path/PathSlice.h"
#include "world/board/path/PathSurface.h"

PathSlice::PathSlice(PathSurface* surface, int index, float width, int numPoints) :
    mSurface(surface),mIndex(index),mWidth(width),mNumPoints(numPoints){
}

const Vec3f& PathSlice::getStart() const{
    return mSurface->getStart(mIndex);
}

const Vec3f& PathSlice::getEnd() const{
    return mSurface->getEnd(mIndex);
}

const Vec3f& PathSlice::getPoint(int index) const{
    return mSurface->getPoint(mIndex, index);
}

int PathSlice::getSurfaceSize() const{
    return mSurface->getSize();
}

void PathSlice::getPointOn(float intrpl, Vec3f *out) const{
    intrpl+= 0.0000001f; //prevent strange rounding errors
    intrpl = max(0.0f, min(intrpl,1.0f));
    //intrpl = 1.0f - intrpl;
    
    int size = mNumPoints;
    int len  = size - 1;
    
    uint   index  = uint(floorf(float(len) * intrpl));
    uint   index1 = uint(min(index + 1, uint(len)));
    float  localIntrpl    = fmodf(intrpl, 1.0f / float(len) ) * float(len);
    float  localIntrplInv = 1.0f - localIntrpl;
    
    const Vec3f& p0 = mSurface->getPoint(mIndex, index);
    const Vec3f& p1 = mSurface->getPoint(mIndex, index1);
    
    *out = p0 * localIntrplInv + p1 * localIntrpl;
}

float PathSlice::getLengthSq() const{
    float sum = 0;
    const vector<Vec3f>& points = mSurface->getPoints();
    
    int numPoints_1 = mNumPoints - 1;
    int i = mIndex * mNumPoints - 1;
    while (++i < numPoints_1) {
        sum += points[i].distanceSquared(points[i+1]);
    }
    
    return sum;
}

float PathSlice::getLength() const{
    return sqrt(getLengthSq());
}



