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
#include "cinder/gl/Material.h"

#include "util/FileWatcher.h"
#include "layout/geom/LayoutArea.h"
#include "layout/quote/Quote.h"
#include "layout/quote/QuoteLine.h"

#include "world/grid/Index.h"
#include "world/grid/Grid.h"
#include "world/Oscillator.h"

#include "world/theme/field/DiverField.h"
#include "world/theme/field/QuoteField.h"


using namespace std;
using namespace ci;

class Theme{
    LayoutArea mArea;
    vector<DiverField*> mDiverFields;
  
    vector<Quote>*      mQuotes;        //  quote data
    Grid*               mGrid;
    Oscillator*         mOscillator;
    Quote*              mQuoteCurrent;  //  ref to curr qzote
    vector<QuoteField*> mQuoteFields;   //  quote fields
    
    gl::GlslProg mShaderDiverFields;
    gl::GlslProg mShaderQuoteFields;
    gl::Material mMaterialDiverFields;
    gl::Material mMaterialQuoteFields;

    

#if defined(BOARD_LIVE_EDIT_MATERIAL_SHADER)
    FileWatcherRef mFileWatcher;
#endif
    
    void loadMaterialProperties();
    
    void deleteQuoteFields();
    void deleteDiverFields();
    
    void setQuote(Quote& quote);
    
    
public:
    Theme(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes);
    ~Theme();
    
    
    void onConfigDidChange();
    void draw(const CameraOrtho& camera, bool useMaterialShader);
    void update();
    
    inline const Quote* getCurrentQuote() const{
        return mQuoteCurrent;
    }
};

#endif
