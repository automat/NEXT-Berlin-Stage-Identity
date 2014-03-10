//
//  StringTexture.h
//  grid_08
//
//  Created by Henryk Wollik on 10/03/14.
//
//

#ifndef grid_08_StringTexture_h
#define grid_08_StringTexture_h

#include "Config.h"
#include "cinder/gl/Texture.h"
#include "cinder/Font.h"
#include <OpenGL/OpenGL.h>
#include <boost/algorithm/string.hpp>

#include "cinder/cairo/Cairo.h"
#include "cinder/gl/Fbo.h"

#include <string>
#include <istream>

using namespace ci;
using namespace std;





class QuoteField {
    
    
private:
    Font*   mFont;
    float   mFontSize;
    ColorAf mFontColor;
    
    int     mSizeX;
    int     mSizeY;
    
    gl::Fbo mFbo;
    
    cairo::SurfaceImage mSurface;
    
    
    
       
    
    
    
public:
    QuoteField(){
        mFont      = &QUOTE_FONT;
        mFontSize  = mFont->getSize();
        mFontColor = QUOTE_FONT_COLOR;
        
        
    }
    
    inline void setSize(int sizeX, int sizeY){
        mSizeX = sizeX;
        mSizeY = sizeY;
        
        
    }
    
    inline void gen(const string& quote, vector<StringCell*>* cells){
        //just make sure the vector is empty
       // while(!cells->empty()) delete cells->back(), cells->pop_back();
        
        
        
        //break string into pieces
        vector<string> words;
        boost::split(words, quote, boost::is_any_of("\t "));
        
        
        
        
        
        
        
        
        
    }
    
    
    
};


#endif
