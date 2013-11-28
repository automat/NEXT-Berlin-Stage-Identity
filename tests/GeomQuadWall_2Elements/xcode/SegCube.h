//
//  ParticleCube.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 26/11/13.
//
//

#ifndef TriangleGeometry_ParticleCube_h
#define TriangleGeometry_ParticleCube_h

#include "SegSkeleton.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include <vector>


class SegCube : public SegSkeleton{
private:
    std::vector<ci::Vec3f> mScaledVertices;
    void init();

protected:
    void update_Internal();
    
public:
    SegCube();
    SegCube(std::vector<ci::Vec3f>* path);
    void draw();
};

#endif
