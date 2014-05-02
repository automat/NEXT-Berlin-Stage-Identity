//
//  Quote.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_Quote_h
#define grid_00_Quote_h

#include <vector>
#include <string>

#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"

#include "data/quote/QuoteLine.h"
#include "stage/grid/Grid.h"
#include "stage/grid/Cell.h"
#include "stage/grid/Index.h"

namespace next{
    using namespace std;
    using namespace ci;

    struct Quote{
        vector<QuoteLine> lines;
        gl::Texture       texture;
        string            author;
        
        Quote(){};
        Quote(const vector<QuoteLine>& lines, const gl::Texture& texture, const string& author = "") : lines(lines), texture(texture), author(author){}
        
    };
}





#endif
