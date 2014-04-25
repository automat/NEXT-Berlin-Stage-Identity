//
//  ColorUtil.h
//  grid_00
//
//  Created by Henryk Wollik on 01/04/14.
//
//

#ifndef grid_00_ColorUtil_h
#define grid_00_ColorUtil_h

#include "cinder/Color.h"
#include "cinder/Vector.h"

namespace utils {
    using namespace ci;
    inline static Colorf toColor(const Vec3f& normal){
        return Colorf(normal.x * 0.5f + 0.5f,normal.y * 0.5f + 0.5f,normal.z * 0.5f + 0.5f);
    }
    
    inline static ColorAf toColorA(const Vec3f& normal, float a = 1.0f){
        return ColorAf(normal.x * 0.5f + 0.5f,normal.y * 0.5f + 0.5f,normal.z * 0.5f + 0.5f,a);
    }
}



#endif
