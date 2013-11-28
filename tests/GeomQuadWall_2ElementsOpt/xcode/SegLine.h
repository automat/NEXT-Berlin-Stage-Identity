//
//  SegLine.h
//  TriangleGeometry
//
//  Created by Henryk Wollik on 27/11/13.
//
//

#ifndef TriangleGeometry_SegLine_h
#define TriangleGeometry_SegLine_h

#include "SegSkeleton.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include <vector>

class SegLine : public SegSkeleton{
private:
    int  mNumSegments;
    void init();
    
protected:
    void update_Internal();
    
public:
    SegLine(int numPoints = 3, int numSegments = 4);
    SegLine(std::vector<ci::Vec3f>* path, int numPoints = 3, int numSegments = 4);
    void draw();
};


#endif
