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

namespace next {
    using namespace std;
    class QuoteFieldManager{
        
        /*--------------------------------------------------------------------------------------------*/
        //  QuoteFieldManager
        /*--------------------------------------------------------------------------------------------*/
        
        int mIndex;
        
        Grid*                mGrid;
        vector<Quote>*       mQuotes;
        size_t               mNumQuotes;
        int                  mIndexQuotes;
        
        Quote*               mQuotesSelected[2];
        vector<QuoteField*>* mQuoteFields[2];
        bool                 mQuoteFieldsActiveStates[2];
        vector<Offset>       mOffsets[2];
        size_t               mNumQuoteFields[2];
        int                  mIndexQuoteFields[2];
        
        
        
        void onQuoteAtTarget(int index);
        void swap();
        void setQuote(int index);
        
    public:
        QuoteFieldManager(vector<Quote>* quotes, vector<QuoteField*>* quoteFields, Grid* grid);
        ~QuoteFieldManager();
#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
        void debugDraw();
#endif
        void update();
        void draw();
        
        //! return the current active quote
        
        const Quote* getSelectedQuote(int index);
    };
}

#endif
