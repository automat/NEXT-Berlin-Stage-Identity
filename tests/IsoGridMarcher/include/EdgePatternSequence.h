//
//  LightCubePattern.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 06/12/13.
//
//

#ifndef IsoGridMarcher_LightCubePattern_h
#define IsoGridMarcher_LightCubePattern_h

#include "EdgePattern.h"
#include <vector>
#include "cinder/Vector.h"


class EdgePatternSequence {
    int mSizeX;
    int mSizeY;
    int mSizeZ;
    
    std::vector<EdgePattern> mPatterns;
    
public:
    ci::Vec3f pos;
    
    EdgePatternSequence(int size = 1, int x = 0, int y = 0, int z = 0) :
    pos(x,y,z){
        mPatterns.resize(size);
    }
    
    
    const EdgePattern& operator[](int n) const{
        return mPatterns[n];
    }
    
    EdgePattern& operator[](int n){
        return mPatterns[n];
    }
    
    std::vector<EdgePattern> getPatterns(){
        return mPatterns;
    }
    
    const std::vector<EdgePattern> getPatterns() const{
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
