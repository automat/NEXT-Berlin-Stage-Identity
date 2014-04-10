//
//  DiverField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_DiverField_h
#define grid_00_DiverField_h

#include "world/theme/field/AbstractField.h"
#include <map>

typedef std::map<const Index,class DiverField*> IndexDiverFieldMap;

class DiverField : public AbstractField {

    void addMeshColors();
public:
    DiverField(const Vec3f& pos, int numPathSlices);
    void debugDrawArea();
    
    void draw();
    void update(Oscillator* osc, float t);
};


#endif
