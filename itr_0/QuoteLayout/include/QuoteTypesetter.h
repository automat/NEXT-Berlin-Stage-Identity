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
#include "LayoutArea.h"
#include "Cell.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <vector>
#include <string>

using namespace std;
using namespace ci;
using namespace boost;
using namespace boost::assign;




class QuoteTypesetter {
    /*--------------------------------------------------------------------------------------------*/
    // Quote String
    /*--------------------------------------------------------------------------------------------*/

    struct QuoteString{
        Vec3f  pos;
        string str;
        Rectf  rect;
        QuoteString(){};
        QuoteString(const string& str, const Vec3f& pos) :
            str(str),pos(pos){
                
            }
    };
    
    /*--------------------------------------------------------------------------------------------*/
    
    gl::TextureFont::Format mTexFontFormat;
    
    gl::TextureFontRef      mTexFontRef;    // font to be used
    float                   mFontSize;
    float                   mFontSizeInv;
    float                   mFontAscent;
    float                   mFontDescent;
    float                   mFontSizeScale;
    float                   mFontScale;
    Matrix44f               mFontTransMat;  // font 3d translation matrix
    bool                    mValid;         // valid layout
    
    vector<Cell*>*          mCells;         // ref cells
    vector<vector<int>>             mCellsIndex;    // cells indices defined by area
    
    LayoutArea              mArea;          // area of cells which should be used
    int                     mDirection;     // left / right alight
    
    string                  mSrcString;     // current string src
    vector<QuoteString>     mQuoteStrings;  // calculated string segments
    
    gl::Texture             mTexture;       // resulting texture
    vector<Rectf>           mCellTexcoords; // texcoords of cell for texture
    
public:
    /*--------------------------------------------------------------------------------------------*/
    // Constructor
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteTypesetter(vector<Cell*>* cells, const LayoutArea& area, int minColumnLength = 5) :
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
                        mCellsIndex += vector<int>();
                        j = -1;
                        while (++j < countColumn) {
                            mCellsIndex.back() += (index - j * GRID_NUM_XY);
                        }
                    }
                    countColumn = 0;
                } else {
                    countColumn++; //until same row, count columns
                }
            }
            
            mValid = mCellsIndex.size() != 0;
            
            //  set defaults
            
            mTexFontFormat.enableMipmapping();
            mTexFontFormat.premultiply();
            mTexFontFormat.textureWidth(2048);
            mTexFontFormat.textureHeight(2048);
            
            setFont(Font("Arial",50),1);
            mDirection = -1;
    
        
            
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Methods
    /*--------------------------------------------------------------------------------------------*/
    
    //! Set the font
    inline void setFont(const Font& font, float scale){
        mTexFontRef    = gl::TextureFont::create(font,mTexFontFormat);
        mFontSize      = mTexFontRef->getFont().getSize();
        mFontSizeInv   = 1.0f / mFontSize;
        mFontSizeScale = mFontSizInvscale;
        mFontAscent    = mTexFontRef->getAscent();
        mFontDescent   = mTexFontRef->getDescent();
        
        mFontTransMat.identity();
        mFontTransMat *= Matrix44f::createRotation(Vec3f::xAxis(), M_PI * 0.5f);
        mFontTransMat *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI * 0.5f);
       // mFontTransMat *= Matrix44f::createScale(Vec3f(mFontSizeInv,mFontSizeInv,mFontSizeInv));
        
    }
    
    //! Set text positioning direction
    inline void setDirection(int dir){
        mDirection = dir;
    }
    
    //! Set the string
    inline void setString(const string& str){
        if(!mSrcString.compare(str)){
            return;
        }
        mSrcString = str;
        mQuoteStrings.resize(0);
        
        Rectf texBounds; // bounds of resulting texture
        
        vector<string> words;
        split(words, mSrcString, is_any_of(" "));
        
        /*
        mQuoteStrings += QuoteString("Alghorithms",Vec3f(0,0,2));
        mQuoteStrings += QuoteString("are the new",Vec3f(1,0,1));
        mQuoteStrings += QuoteString("Art Directors",Vec3f(2,0,0));
        */
        
        mQuoteStrings += QuoteString("Knowing what",Vec3f(0,0,2));
        mQuoteStrings += QuoteString("your customers",Vec3f(1,0,1));
        mQuoteStrings += QuoteString("want before",Vec3f(2,0,0));
        mQuoteStrings += QuoteString("they do.",Vec3f(3,0,-1));
        
        
        
        
        
        
        
        
        
        
        
        
        
    }
    
    //! Debug draw the area and the cells used for positioning
    inline void debugDrawArea(){
        if(!mValid){
            return;
        }
        
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
        for (auto& row : mCellsIndex) {
            for(auto& column : row){
                glVertexPointer(3, GL_FLOAT, 0, &(*mCells)[column]->getCenter().x);
                glDrawArrays(GL_POINTS, 0, 1);
                
            }
        }
        glPointSize(1);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
    }
    
    inline void debugDrawString(){
        if(!mValid || mQuoteStrings.empty()){
            return;
        }
        
        static const Vec2f zero;
        
        for (auto& str : mQuoteStrings) {
            glPushMatrix();
            glTranslatef(str.pos.x,str.pos.y,str.pos.z);
            glMultMatrixf(&mFontTransMat[0]);
            glScalef(-mFontSizeInv,mFontSizeInv,mFontSizeInv);
            glScalef(mFontSizeScale, mFontSizeScale, mFontSizeScale);
            
            mTexFontRef->drawString(str.str, zero);
            
            glPopMatrix();
        }
    }
    
    //! Draw the string
    inline void draw(){
        
    }
    
    
    
    inline void update(){
        
    }
    
    
    
    
    
    
    
    
};



#endif
