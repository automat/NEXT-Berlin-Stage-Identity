//
//  QuoteField.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_QuoteField_h
#define grid_00_QuoteField_h

#include "world/board/field/AbstractField.h"
#include "layout/quote/QuoteLine.h"
#include <map>
#include "cinder/Camera.h"

typedef std::map<const Index, class QuoteField*> IndexQuoteFieldMap;

class QuoteField : public AbstractField {
    QuoteLine* mQuote;
    
    void updateMeshTexcoords();
    void updateDivers();
    
public:
    QuoteField(const Vec3f& pos, int numPathSlices, QuoteLine* quoteLine);
    
    void debugDrawArea();
    

    
    void draw();
    void update(Oscillator* osc, float t);
    void reset(const Vec3f& pos, int numPathSlices, QuoteLine* quoteLine);
    
    
    void debugDrawIndices(const CameraOrtho& camera);
    void debugDrawDiverIndices(const CameraOrtho& camera);
};

#endif
