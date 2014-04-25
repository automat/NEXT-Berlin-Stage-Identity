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

#include "stage/theme/field/QuoteField.h"
#include "quote/Quote.h"

namespace next {
    using namespace std;
    class QuoteFieldManager{
        vector<Quote>* mQuotes;
        vector<QuoteField>* mQuoteFields;
        
    public:
        QuoteFieldManager();
        ~QuoteFieldManager();
    };
}

#endif
