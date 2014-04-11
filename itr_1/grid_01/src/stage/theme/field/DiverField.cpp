#include "stage/theme/field/DiverField.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "cinder/Rand.h"

#include "Config.h"
#include "util/ColorUtil.h"

using namespace boost::assign;

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
        mMeshLayout.setStaticColorsRGB();
        reset_Internal();
}

void DiverField::addMeshColors(){
    const Vec3f up(1,1,1),down(0,-1,0);
    const Vec3f left(-1,0,0),right(1,0,0);
    const Vec3f front(0,0,-1),back(0,0,1);
    
#ifndef DIVER_FIELD_PUT_NORMAL_COLORS
    Colorf white(1,1,1);
#endif
    
    vector<Colorf> meshColors;  // buffer colors for debug
    
    int i,j;
    i = -1;
    while (++i < mNumDivers) {
        j = 0;
        while(j < mDiverNumPoints){
#ifdef DIVER_FIELD_PUT_NORMAL_COLORS
            meshColors += utils::toColor(down), utils::toColor(down);
            meshColors += utils::toColor(  up), utils::toColor(  up);
            meshColors += utils::toColor(right),utils::toColor(left);
            meshColors += utils::toColor(right),utils::toColor(left);
#else
            meshColors += white,white;
            meshColors += white,white;
            meshColors += white,white;
            meshColors += white,white;
#endif
            ++j;
        }
#ifdef DIVER_FIELD_PUT_NORMAL_COLORS
        meshColors += utils::toColor(front),utils::toColor(front);
        meshColors += utils::toColor(front),utils::toColor(front);
        meshColors += utils::toColor( back),utils::toColor( back);
        meshColors += utils::toColor( back),utils::toColor( back);
#else
        meshColors += white,white;
        meshColors += white,white;
        meshColors += white,white;
        meshColors += white,white;
#endif
    }
    
    mMesh.bufferColorsRGB(meshColors);
}

void DiverField::debugDrawArea(){
    glColor3f(1,0,0);
    debugDrawArea_Internal();
}

void DiverField::draw(){
    drawMesh();
}

void DiverField::update(Oscillator* osc, float t){
    mSurfaceOffset -= mSurfaceOffsetSpeed;
    updatePathSurface(osc, mSurfaceOffset);
    updateDivers();
    
    updateMesh();
}