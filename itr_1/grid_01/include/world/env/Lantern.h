//
//  Lantern.h
//  grid_01
//
//  Created by Henryk Wollik on 08/04/14.
//
//

#ifndef grid_01_Lantern_h
#define grid_01_Lantern_h

#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include "cinder/gl/Light.h"

using namespace ci;
class Lantern : public gl::Light{
    
public:
    Lantern(int iD) : gl::Light(gl::Light::DIRECTIONAL,iD){}
    
    void wakeUp(){}
    void tearDown(){}
    void debugDraw(){
        glPushMatrix();
        glTranslatef(mPosition.x, mPosition.y, mPosition.z);
        glColor3f(1, 1, 1);
        gl::drawVector(Vec3f::zero(), mDirection);
        //gl::drawSphere(Vec3f::zero(), 0.25f, 24);
        glPopMatrix();
    };

};



#endif
