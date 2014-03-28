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
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"
#include "world/grid/Grid.h"
#include "world/grid/Cell.h"
#include "world/Index.h"


using namespace std;
using namespace ci;

typedef std::shared_ptr<class Quote> QuoteRef;

class Quote {
public:
    class Line{
    private:
        vector<Index> mIndices;
        vector<Vec2f> mTexcoords;
    public:
        Line(){};
        Line(const vector<Index>& indices, const vector<Vec2f>& texcoords) :
        mIndices(indices), mTexcoords(texcoords){}
        
        inline const vector<Index>& getIndices() const{
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
    Quote(const vector<Line>& lines, const gl::Texture& texture) :
    mLines(lines), mTexture(texture){}
    
    inline static QuoteRef create(const vector<Line>& lines, const gl::Texture& texture){
        return std::make_shared<Quote>(lines, texture);
    }
    
    
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
