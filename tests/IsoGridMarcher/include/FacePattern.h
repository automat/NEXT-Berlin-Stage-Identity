//
//  FacePattern.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 09/12/13.
//
//

#ifndef IsoGridMarcher_FacePattern_h
#define IsoGridMarcher_FacePattern_h

#include "Cube.h"
#include "cinder/Vector.h"

class FacePattern {
private:
    std::vector<uint> mI;
    
public:
    ci::Vec3f pos;
    
    FacePattern(int x = 0, int y = 0, int z = 0) :
        pos(x,y,z){}
    
    void addFace(uint i0, uint i1, uint i2){
        mI.push_back(i0);
        mI.push_back(i1);
        mI.push_back(i2);
    }
    
    int size(){
        return mI.size();
    }
    
    const int size() const{
        return mI.size();
    }
    
    uint& operator[](int n){
        return mI[n];
    }
    
    const uint& operator[](int n) const{
        return mI[n];
    }
};



#endif
