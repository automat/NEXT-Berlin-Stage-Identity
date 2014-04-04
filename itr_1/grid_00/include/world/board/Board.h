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
    LayoutArea          mArea;
    
    vector<DiverField*> mDiverFields;           //  diverfields
    IndexDiverFieldMap  mIndexDiverFieldMap;    //  index / diverfield map
    
    vector<Quote>*      mQuotes;                //  qoute data
    Quote*              mQuoteCurrent;          //  current quote beeing used
    int                 mQuoteFieldsNumMax;     //  maximun num of quote cells according to quote data
    int                 mQuoteFieldsNumCurr;    //  current num of quote cells
    vector<QuoteField*> mQuoteFields;           //  quote fields
    IndexQuoteFieldMap  mIndexQuoteFieldMap;    //  index / quotefield map
    
    Grid*       mGrid;
    Oscillator* mOscillator;
    
    QuoteLine mTempQuoteLine;
    
    
    void deleteQuoteFields();
    void deleteDiverFields();
    void setQuote(Quote& quote);
    
public:
    Board(Grid* grid, const LayoutArea& area, vector<Quote>* quotes);
    ~Board();

    void setQuote(const Quote& quote, float duration);
    
    void draw(const CameraOrtho& camera);
    void update();

    inline const LayoutArea& getArea(){
        return mArea;
    }
    
    inline const LayoutArea& getArea() const{
        return mArea;
    }
    
    inline const Quote* getCurrentQuote() const{
        return mQuoteCurrent;
    }
};


#endif
