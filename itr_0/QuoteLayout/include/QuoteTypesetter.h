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

#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include <vector>
#include <string>

using namespace std;
using namespace ci;
using namespace boost;
using namespace boost::assign;




class QuoteTypesetter {
public:
    enum Align {
        LEFT   = 0,
        CENTER = 1,
        RIGHT  = 2
    };
    
private:
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
    
    float                   mStringWidthMax;
    
    int                     mColLengthMin;
    
    bool                    mFontAutoScale;
    bool                    mManualBr;
    
    
    Matrix44f               mFontTransMat;      // font 3d translation matrix
    
    int                     mCellPadding[4];    // padding tblr
    Align                   mAlign;             // left / right / center
    
    bool                    mValid;             // valid layout
    
    vector<Cell*>*          mCells;             // ref cells
    vector<vector<int>>     mCellsIndex;        // cells indices defined by area
    
    LayoutArea              mArea;              // area of cells which should be used
 
    string                  mSrcString;         // current string src
    vector<QuoteString>     mQuoteStrings;      // calculated string segments
    
    gl::Texture             mTexture;           // resulting texture
    LayoutArea              mTextureArea;
    
    vector<Rectf>           mCellTexcoords;     // texcoords of cell for texture

    
    
    /*--------------------------------------------------------------------------------------------*/
    
    //! Get position of cell from to row + column
    inline Vec3f getPos(int row, int column){
        return (*mCells)[mCellsIndex[row][column]]->getCenter();
    }
    
    //! Get width of string
    inline float measureString(const string& str){
        return mTexFontRef->measureString(str).x * mFontSizeInv * mFontScale;
    }
    
    //! Get string offset on column according to alignment
    inline void getStringOffset(const string& str, float strWidth, float colWidth, Vec3f* offset){
        switch (mAlign) {
            case Align::RIGHT:
                offset->z = -colWidth + strWidth;
                break;
            case Align::CENTER:
                offset->z = -(colWidth - strWidth)*0.5f;
                break;
            default:
                offset->z = 0.0f;
                break;
        }
    }
    
    //! Remove char from string
    inline void eraseChar(string& str, const char& c){
        str.erase(remove(str.begin(), str.end(), c), str.end());
    }
    
    //! Draw bounding box of string
    inline void drawStringBoundingBox(const string& str){
        float width = measureString(str);
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
    
    //! Get indices of cells within the cell area
    inline void getCellIndices(){
        mCellsIndex.resize(0);
        const vector<Cell*>& cells = (*mCells);
        
        vector<int> cellsIndex;
        for(auto* cell : *mCells){
            if(mArea.contains(cell->getArea())){
                cellsIndex += cell->getId()[0] * GRID_NUM_XY + cell->getId()[1];
            }
        }
        
        int index,index1;                   // index cell, next
        
        int rowIndexMin = mCellPadding[0];  // row padding top
        int rowIndexMax;                    // row padding bottom
        int rowIndex, rowIndex1;            // index row, next
        int rowIndexValid = 0;              // index row valid with atleast 1 column
        
        int colCount = 0;
        int colCountMin = mCellPadding[3];
        int colCountMax;
        int colPaddingR = mCellPadding[1];
        
        
        int size = cellsIndex.size();
        int i    = -1;
        
        int j;

        while (++i < size) {
            index     = cellsIndex[i];
            index1    = cellsIndex[(i+1)%size];    // 0 if end
            rowIndex  = cells[index ]->getId()[0];
            rowIndex1 = cells[index1]->getId()[0];
            
            if(rowIndex1 != rowIndex){ // new row
                colCount += 1;
                // count reaches threshold
                if( colCount >= mColLengthMin){
                    // within row min padding range ?
                    if(rowIndexValid >= rowIndexMin){
                        mCellsIndex += vector<int>();
                        // widthin col padding range
                        colCountMax = colCount - colPaddingR;
                        j           = colCountMin - 1;
                        while (++j < colCountMax) {
                            mCellsIndex.back() += (index - j * GRID_NUM_XY);
                        }
                    }
                    rowIndexValid++;
                }
                colCount = 0;
            } else {
                colCount++; //until same row, count columns
            }
        }
        
        // remove rows from back according to padding
        // TODO: Dont do it after validating all cells
        rowIndexMax = mCellsIndex.size() - mCellPadding[2];
        while(mCellsIndex.size() > rowIndexMax){
            mCellsIndex.pop_back();
        }
        
        // no cells available...
        mValid = mCellsIndex.size() != 0;
    }
    
public:
    /*--------------------------------------------------------------------------------------------*/
    // QuoteTypesetter
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteTypesetter(vector<Cell*>* cells, const LayoutArea& area, int columnLengthMin = 4) :
        mCells(cells),
        mArea(area),
        mColLengthMin(columnLengthMin),
        mManualBr(false){
            // Init props
            mTexFontFormat.enableMipmapping();
            mTexFontFormat.premultiply();
            mTexFontFormat.textureWidth(2048);
            mTexFontFormat.textureHeight(2048);
            
            // Init defaults
            setPadding(0, 0, 0, 0);
            setAlign(Align::LEFT);
            setFont(Font("Arial",50),1);
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Methods
    /*--------------------------------------------------------------------------------------------*/
    
    //! Enable/Disable manual linebreaks
    inline void enableManualLineBreak(bool b){
        mManualBr = b;
    }
    //! Set text align vertical
    inline void setAlign(Align horizontal){
        mAlign = horizontal;
    }
    
    //! Set the cell padding per unit
    inline void setPadding(int top, int right, int bottom, int left){
        mCellPadding[0] = top;
        mCellPadding[1] = right;
        mCellPadding[2] = bottom;
        mCellPadding[3] = left;
        
        // get cell indices
        getCellIndices();
  
        // get maximum cells available
        mStringWidthMax = 0;
        int size = mCellsIndex.size();
        int i    = -1;
        while(++i < size){
            mStringWidthMax += mCellsIndex[i].size();
        }
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
        
        float pi_2 = float(M_PI) * 0.5f;
        mFontTransMat.identity();
        mFontTransMat *= Matrix44f::createTranslation(Vec3f(0,0,0));
        mFontTransMat *= Matrix44f::createRotation(Vec3f::xAxis(), pi_2);
        mFontTransMat *= Matrix44f::createRotation(Vec3f::yAxis(), pi_2);
     }
    
    //! Set the string
    inline bool setString(const string& str){
        if(!mValid || !mSrcString.compare(str) ){
            return false;
        }
        mSrcString = str;
        mQuoteStrings.resize(0);
        
        if (str.size() == 0) {
            return true;
        }
        
        char br('\n');
        
        string input(str);
        
        int  numBr = count(input.begin(), input.end(), br);
        bool hasBr = numBr != 0;
        
        if(hasBr){
            if(!mManualBr){
                eraseChar(input, br);
            } else {
                // front
                while (input.front() == br) {
                    input.erase(input.begin());
                }
                
                // back
                while (input.back() == br) {
                    input.pop_back();
                }
                /*
                // inbetween
                for(string::iterator itr = input.begin(); itr != input.end() - 1; ++itr){
                    if (*itr == br) {   // current is break
                        string::iterator _itr(itr);
                        if(*_itr != ' '){
                            while(*(++_itr) == br){
                                input.erase(_itr);
                            }
                            _itr = itr + 1;
                            if(*_itr != ' '){
                                input.insert(_itr, ' ');
                            }
                        }
                    }
                }
                */
            }
        }
        
        float stringWidth = measureString(input);
        
        // Check if string size exceeds maximum available space
        if(stringWidth > mStringWidthMax){
            return false;
        }
        
        // Check if string allready fits first column
        if(!hasBr && stringWidth <= mCellsIndex[0].size()){
            mQuoteStrings += QuoteString(str,getPos(0,0));
            return true;
        }
        
        deque<string> words;
        split(words, input, is_any_of(" "));
        
        int   rowMax = mCellsIndex.size();
        int   row = 0;
        float colWidth;
        
        string token;
        int    tokenNumBr;
        bool   tokenHasBr;
        int    tokenCountBr = 0;
        
        string line;
        float  lineWidth;
        
        string space;
        
        Vec3f offset;
        vector<QuoteString> lines;
        
        while (words.size() > 0) {
            token     = space + words.front();
            colWidth  = float(mCellsIndex[row].size());
            
            if(hasBr && tokenCountBr <= numBr){
                tokenNumBr    = count(token.begin(),token.end(),br);
                tokenHasBr    = tokenNumBr != 0;
                if(tokenHasBr){
                    eraseChar(token, br);
                }
                tokenCountBr += tokenNumBr;
            }
            
            lineWidth = measureString(line + token);
            
            if(tokenHasBr){
                if(lineWidth < colWidth){
                    line  += token;
                    getStringOffset(line, lineWidth, colWidth, &offset);
                    
                    lines += QuoteString(line, getPos(row,0) + offset);
                    line.clear();
                    space.clear();
                    words.pop_front();
                    row++;
                    
                } else { // manually breaked string exceeds row, sorry
                    // do auto resizing here
                    return false;
                }
            } else {
                if(lineWidth< colWidth){
                    line += token;
                    getStringOffset(line, lineWidth, colWidth, &offset);
                    
                    space = " ";
                    words.pop_front();
                
                } else {
                    lines += QuoteString(line,getPos(row,0) + offset);
                    line.clear();
                    space.clear();
                    row++;
                }
            }
            
            // line breaked string exceeds rows, sorry
            if(row >= rowMax){
                // do auto resizing here
                return false;
            }
            
            if(words.size() == 0){
                lines += QuoteString(line,getPos(row,0) + offset);
            }
         }
        
        mQuoteStrings = lines;
        return true;
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Debug Draw
    /*--------------------------------------------------------------------------------------------*/
    
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
    
};



#endif
