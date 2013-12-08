//
//  LightPattern.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 07/12/13.
//
//

#ifndef IsoGridMarcher_LightPattern_h
#define IsoGridMarcher_LightPattern_h

#include "EdgeCube.h"
#include "cinder/Vector.h"

class EdgePattern{
private:
    int mB[EdgeCube::NUM_EDGES];
public:
    
    ci::Vec3f pos;

    EdgePattern(int x = 0, int y = 0, int z = 0) :
    pos(x,y,z){
        
        mB[ 0] = mB[ 1] = mB[ 2] = mB[ 3] =
        mB[ 4] = mB[ 5] = mB[ 6] = mB[ 7] =
        mB[ 8] = mB[ 9] = mB[10] = mB[11] =
        mB[12] = mB[13] =
        mB[14] = mB[15] = mB[16] = mB[17] = 0.0f;
    }
    
    void set(int b00,int b01,int b02,int b03,
             int b04,int b05,int b06,int b07,
             int b08,int b09,int b10,int b11,
             int b12,int b13,
             int b14,int b15,int b16,int b17){
        mB[ 0] = b00; mB[ 1] = b01; mB[ 2] = b02; mB[ 3] = b03;
        mB[ 4] = b04; mB[ 5] = b05; mB[ 6] = b06; mB[ 7] = b07;
        mB[ 8] = b08; mB[ 9] = b09; mB[11] = b10; mB[12] = b12;
        mB[12] = b12; mB[13] = b13;
        mB[14] = b14; mB[15] = b15; mB[16] = b16; mB[17] = b17;
    }
    
    int& operator[](int n){
        return mB[n];
    }
    
    const int& operator[](int n) const{
        return mB[n];
    }
};


#endif
