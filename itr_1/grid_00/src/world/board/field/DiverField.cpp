#include "world/board/field/DiverField.h"
#include "Config.h"
#include "cinder/Rand.h"

DiverField::DiverField(const Vec3f& pos, int numPathSlices) :
    AbstractField(pos, 1, numPathSlices){
        mSurfaceDisplacement = Vec3f(Rand::randInt(0, WORLD_GRID_NUM_CELLS_XY),0,Rand::randInt(0, WORLD_GRID_NUM_CELLS_XY));
        mSurfaceAmplitude    = DIVER_FIELD_SURFACE_PATH_AMPLITUDE;
        mSurfaceDensity      = DIVER_FIELD_SURFACE_PATH_DENSITY;
        mSurfaceOffsetSpeed  = DIVER_FIELD_SURFACE_PATH_OFFSET_SPEED;
        
        mDiverUnitNumPoints = DIVER_FIELD_DIVER_NUM_POINTS;
        mDiverHeightMin     = DIVER_FIELD_DIVER_MIN_HEIGHT;
        mDiverHeightMax     = DIVER_FIELD_DIVER_MAX_HEIGHT;
        mDiverOffsetMin     = DIVER_FIELD_DIVER_MIN_OFFSET;
        mDiverOffsetMax     = DIVER_FIELD_DIVER_MAX_OFFSET;
        mDiverSpeedMin      = DIVER_FIELD_DIVER_MIN_SPEED;
        mDiverSpeedMax      = DIVER_FIELD_DIVER_MAX_SPEED;
        mDiverLengthMin     = DIVER_FIELD_DIVER_MIN_LENGTH;
        mDiverLengthMax     = DIVER_FIELD_DIVER_MAX_LENGTH;
        
        reset_Internal();
}

void DiverField::debugDrawArea(){
    glColor3f(1,0,0);
    debugDrawArea_Internal();
}

void DiverField::draw(){
    drawMesh();
}

void DiverField::update(Oscillator* osc, float t){
    mSurfaceOffset += mSurfaceOffsetSpeed;
    updatePathSurface(osc, mSurfaceOffset);
    updateDivers();
    
    updateMesh();
}