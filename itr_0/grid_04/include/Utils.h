//
//  Common.h
//  grid_04
//
//  Created by Henryk Wollik on 02/03/14.
//
//

#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"

namespace Utils {
    inline static Colorf toColor(const Vec3f& normal){
        return Colorf(normal.x * 0.5f + 0.5f,normal.y * 0.5f + 0.5f,normal.z * 0.5f + 0.5f);
    }
}






