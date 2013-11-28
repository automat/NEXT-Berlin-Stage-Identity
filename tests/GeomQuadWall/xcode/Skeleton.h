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


class Skeleton : public ci::TriMesh{
private:
    std::vector<ci::Vec3f>* mPath;
    std::vector<ci::Vec2f>  mOffsets;
    int mNumJoints;
    
    float mRangeBegin;
    float mRangeEnd;
    float mRangeMin;
    
    bool mOffsetsSet;
    
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
    Skeleton(int numPoints = 2);
    Skeleton(std::vector<ci::Vec3f>* path, int numPoints = 2);
    ~Skeleton();
    
    void update();
    
    void setPath(std::vector<ci::Vec3f>* path);
    void setPathRange(float rangeMin, float rangeMax);
    void setOffset(float offsetX,float offsetY,int index = -1);
    void setScale(float x, float y, float z);
    void setScale(float x);
    void inverseDirection();
    
    
    void draw();
    void drawDebug();
    void drawPath();
    
    
    
};



#endif
