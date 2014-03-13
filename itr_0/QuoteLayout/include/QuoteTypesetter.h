//
//  QuoteTypesetter.h
//  QuoteLayout
//
//  Created by Henryk Wollik on 12/03/14.
//
//

#ifndef QuoteLayout_QuoteTypesetter_h
#define QuoteLayout_QuoteTypesetter_h
#include <boost/assign/std/vector.hpp>

#include "Config.h"
#include "cinder/gl/TextureFont.h"
#include "QuoteLayoutArea.h"
#include "Cell.h"

#include <vector>
#include <string>

using namespace std;
using namespace ci;
using namespace boost::assign;




class QuoteTypesetter {
    gl::TextureFontRef  mTexFontRef;
    vector<Cell*>*      mCells;         // ref cells
    QuoteLayoutArea     mArea;
    vector<int>         mCellsIndex;    // cells indices defined by area
    
    
public:
    QuoteTypesetter(vector<Cell*>* cells, const QuoteLayoutArea& area) :
        mCells(cells),
        mArea(area){
        mTexFontRef = gl::TextureFont::create(Font(FONT_NAME,FONT_SIZE));
            for(auto* cell : *mCells){
                if(mArea.contains(cell->getCenter())){
                    mCellsIndex += cell->getId()[0] * GRID_NUM_XY + cell->getId()[1];
                }
            }
    }
    
    inline void setString(const string& str){
        
        
    }
    
    inline void debugDrawArea(){
        static int indices[] = {0,1,3,2};
        Vec3f vertices[] = {mArea.getTL(),
                            mArea.getTR(),
                            mArea.getBL(),
                            mArea.getBR()};
        
        glColor3f(0, 1, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
        
        glColor3f(1, 1, 1);
        glPointSize(10);
        for (auto& index : mCellsIndex) {
            glVertexPointer(3, GL_FLOAT, 0, &(*mCells)[index]->getCenter().x);
            glDrawArrays(GL_POINTS, 0, 1);
        }
        glPointSize(1);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
    }
    
    inline void draw(){
        
    }
    
    inline void update(){
        
    }
    
    
    
    
    
    
    
    
};



#endif
