//
//  DiverField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_DiverField_h
#define grid_00_DiverField_h

#include <map>
#include "stage/theme/field/AbstractField.h"

namespace next{
    class DiverField : public AbstractField {

        void addMeshColors();
    public:
        DiverField(const Vec3f& pos, int numPathSlices);
        void debugDrawArea();

        void draw();
        void update(Oscillator* osc, float t);
    };
}


#endif
