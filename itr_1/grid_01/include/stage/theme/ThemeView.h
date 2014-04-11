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

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"

#include "util/FileWatcher.h"
#include "layout/quote/Quote.h"
#include "layout/quote/QuoteLine.h"

#include "stage/grid/Index.h"
#include "stage/Oscillator.h"

#include "stage/theme/field/DiverField.h"
#include "stage/theme/field/QuoteField.h"

#include "stage/AbstractView.h"

using namespace std;
using namespace ci;

class ThemeView : public AbstractView {
    vector<DiverField*> mDiverFields;
  
    vector<Quote>*      mQuotes;        //  quote data
    Oscillator*         mOscillator;
    Quote*              mQuoteCurrent;  //  ref to curr quote
    vector<QuoteField*> mQuoteFields;   //  quote fields
    
    gl::GlslProg mShaderDiverFields;
    gl::GlslProg mShaderQuoteFields;
    gl::Material mMaterialDiverFields;
    gl::Material mMaterialQuoteFields;

    

#if defined(THEME_LIVE_EDIT_MATERIAL_SHADER)
    FileWatcherRef mFileWatcher;
#endif
    
    void loadMaterialProperties();
    
    void deleteQuoteFields();
    void deleteDiverFields();
    
    void setQuote(Quote& quote);
    
    
public:
    ThemeView(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes);
    ~ThemeView();
    
    void onConfigDidChange();
    void draw(const CameraOrtho& camera, bool useMaterialShader);
    void update();
    
    inline const Quote* getCurrentQuote() const{
        return mQuoteCurrent;
    }
};

#endif
