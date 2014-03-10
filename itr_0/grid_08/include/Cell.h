//
//  GridCell.h
//  grid_01
//
//  Created by Henryk Wollik on 23/02/14.
//
//

#ifndef grid_01_GridCell_h
#define grid_01_GridCell_h

#include "AbstractCell.h"
#include "Config.h"

class Cell : public AbstractCell {
public:
    Cell(int* id, const Vec3f& pos, Oscillator* oscillator) :
        AbstractCell(id,pos,oscillator,Vec3f(1,1,1)){
            reset();
    }
    
    inline void reset(){
        mNumDiversMin   = CELL_MIN_NUM_DIVERS;
        mNumDiversMax   = CELL_MAX_NUM_DIVERS;
        mPathAmplitude  = CELL_PATH_AMPLITUDE;
        mOffsetSpeed    = CELL_OFFSET_SPEED;
        
        mDiverUnitNumPoints = CELL_DIVER_NUM_POINTS;
        mDiverHeightMin     = CELL_DIVER_MIN_HEIGHT;
        mDiverHeightMax     = CELL_DIVER_MAX_HEIGHT;
        mDiverOffsetMin     = CELL_DIVER_MIN_OFFSET;
        mDiverOffsetMax     = CELL_DIVER_MAX_OFFSET;
        mDiverSpeedMin      = CELL_DIVER_MIN_SPEED;
        mDiverSpeedMax      = CELL_DIVER_MAX_SPEED;
        mDiverLengthMin     = CELL_DIVER_MIN_LENGTH;
        mDiverLengthMax     = CELL_DIVER_MAX_LENGTH;
        
        reset_internal();
    }
};

#endif
