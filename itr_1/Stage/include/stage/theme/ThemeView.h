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
#include "data/quote/Quote.h"
#include "data/quote/QuoteLine.h"

#include "stage/grid/Index.h"
#include "stage/Oscillator.h"

#include "stage/theme/field/DiverField.h"
#include "stage/theme/field/QuoteField.h"
#include "stage/theme/QuoteFieldManager.h"

#include "stage/AbstractView.h"

namespace next{
    using namespace std;
    using namespace ci;

    class ThemeView : public AbstractView {
        LayoutArea mArea;
        Grid*      mGrid;
        
        vector<DiverField*> mDiverFields;

        vector<Quote>*      mQuotes;        //  quote data
        Oscillator*         mOscillator;
        
        QuoteFieldManager*  mQuoteFieldManager;

        gl::GlslProg mShaderDiverFields;
        gl::GlslProg mShaderQuoteFields;
        gl::Material mMaterialDiverFields;
        gl::Material mMaterialQuoteFields;
        

#if defined(THEME_LIVE_EDIT_MATERIAL_SHADER)
        FileWatcherRef mFileWatcher;
#endif

        void loadMaterialProperties();

        void deleteDiverFields();

    public:
        ThemeView(Grid* grid, const LayoutArea& area, Oscillator* oscillator, vector<Quote>* quotes);
        ~ThemeView();

        void onConfigDidChange();
        void draw(const CameraOrtho& camera, bool useMaterialShader);
        void update();

        
        void debugDrawQuoteManager();
        
        void play(int num, const std::function<void()>& callback);
    };
}


#endif
