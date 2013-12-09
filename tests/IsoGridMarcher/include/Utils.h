//
//  Utils.h
//  IsoGridMarcher
//
//  Created by Henryk Wollik on 03/12/13.
//
//

#ifndef IsoGridMarcher_Utils_h
#define IsoGridMarcher_Utils_h

#include "cinder/Rand.h"
#include "cinder/Vector.h"

inline float sawf(float n){
    return 2.0f * (n  - floorf((0.5f + n )));
}

ci::Vec3f randVec3f(float rx = 1, float ry = 1, float rz = 1){
    return ci::Vec3f(ci::randFloat(-rx,rx),
                     ci::randFloat(-ry,ry),
                     ci::randFloat(-rz,rz));
}



#endif
