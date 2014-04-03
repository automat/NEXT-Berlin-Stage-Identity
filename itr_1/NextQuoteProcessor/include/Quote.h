//
//  Quote.h
//  QuoteLayout_00
//
//  Created by Henryk Wollik on 24/03/14.
//
//

#ifndef QuoteLayout_00_Quote_h
#define QuoteLayout_00_Quote_h

#include <vector>
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "Cell.h"


using namespace std;
using namespace ci;

class Quote;
typedef std::shared_ptr<Quote> QuoteRef;

class Quote {
public:
    class Line{
    private:
        vector<Cell::Index> mIndices;
        vector<Vec2f> mTexcoords;
    public:
        Line(){};
        Line(const vector<Cell::Index>& indices, const vector<Vec2f>& texcoords) :
            mIndices(indices), mTexcoords(texcoords){}
        
        inline const vector<Cell::Index>& getIndices() const{
            return mIndices;
        }
        
        inline const vector<Vec2f>& getTexcoords() const{
            return mTexcoords;
        }
    };
    
private:
    vector<Line> mLines;    // quote lines spanning across cells
    gl::Texture  mTexture;  // src texture
    
public:
    Quote(){}
    Quote(const vector<Line>& lines, const gl::Texture& texture) :
        mLines(lines), mTexture(texture){}

    inline const vector<Line>& getLines() const{
        return mLines;
    }
    
    inline const gl::Texture& getTexture() const{
        return mTexture;
    }
    
    inline bool isEmpty(){
        return mLines.empty();
    }
};



#endif
