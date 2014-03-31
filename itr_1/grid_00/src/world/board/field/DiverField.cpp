#include "world/board/field/DiverField.h"
#include "Config.h"
#include "cinder/Rand.h"

DiverField::DiverField(const Vec3f& pos, int numPathSlices) :
    AbstractField(pos, 1, numPathSlices){
        mSurfaceDisplacement = Vec3f(Rand::randInt(0, WORLD_GRID_NUM_CELLS_XY),0,Rand::randInt(0, WORLD_GRID_NUM_CELLS_XY));
        mSurfaceAmplitude    = DIVER_FIELD_SURFACE_PATH_AMPLITUDE;
        mSurfaceDensity      = DIVER_FIELD_SURFACE_PATH_DENSITY;
        mSurfaceOffsetSpeed  = DIVER_FIELD_SURFACE_PATH_OFFSET_SPEED;
    
}

void DiverField::debugDrawArea(){
    glColor3f(1,0,0);
    debugDrawArea_Internal();
}

void DiverField::draw(){
    
}

void DiverField::update(Oscillator* osc, float t){
    mSurfaceOffset += mSurfaceOffsetSpeed;
    updatePathSurface(osc, mSurfaceOffset);
    
}