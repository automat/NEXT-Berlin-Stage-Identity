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

#include "world/grid/Cell.h"
#include "world/board/path/PathSurface.h"


using namespace ci;
using namespace std;

class Oscillator;


class AbstractField{
protected:
    
    bool        mActive;
    Vec3f       mPos;
    int         mSize;
    
    Vec3f       mSurfaceDisplacement;   //  field random pos offset
    float       mSurfaceDensity;        //  field density, low/high freq
    float       mSurfaceAmplitude;      //  field amplitude scale
    float       mSurfaceOffset;
    float       mSurfaceOffsetSpeed;
    
    PathSurface mPathSurface;
    
    void updatePathSurface(Oscillator* osc,float t);
    void debugDrawArea_Internal();
    
public:
    AbstractField(const Vec3f& pos, int size, int numPathSlices);
    ~AbstractField();
    
    virtual void debugDrawArea() = 0;
    void debugDrawPathSurface();
    
    virtual void draw() = 0;
    virtual void update(Oscillator* osc, float t)= 0;
    
};

#endif
