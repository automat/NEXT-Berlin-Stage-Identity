//
//  QuoteJson.h
//  grid_00
//
//  Created by Henryk Wollik on 03/04/14.
//
//

#ifndef grid_00_QuoteJson_h
#define grid_00_QuoteJson_h

#include "quote/QuoteAlign.h"
#include <iostream>

namespace next{
    using namespace std;

    struct QuoteJson {
        struct Format{
            float      scale;
            int        padding[4];
            QuoteAlign align;
            bool       balance;
            Format() : scale(0.70f), align(QuoteAlign::CENTER), balance(false){
                padding[0] = padding[1] = padding[2] = padding[3] = 0;
            }
        };

        std::string str;
        Format format;
    };
}

#endif
