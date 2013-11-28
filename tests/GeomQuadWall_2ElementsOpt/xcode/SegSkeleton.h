//
//  Skeleton.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 25/11/13.
//
//

#ifndef TriangleGeometry_Skeleton_h
#define TriangleGeometry_Skeleton_h

#include <vector>
#include "cinder/Vector.h"
#include <OpenGL/OpenGL.h>
#include "cinder/TriMesh.h"
#include "cinder/Matrix44.h"


class SegSkeleton : public ci::TriMesh{
private:
    std::vector<ci::Vec3f>* mPath;
    std::vector<ci::Vec2f>  mOffsets;

    float mRangeBegin;
    float mRangeEnd;
    float mRangeMin;
    
    void init(int numPoints);
    
    void getVecOnPath(float intrpl, ci::Vec3f* vec);
    void recalculateRotFrames();
    
protected:
    std::vector<ci::Vec3f> mPoints;
    std::vector<ci::Vec3f> mRotTangents;
    std::vector<ci::Vec3f> mRotNormals;
    std::vector<ci::Vec3f> mRotBinormals;
    ci::Matrix44f          mTempMatrix;
    ci::Vec3f              mObjScale;
    float mDirection;
    
    void getRotationMatrix(int index,ci::Matrix44f* matrix);
    virtual void update_Internal();
    
public:
    SegSkeleton(int numPoints = 2);
    SegSkeleton(std::vector<ci::Vec3f>* path, int numPoints = 2);
    ~SegSkeleton();
    
    //! Interpolate points in range from path, apply offsets, recalculate rotation frames,
    //! call update_internal overriden by subclass
    void update();
    //! Set path to use
    void setPath(std::vector<ci::Vec3f>* path);
    //! Set range of path to be used
    void setPathRange(float rangeMin, float rangeMax);
    //! Set horizontal & vertical offset
    void setPathOffset(float offsetX,float offsetY,int index = -1);
    
    //! Set obj scale
    void setScale(float x, float y, float z);
    void setScale(float x);
    
    //! Reverse interpolation and rotation frames order
    void inverseDirection();
    
    //! Draw the thing
    void draw();
    //! Draw rotation frames
    void drawDebug();
    //! Draw path
    void drawPath();
    
    size_t getNumPoints();
    
    
    
};



#endif
