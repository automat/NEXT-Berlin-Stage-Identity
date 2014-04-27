//
//  QuoteManager.h
//  Stage
//
//  Created by Henryk Wollik on 25/04/14.
//
//

#ifndef Stage_QuoteManager_h
#define Stage_QuoteManager_h

#include <vector>

#include "stage/grid/Grid.h"
#include "stage/theme/field/QuoteField.h"
#include "stage/theme/Offset.h"
#include "quote/Quote.h"
#include "stage/Oscillator.h"

namespace next {
    using namespace std;
    class QuoteFieldManager{
        
        /*--------------------------------------------------------------------------------------------*/
        //  QuoteFieldManager
        /*--------------------------------------------------------------------------------------------*/
        
        Grid*                 mGrid;
        vector<Quote>*        mQuotes;
        size_t                mNumQuotes;
        int                   mIndexQuotes;
        
        Quote*                       mQuoteSelected;
        vector<vector<QuoteField*>>  mQuoteFields;
        vector<QuoteField*>*         mQuoteFieldsSelected;
        vector<vector<Offset>>       mOffsets;
        vector<Offset>*              mOffsetsSelected;
        
        size_t                       mNumQuoteFields;
        int                          mIndexQuoteFields;
        
        bool                 mReset;
        bool                 mDraw;
        int                  mFramesSkipped;
        
        
        void onQuoteAtTarget();
        void onQuoteAtEnd();
        void onTriggerNext(int index);
        void setQuote();
        
    public:
        QuoteFieldManager(vector<Quote>* quotes, Grid* grid);
        ~QuoteFieldManager();
#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
        void debugDraw();
#endif
        void update(Oscillator* osc, float t);
        void draw();
        
        //! return the current active quote
        
        const gl::Texture& getTexture();
    };
}

#endif
