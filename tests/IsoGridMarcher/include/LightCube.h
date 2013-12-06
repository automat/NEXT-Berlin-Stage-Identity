//
//  GridCube.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 28/11/13.
//
//

#ifndef IsoGridMarcher_GridCube_h
#define IsoGridMarcher_GridCube_h

#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "LightBulb.h"
#include <cassert>

class LightCube {
private:
    int mX,mY,mZ;
    const static int NUM_BULBS = 18;
    LightBulb* mBulbs[NUM_BULBS];
    
public:
    LightCube(int x = 0, int y = 0, int z = 0) :
    mX(x),
    mY(y),
    mZ(z){
        mBulbs[ 0] = mBulbs[ 1] = mBulbs[ 2] = mBulbs[ 3] =    // bottom
        mBulbs[ 4] = mBulbs[ 5] = mBulbs[ 6] = mBulbs[ 7] =    // top
        mBulbs[ 8] = mBulbs[ 9] = mBulbs[10] = mBulbs[11] =    // sides
        mBulbs[12] = mBulbs[13] =                              // bottom top diagonal
        mBulbs[14] = mBulbs[15] = mBulbs[16] = mBulbs[17] = 0; // sided diagonal
    }
    
    
    
    void set(LightBulb* bulb00, LightBulb* bulb01, LightBulb* bulb02, LightBulb* bulb03,
             LightBulb* bulb04, LightBulb* bulb05, LightBulb* bulb06, LightBulb* bulb07,
             LightBulb* bulb08, LightBulb* bulb09, LightBulb* bulb10, LightBulb* bulb11,
             LightBulb* bulb12, LightBulb* bulb13,
             LightBulb* bulb14, LightBulb* bulb15, LightBulb* bulb16, LightBulb* bulb17){
        
        mBulbs[ 0] = bulb00; mBulbs[ 1] = bulb01; mBulbs[ 2] = bulb02; mBulbs[ 3] = bulb03;
        mBulbs[ 4] = bulb04; mBulbs[ 5] = bulb05; mBulbs[ 6] = bulb06; mBulbs[ 7] = bulb07;
        mBulbs[ 8] = bulb08; mBulbs[ 9] = bulb09; mBulbs[10] = bulb10; mBulbs[11] = bulb11;
        mBulbs[12] = bulb12; mBulbs[13] = bulb13;
        mBulbs[14] = bulb14; mBulbs[15] = bulb15; mBulbs[16] = bulb16; mBulbs[17] = bulb17;
    }
    
    void switchOn(){
        int i = -1;
        while (++i < NUM_BULBS) {
            mBulbs[i]->switchOn();
        }
    }
    
    void switchOn(int index){
        mBulbs[index]->switchOn();
    }
    
    
    void switchOff(){
        int i = -1;
        while (++i < NUM_BULBS) {
            mBulbs[i]->switchOff();
        }
    }
    
    void switchOff(int index){
        mBulbs[index]->switchOff();
    }
    
    void switchRandom(float triggerTolerance = 0.0015f){
        int i = -1;
        while (++i < NUM_BULBS) {
            mBulbs[i]->switchRandom(triggerTolerance);
        }
    }
    
    int getNumBulbs(){
        return NUM_BULBS;
    }
    
    int getIX(){ return mX; }
    int getIY(){ return mY; }
    int getIZ(){ return mZ; }
    
    
    LightBulb* operator[](int n){
        return mBulbs[n];
    }
    
    const LightBulb* operator[](int n) const{
        return mBulbs[n];
    }
    

};

#endif
