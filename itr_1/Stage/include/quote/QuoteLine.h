//
//  QuoteLine.h
//  grid_00
//
//  Created by Henryk Wollik on 01/04/14.
//
//

#ifndef grid_00_QuoteLine_h
#define grid_00_QuoteLine_h

#include <vector>
#include "cinder/Vector.h"
#include "quote/QuoteAlign.h"
#include "stage/grid/Index.h"

namespace next{
    using namespace std;
    using namespace ci;

    class QuoteLine {
    private:
        vector<Index> mIndices;
        vector<Vec2f> mTexcoords;
        QuoteAlign    mAlignment;

    public:
        QuoteLine(){};
        QuoteLine(const vector<Index>& indices, const vector<Vec2f>& texcoords, QuoteAlign align = QuoteAlign::CENTER) :
        mIndices(indices), mTexcoords(texcoords),mAlignment(align){}

        inline const vector<Index>& getIndices() const{
            return mIndices;
        }

        inline const vector<Vec2f>& getTexcoords() const{
            return mTexcoords;
        }

        inline QuoteAlign getAlignment(){
            return mAlignment;
        }
    };
}




#endif
