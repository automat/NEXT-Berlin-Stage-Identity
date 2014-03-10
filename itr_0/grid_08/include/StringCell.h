//
//  StringCell.h
//  grid_05
//
//  Created by Henryk Wollik on 05/03/14.
//
//

#ifndef grid_05_StringCell_h
#define grid_05_StringCell_h

#include "AbstractCell.h"
#include "Config.h"
#include "cinder/gl/Texture.h"

using namespace ci;

class StringCell : public AbstractCell {
    gl::Texture mTexture;
    
public:
    StringCell(int* id,const Vec3f& pos,Oscillator* oscillator,const Vec3f& size) :
        AbstractCell(id,pos,oscillator,size){
        
            reset();
    }
    
    inline void reset(){
        mNumDiversMin  = STRING_CELL_MIN_NUM_DIVERS;
        mNumDiversMax  = STRING_CELL_MAX_NUM_DIVERS;
        mPathAmplitude = STRING_CELL_PATH_AMPLITUDE;
        mOffsetSpeed   = STRING_CELL_OFFSET_SPEED;
        
        mDiverUnitNumPoints = STRING_CELL_DIVER_NUM_POINTS;
        mDiverHeightMin     = STRING_CELL_DIVER_MIN_HEIGHT;
        mDiverHeightMax     = STRING_CELL_DIVER_MAX_HEIGHT;
        mDiverOffsetMin     = STRING_CELL_DIVER_MIN_OFFSET;
        mDiverOffsetMax     = STRING_CELL_DIVER_MAX_OFFSET;
        mDiverSpeedMin      = STRING_CELL_DIVER_MIN_SPEED;
        mDiverSpeedMax      = STRING_CELL_DIVER_MAX_SPEED;
        mDiverLengthMin     = STRING_CELL_DIVER_MIN_LENGTH;
        mDiverLengthMax     = STRING_CELL_DIVER_MAX_LENGTH;
        
        reset_internal();
    }
    
};



#endif
