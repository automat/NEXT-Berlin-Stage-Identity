//
//  QuoteField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_QuoteField_h
#define grid_00_QuoteField_h

#include <map>
#include "cinder/Camera.h"

#include "quote/QuoteLine.h"
#include "stage/theme/field/AbstractField.h"

namespace next{
    typedef std::map<const Index, class QuoteField*> IndexQuoteFieldMap;

    class QuoteField : public AbstractField {
        Vec2f      mTexcoordStart;
        Vec2f      mTexcoordStep;

        void addMeshColors();
        
    public:
        QuoteField(const Vec3f& pos, int numPathSlices, const QuoteLine& quoteLine);

        void debugDrawArea();



        void draw();
        void update(Oscillator* osc, float t);
        void reset(const Vec3f& pos, int numPathSlices, const QuoteLine& quoteLine);
        void updateDivers(float value);

        void debugDrawIndices(const CameraOrtho& camera);
        void debugDrawDiverIndices(const CameraOrtho& camera);
    };
}


#endif
