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
#include "cinder/Font.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "QuoteLayoutArea.h"
#include "Cell.h"

#include <vector>
#include <string>

using namespace std;
using namespace ci;
using namespace boost::assign;




class QuoteTypesetter {
    /*--------------------------------------------------------------------------------------------*/
    // Quote String
    /*--------------------------------------------------------------------------------------------*/
    struct QuoteString{
        Vec3f  pos;
        string string;
    };
    
    /*--------------------------------------------------------------------------------------------*/
    
    gl::TextureFontRef  mTexFontRef;    // font to be used
    bool                mValid;         // valid layout
    
    vector<Cell*>*      mCells;         // ref cells
    vector<int>         mCellsIndex;    // cells indices defined by area
    
    QuoteLayoutArea     mArea;          // area of cells which should be used
    int                 mDirection;     // left / right alight
    
    string              mSrcString;     // current string src
    vector<QuoteString> mQuoteStrings;  // calculated string segments
    
    gl::Texture         mTexture;       // resulting texture
    vector<Rectf>       mCellTexcoords; // texcoords of cell for texture
    
public:
    /*--------------------------------------------------------------------------------------------*/
    // Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteTypesetter(vector<Cell*>* cells, const QuoteLayoutArea& area, int minColumnLength = 5) :
        mCells(cells),
        mArea(area){
            //
            //  Check if cell is within area, if so push to indices to use
            //
            vector<int> tempCellsIndex;
            for(auto* cell : *mCells){
                if(mArea.contains(cell->getCenter())){
                    tempCellsIndex += cell->getId()[0] * GRID_NUM_XY + cell->getId()[1];
                }
            }
            
            int countColumn = 0;
            int currRow,currColumn,nextRow;
            int i,j;
            int size = tempCellsIndex.size();
            int index,indexNext;
            
            i = -1;
            while (++i < size) {
                index     = tempCellsIndex[i];          // get current index
                indexNext = tempCellsIndex[(i+1)%size]; // get next index, 0 if end
                
                currColumn = (*mCells)[index    ]->getId()[1];
                currRow    = (*mCells)[index    ]->getId()[0];
                nextRow    = (*mCells)[indexNext]->getId()[0];
                
                if(nextRow != currRow){ // new row
                    countColumn += 1;
                    if( countColumn >= minColumnLength){ // count reaches threshold
                        j = -1;
                        while (++j < countColumn) {
                            mCellsIndex += (index - j * GRID_NUM_XY);
                        }
                    }
                    countColumn = 0;
                } else {
                    countColumn++; //until same row, count columns
                }
            }
            
            mValid = mCellsIndex.size() != 0;
        
            //  set defaults
            
            setFont(Font("Arial",10));
            mDirection = -1;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Methods
    /*--------------------------------------------------------------------------------------------*/
    
    //! Set the font
    inline void setFont(const Font& font){
        mTexFontRef = gl::TextureFont::create(font);
    }
    
    //! Set text positioning direction
    inline void setDirection(int dir){
        mDirection = dir;
    }
    
    //! Set the string
    inline void setString(const string& str){
        if(mSrcString.compare(str)){
            return;
        }
        mSrcString = str;
        mQuoteStrings.resize(0);
        
        Rectf texBounds; // bounds of resulting texture
        
        vector<string> words;
        
        
        
        
        
        
        
        
        
        
        
        
    }
    
    //! Debug draw the area and the cells used for positioning
    inline void debugDrawArea(){
        if(!mValid)return;
        
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
    
    //! Draw the string
    inline void draw(){
        
    }
    
    
    
    inline void update(){
        
    }
    
    
    
    
    
    
    
    
};



#endif
