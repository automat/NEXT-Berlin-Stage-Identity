//
//  Oscillator.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Oscillator_h
#define grid_00_Oscillator_h

#include "cinder/Perlin.h"

using namespace ci;

class Oscillator {
    Perlin mPerlin;
    
public:
    Oscillator();
    float getValue(float x, float y, float t);
    float getValue(float x, float y, float offsetX, float offsetY, float t);
    
};


#endif
