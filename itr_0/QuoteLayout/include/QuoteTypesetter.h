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
    
    gl::TextureFontRef      mTexFontRef;        // font to be used
    float                   mFontSize;
    float                   mFontSizeInv;
    float                   mFontAscent;
    float                   mFontDescent;
    float                   mFontSizeScale;
    float                   mFontScale;
    float                   mFontBaseline;
    float                   mFontAscentline;
    float                   mFontDescentline;
    Matrix44f               mFontTransMat;      // font 3d translation matrix
    
    int                     mCellPadding[4];    // padding tblr
    int                     mAlign;             // left / right alight
    
    bool                    mValid;             // valid layout
    
    vector<Cell*>*          mCells;             // ref cells
    vector<vector<int>>     mCellsIndex;        // cells indices defined by area
    
    LayoutArea              mArea;              // area of cells which should be used
    
    
    string                  mSrcString;         // current string src
    vector<QuoteString>     mQuoteStrings;      // calculated string segments
    
    gl::Texture             mTexture;           // resulting texture
    vector<Rectf>           mCellTexcoords;     // texcoords of cell for texture
    
    inline void getPos(int row, int column, Vec3f* pos){
        *pos = (*mCells)[mCellsIndex[row][column]]->getCenter();
    }
    
    inline float measureText(const string& str){
        return mTexFontRef->measureString(str).x * mFontSizeInv * mFontScale;
    }
    
    inline void drawStringBoundingBox(const string& str){
        float width = measureText(str);
        float vertices[12] = {
            -0.5f, 0, 0,
            -0.5f, 0, -width,
             0.5f, 0, -width,
             0.5f, 0, 0
        };
        
        glColor3f(1,0.5,0.5);
        glLineWidth(2);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(10, 0xAAAA);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glDisable(GL_LINE_STIPPLE);
        glDisableClientState(GL_VERTEX_ARRAY);
        glLineWidth(1);
        
        
    }
    
public:
    /*--------------------------------------------------------------------------------------------*/
    // QuoteTypesetter
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteTypesetter(vector<Cell*>* cells, const LayoutArea& area, int minColumnLength = 4) :
        mCells(cells),
        mArea(area){
            //
            //  Check if cell is within area, if so push to indices to use
            //
            vector<int> tempCellsIndex;
            for(auto* cell : *mCells){
                if(mArea.contains(cell->getArea())){
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
            
            // Init format
            
            mTexFontFormat.enableMipmapping();
            mTexFontFormat.premultiply();
            mTexFontFormat.textureWidth(2048);
            mTexFontFormat.textureHeight(2048);
            
            //
            
            setCellPadding(0, 0, 0, 0);
            setAlign(1);
            setFont(Font("Arial",50),1);
            
        
            
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Methods
    /*--------------------------------------------------------------------------------------------*/
    
    //! Set text align
    inline void setAlign(int direction){
        mAlign = direction;
    }
    
    //! Set the cell padding per unit
    inline void setCellPadding(int top, int right, int bottom, int left){
        mCellPadding[0] = top;
        mCellPadding[1] = right;
        mCellPadding[2] = bottom;
        mCellPadding[3] = left;
    }
    
    //! Set the font
    inline void setFont(const Font& font, float scale){
        mTexFontRef      = gl::TextureFont::create(font,mTexFontFormat);
        mFontSize        = mTexFontRef->getFont().getSize();
        mFontSizeInv     = 1.0f / mFontSize;
        mFontSizeScale   = mFontSizeInv;
        mFontScale       = scale;
        mFontAscent      = mTexFontRef->getAscent()  / mFontSize * scale;
        mFontDescent     = mTexFontRef->getDescent() / mFontSize * scale;
        
        mFontDescentline = 0.5f * scale;
        mFontBaseline    = mFontDescentline - mFontDescent;
        mFontAscentline  = mFontDescentline - mFontAscent;
        
        const static float pi_2 = float(M_PI) * 0.5f;
        
        mFontTransMat.identity();
        mFontTransMat *= Matrix44f::createTranslation(Vec3f(0,0,0));
        mFontTransMat *= Matrix44f::createRotation(Vec3f::xAxis(), pi_2);
        mFontTransMat *= Matrix44f::createRotation(Vec3f::yAxis(), pi_2);
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
        Vec3f pos;
        
        getPos(0, 1, &pos);
        mQuoteStrings += QuoteString("Knowing",pos);
        getPos(1, 1, &pos);
        mQuoteStrings += QuoteString("what your",pos);
        getPos(2, 1, &pos);
        mQuoteStrings += QuoteString("customers",pos);
        getPos(3, 1, &pos);
        mQuoteStrings += QuoteString("want before",pos);
        getPos(4, 1, &pos);
        mQuoteStrings += QuoteString("they do.",pos);
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
        
        glPointSize(10);
        for (auto& row : mCellsIndex) {
            for(auto& column : row){
                glVertexPointer(3, GL_FLOAT, 0, &(*mCells)[column]->getCenter().x);
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }
        glPointSize(1);
        
        glLineWidth(1);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(5, 0xAAAA);

        Vec3f p0,p1;
        int i,j;
        i = -1;
        while (++i < mCellsIndex.size()) {
            const vector<int>& column = mCellsIndex[i];
            j = -1;
            while (++j < column.size() - 1) {
                const Vec3f& c0 = (*mCells)[column[j  ]]->getCenter();
                const Vec3f& c1 = (*mCells)[column[j+1]]->getCenter();
                
                // Draw ascentline
                p0    = c0;
                p1    = c1;
                p0.x += mFontAscentline;
                p1.x += mFontAscentline;
                glColor3f(1, 0, 0);
                gl::drawLine(p0,p1);
                
                // Draw descentline
                p0 = c0;
                p1 = c1;
                p0.x += mFontDescentline;
                p1.x += mFontDescentline;
                glColor3f(0,0,1);
                gl::drawLine(p0,p1);
                
                // Draw baseline
                p0 = c0;
                p1 = c1;
                p0.x += mFontBaseline;
                p1.x += mFontBaseline;
                glColor3f(1,0,1);
                gl::drawLine(p0,p1);
            }
        }
        
        glDisable(GL_LINE_STIPPLE);
        glLineWidth(1);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    inline void debugDrawString(){
        if(!mValid || mQuoteStrings.empty()){
            return;
        }
        
        static const Vec2f zero;
        
        for (auto& str : mQuoteStrings) {
            glPushMatrix();
            glTranslatef(str.pos.x,str.pos.y,str.pos.z + 0.5f);
            drawStringBoundingBox(str.str);
            glPopMatrix();
            glPushMatrix();
            glTranslatef(str.pos.x + mFontBaseline,str.pos.y,str.pos.z + 0.5f);
            glMultMatrixf(&mFontTransMat[0]);
            glScalef(-mFontSizeInv * mFontScale, mFontSizeInv * mFontScale, mFontSizeInv * mFontScale);
            glColor3f(1, 1, 1);
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
