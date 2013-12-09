//
//  FacePatternSequence.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 09/12/13.
//
//

#ifndef IsoGridMarcher_FacePatternSequence_h
#define IsoGridMarcher_FacePatternSequence_h

#include "FacePattern.h"
#include <vector>
#include "cinder/Vector.h"

class FacePatternSequence {
    int mSizeX;
    int mSizeY;
    int mSizeZ;
    
    std::vector<FacePattern> mPatterns;
    
public:
    ci::Vec3f pos;
    
    FacePatternSequence(int size = 1, int x = 0, int y = 0, int z = 0) :
    pos(x,y,z){
        mPatterns.resize(size);
    }
    
    const FacePattern& operator[](int n) const{
        return mPatterns[n];
    }
    
    FacePattern& operator[](int n){
        return mPatterns[n];
    }
    
    std::vector<FacePattern> getPatterns(){
        return mPatterns;
    }
    
    const std::vector<FacePattern> getPatterns() const{
        return mPatterns;
    }
    
    int getNumPatterns(){
        return mPatterns.size();
    }
    
    const int getNumPatterns() const{
        return mPatterns.size();
    }
    
};


#endif
