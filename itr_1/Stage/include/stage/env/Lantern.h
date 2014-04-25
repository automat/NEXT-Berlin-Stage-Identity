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
#include "cinder/Color.h"

using namespace ci;
class Lantern : public gl::Light{
    ColorAf mAmbientTarget;
    ColorAf mDiffuseTarget;
    ColorAf mSpecularTarget;
    
public:
    Lantern(int iD) : gl::Light(gl::Light::DIRECTIONAL,iD){}
    
    inline void setAmbient(const Color& ambient){
        gl::Light::setAmbient(ambient);
        mAmbientTarget.set(ambient);
    }
    
    inline void setDiffuse(const Color& diffuse){
        gl::Light::setDiffuse(diffuse);
        mDiffuseTarget.set(diffuse);
    }
    
    inline void setSpecular(const Color& specular){
        gl::Light::setSpecular(specular);
        mSpecularTarget.set(specular);
    }
    
    inline void wakeUp(){}
    inline void tearDown(){}
    inline void debugDraw(){
        glPushMatrix();
        glTranslatef(mPosition.x, mPosition.y, mPosition.z);
        glColor3f(1, 1, 1);
        gl::drawVector(Vec3f::zero(), mDirection);
        glPopMatrix();
    };

};



#endif
