//
//  GridEdge.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef IsoGridMarcher_GridEdge_h
#define IsoGridMarcher_GridEdge_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Matrix44.h"
#include "cinder/Quaternion.h"
#include <OpenGL/OpenGL.h>

class GridEdge {
private:
    ci::Vec3f* mP0;
    ci::Vec3f* mP1;
    
    
public:
    GridEdge(){
        
    }
    
    GridEdge(ci::Vec3f* p0, ci::Vec3f* p1){
        mP0 = p0;
        mP1 = p1;
    }
    
    void draw(){
        ci::Vec3f tangent = *mP1 - *mP0;
        float dist = tangent.length();
        tangent.normalize();
        
        ci::Vec3f binormal = tangent.cross(tangent.dot(ci::Vec3f::yAxis()) > 0.998f ?
                                                       ci::Vec3f::xAxis() :
                                                       ci::Vec3f::yAxis()).normalized();
        ci::Vec3f normal = binormal.cross(tangent);
        normal.normalize();
        
        ci::Matrix44f mat = ci::Matrix44f::createRotationOnb(tangent, normal, binormal);
        ci::Quatf quat(mat);

        glPushMatrix();
        ci::gl::translate(*mP0);

        
        glPopMatrix();
        ci::gl::drawLine(*mP0, *mP1);
    }
}; 

#endif
