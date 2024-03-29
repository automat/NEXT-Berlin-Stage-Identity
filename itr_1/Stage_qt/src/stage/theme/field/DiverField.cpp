#include "stage/theme/field/DiverField.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Config.h"
#include "util/ColorUtil.h"

namespace next{
    using namespace boost::assign;

    DiverField::DiverField(const Vec3f& pos, int numPathSlices) :
    AbstractField(pos, 1, numPathSlices){
        mSurfaceDisplacement = Vec3f(Rand::randInt(0, STAGE_GRID_NUM_CELLS_XY),0,Rand::randInt(0, STAGE_GRID_NUM_CELLS_XY));
        mSurfaceAmplitude    = DIVER_FIELD_SURFACE_PATH_AMPLITUDE;
        mSurfaceDensity      = DIVER_FIELD_SURFACE_PATH_DENSITY;
        mSurfaceOffsetSpeed  = DIVER_FIELD_SURFACE_PATH_OFFSET_SPEED;

        mDiverUnitNumPoints = DIVER_FIELD_DIVER_NUM_POINTS;
        mDiverHeightMin     = DIVER_FIELD_DIVER_MIN_HEIGHT;
        mDiverHeightMax     = DIVER_FIELD_DIVER_MAX_HEIGHT;
        mDiverOffsetXMin    = DIVER_FIELD_DIVER_MIN_OFFSET_X;
        mDiverOffsetXMax    = DIVER_FIELD_DIVER_MAX_OFFSET_X;
        mDiverOffsetYMin    = DIVER_FIELD_DIVER_MIN_OFFSET_Y;
        mDiverOffsetYMax    = DIVER_FIELD_DIVER_MAX_OFFSET_Y;
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

        Colorf white(1,1,1);

        vector<Colorf> meshColors;  // buffer colors for debug

        int i,j;
        i = -1;
        while (++i < mNumDivers) {
            j = 0;
            while(j < mDiverNumPoints){
                meshColors += util::toColor(down), util::toColor(down);
                meshColors += util::toColor(  up), util::toColor(  up);
                meshColors += util::toColor(right),util::toColor(left);
                meshColors += util::toColor(right),util::toColor(left);
                ++j;
            }

            meshColors += util::toColor(front),util::toColor(front);
            meshColors += util::toColor(front),util::toColor(front);
            meshColors += util::toColor( back),util::toColor( back);
            meshColors += util::toColor( back),util::toColor( back);
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
    
    void DiverField::updateDivers(){
        for(vector<Diver*>::const_iterator itr = mDivers.begin(); itr != mDivers.end(); ++itr){
            Diver* diver = (*itr);
            diver->increaseOffset(diver->getSpeed());
            diver->update();
        }
    }
    void DiverField::update(Oscillator* osc, float t){
        mSurfaceOffset -= mSurfaceOffsetSpeed;
        updatePathSurface(osc, mSurfaceOffset);
        updateDivers();

        updateMesh();
    }
}