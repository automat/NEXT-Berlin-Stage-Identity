//
//  Lantern.h
//  grid_01
//
//  Created by Henryk Wollik on 08/04/14.
//
//

#ifndef grid_01_Lantern_h
#define grid_01_Lantern_h

#include "cinder/gl/Light.h"

using namespace ci;
class Lantern : public gl::Light{
    
public:
    Lantern(int iD) : gl::Light(gl::Light::DIRECTIONAL,iD){}
    
    void wakeUp(){}
    void tearDown(){}

};



#endif
