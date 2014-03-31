//
//  AbstractField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_AbstractField_h
#define grid_00_AbstractField_h

#include <boost/assign/std/vector.hpp>
#include <OpenGL/OpenGL.h>

#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Matrix44.h"
#include "cinder/Rand.h"
#include "cinder/gl/Vbo.h"

#include "Config.h"

#include "world/Index.h"
#include "world/grid/Cell.h"
#include "world/Oscillator.h"
#include "world/board/path/PathSurface.h"
#include "world/board/diver/Diver.h"

using namespace ci;
using namespace std;

class AbstractField : public Cell {
protected:
    bool mActive;
   
    
public:
    AbstractField(const Index& index, const Vec3f& pos);
    ~AbstractField();
};

#endif
