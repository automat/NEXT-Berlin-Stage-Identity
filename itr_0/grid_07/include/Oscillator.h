//
//  Oscillator.h
//  grid_04
//
//  Created by Henryk Wollik on 02/03/14.
//
//

#ifndef grid_04_Oscillator_h
#define grid_04_Oscillator_h

#include "Settings.h"
#include "cinder/Perlin.h"
#include <math.h>

using namespace ci;
using namespace std;

class Oscillator {
    Perlin mSources[OSCILLATOR_NUM_SOURCES_XY * OSCILLATOR_NUM_SOURCES_XY];
    
public:
    Oscillator(){
        int i = -1;
        while(++i < OSCILLATOR_NUM_SOURCES_XY * OSCILLATOR_NUM_SOURCES_XY){
            mSources[i] = Perlin(OSCILLATOR_OCTAVES,OSCILLATOR_SEED);
        }
    }
    
    inline float getValue(float x, float y, float t, int sourceX = 0, int sourceY = 0){
        return mSources[sourceY * OSCILLATOR_NUM_SOURCES_XY + sourceX].fBm(x,y,t);
    }

};

#endif
