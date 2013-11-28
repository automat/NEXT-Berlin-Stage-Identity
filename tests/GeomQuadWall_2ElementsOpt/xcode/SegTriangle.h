//
//  SegTriangle.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef TriangleGeometry_SegTriangle_h
#define TriangleGeometry_SegTriangle_h

#include "SegSkeleton.h"
#include <vector>
#include "cinder/Vector.h"

class SegTriangle : public SegSkeleton{
private:
    void init();
    std::vector<ci::Vec3f> mNormVertices;

protected:
    void update_Internal();
    
public:
    SegTriangle();
    SegTriangle(std::vector<ci::Vec3f>* path);
    void draw();
};


#endif
