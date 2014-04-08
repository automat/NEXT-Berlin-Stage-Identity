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
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"

#include "util/SharedFileWatcher.h"
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
    LayoutArea mArea;
    vector<DiverField*> mDiverFields;
  
    vector<Quote>*      mQuotes;        //  quote data
    Grid*               mGrid;
    Oscillator*         mOscillator;
    Quote*              mQuoteCurrent;  //  ref to curr qzote
    vector<QuoteField*> mQuoteFields;   //  quote fields
    
    gl::GlslProg mShaderDiverFields;
    gl::GlslProg mShaderQuoteFields;
#if defined(BOARD_LIVE_EDIT_MATERIAL_SHADER)
    SharedFileWatcherRef mSharedFileWatcher;
#endif

    
    void deleteQuoteFields();
    void deleteDiverFields();
    
    void setQuote(Quote& quote);
    
    
public:
    Board(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes);
    ~Board();
    
    void draw(const CameraOrtho& camera, bool useMaterialShader);
    void update();
    
    inline const Quote* getCurrentQuote() const{
        return mQuoteCurrent;
    }
};

#endif
