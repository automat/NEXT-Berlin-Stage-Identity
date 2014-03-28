//
//  AbstractField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_AbstractField_h
#define grid_00_AbstractField_h

#include "world/Index.h"
#include "world/grid/Cell.h"


class AbstractField : public Cell {
    
    
public:
    AbstractField(const Index& index, const Vec3f& pos) :
    Cell(index, pos){
        
    }
};

#endif
