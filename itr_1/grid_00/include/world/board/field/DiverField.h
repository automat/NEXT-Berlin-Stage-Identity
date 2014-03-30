//
//  DiverField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_DiverField_h
#define grid_00_DiverField_h

#include "world/board/field/AbstractField.h"
#include <map>

typedef std::map<const Index,class DiverField*> IndexDiverFieldMap;

class DiverField : public AbstractField {
    
public:
    DiverField(const Index& index, const Vec3f& pos);
    void debugDrawArea();
    
};


#endif
