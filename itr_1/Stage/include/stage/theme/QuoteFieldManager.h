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
        
        Grid*                mGrid;
        vector<Quote>*       mQuotes;
        vector<QuoteField*>* mQuoteFields;
        size_t               mNumQuoteFields;
        vector<Offset>       mOffsets;
        
        int mIndexQuotes;
        Offset mOffset;
        
        void setQuote(const Quote& quote);
        
    public:
        QuoteFieldManager(vector<Quote>* quotes, vector<QuoteField*>* quoteFields, Grid* grid);
        ~QuoteFieldManager();
#ifdef DEBUG_THEME_FIELD_QUOTE_MANAGER
        void debugDraw();
#endif
        void update();
        
        //! return the current active quote
        const Quote* getCurrQuote();
    };
}

#endif
