//
//  JsonLoader.h
//  grid_00
//
//  Created by Henryk Wollik on 03/04/14.
//
//

#ifndef grid_00_JsonLoader_h
#define grid_00_JsonLoader_h

#include "cinder/Json.h"
#include "layout/quote/json/QuoteJson.h"

using namespace std;
using namespace ci;

class QuoteParser {
    static bool      IsValidQuoteObj(const JsonTree& tree, string* msg);
    static QuoteJson ReadQuote(const JsonTree& tree);
    
    QuoteParser();
public:
    static bool LoadJson(const string& filepath, vector<QuoteJson>* data, string* msg);
    
};


#endif
