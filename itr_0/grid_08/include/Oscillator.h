//
//  Oscillator.h
//  grid_04
//
//  Created by Henryk Wollik on 02/03/14.
//
//

#ifndef grid_04_Oscillator_h
#define grid_04_Oscillator_h

#include "Config.h"
#include "cinder/Perlin.h"
#include <math.h>
#include <vector>

using namespace ci;
using namespace std;
using namespace config;

class Oscillator {
    vector<Perlin> mSources;
    
public:
    Oscillator(){
        reset();
    }
    
    inline void reset(){
        mSources.clear();
        mSources.resize(OSCILLATOR_NUM_SOURCES_XY * OSCILLATOR_NUM_SOURCES_XY);
        int i = -1;
        while(++i < mSources.size()){
            mSources[i] = Perlin(OSCILLATOR_OCTAVES,OSCILLATOR_SEED);
        }
        
    }
    
    inline float getValue(float x, float y, float t, int sourceX = 0, int sourceY = 0){
        return mSources[sourceY * OSCILLATOR_NUM_SOURCES_XY + sourceX].fBm(x,y,t);
    }

};

#endif
