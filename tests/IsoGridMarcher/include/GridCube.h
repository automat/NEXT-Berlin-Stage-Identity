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

class GridCube {
private:
    ci::Vec3f mPos;
    ci::Vec3f mSize;
    
public:
    GridCube(){
    }
    
    GridCube(const ci::Vec3f& pos,const ci::Vec3f& size){
        mPos  = pos;
        mSize = size;
    }
    

    
    const ci::Vec3f& getPosition() const{
        return mPos;
    }
    
    const ci::Vec3f& getSize() const{
        return mSize;
    }
};

#endif
