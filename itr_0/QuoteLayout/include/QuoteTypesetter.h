//
//  QuoteTypesetter.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 12/03/14.
//
//

#ifndef QuoteLayout_QuoteTypesetter_h
#define QuoteLayout_QuoteTypesetter_h

#include "Config.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Rect.h"
#include <vector>
#include <string>
#include "Cell.h"


using namespace std;
using namespace ci;

class QuoteTypesetter {
    gl::TextureFontRef  mTexFontRef;
    vector<Cell*>*      mCells;         // ref cells
    vector<Vec3f>       mArea;
    vector<vector<int>> mCellsIndex;    // cells indices defined by area
    
    
public:
    QuoteTypesetter(vector<Cell*>* cells, const vector<Vec3f>& area) :
        mCells(cells),
        mArea(area){
        mTexFontRef = gl::TextureFont::create(Font(FONT_NAME,FONT_SIZE));
    }
    
    inline void setString(const string& str){
        
        
    }
    
    inline void debugDrawArea(){
        static int indices[] = {0,1,3,2};
        glColor3f(0, 1, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &mArea[0].x);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    inline void draw(){
        
    }
    
    inline void update(){
        
    }
    
    
    
    
    
    
    
    
};



#endif
