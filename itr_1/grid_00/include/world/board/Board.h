//
//  Board.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Board_h
#define grid_00_Board_h

#include <vector>

#include "cinder/Camera.h"

#include "layout/geom/LayoutArea.h"
#include "layout/quote/Quote.h"
#include "layout/quote/QuoteLine.h"

#include "world/Index.h"
#include "world/grid/Grid.h"
#include "world/Oscillator.h"
#include "world/Index.h"



#include "world/board/field/DiverField.h"
#include "world/board/field/QuoteField.h"


using namespace std;
using namespace ci;

class Board{
    vector<DiverField*> mDiverFields;           //  diverfields
    IndexDiverFieldMap  mIndexDiverFieldMap;    //  index / diverfield map
    
    vector<QuoteField*> mQuoteFields;           //  quote fields
    IndexQuoteFieldMap  mIndexQuoteFieldMap;    //  index / quotefield map
    
    Grid*       mGrid;
    Oscillator* mOscillator;
    
    QuoteLine mTempQuoteLine;
    
    
    
public:
    Board(Grid* grid, const LayoutArea& area);
    ~Board();

    
    void setQuote(const Quote& quote, float duration);
    
    void draw(const CameraOrtho& camera);
    void update();
};


#endif
