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
#include "cinder/gl/Fbo.h"
#include "LayoutArea.h"

#include "Grid.h"
#include "Cell.h"

#include "Utils.h"
#include <algorithm>
#include <boost/algorithm/string/trim.hpp>
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

    struct QuoteLine{
        Vec3f               pos;
        string              str;
        vector<Cell::Index> indices;
        QuoteLine(){};
        QuoteLine(const string& str, const Vec3f& pos, const vector<Cell::Index> indices) :
            str(str),pos(pos),indices(indices){}
    };
    
    /*--------------------------------------------------------------------------------------------*/
    
    // font
    
    gl::TextureFont::Format mTexFontFormat;
    gl::TextureFontRef      mTexFontRef;        // font to be used
    float                   mFontScale;
    float                   mFontBaseline;
    float                   mFontAscentline;
    float                   mFontDescentline;
    Matrix44f               mFontTransMat;      // font 3d translation matrix
    
    Align                   mAlign;             // left / right / center
    bool                    mManualBr;
    
    // line length
    
    float                   mStringWidthMax;
    int                     mColLengthMin;
    
    // grid / area
    
    LayoutArea              mArea;              // area of cells which should be used
    Grid*                   mGrid;
    vector<Cell*>*          mCells;
    int                     mCellPadding[4];    // padding tblr
    vector<vector<int>>     mCellsIndex;        // cells indices defined by area
    
    // string / computed lines
    
    bool                    mConstrain;         // string doesnt fit?, draw anyway
    vector<QuoteLine>       mQuoteLines;        // calculated string segments
    
    // texture
    
    gl::Fbo                 mFbo;
    
    gl::Texture             mTexture;           // resulting texture
    LayoutArea              mTextureArea;
    vector<Rectf>           mCellTexcoords;     // texcoords of cell for texture

    // general
    
    bool                    mValid;             // valid layout

    
    /*--------------------------------------------------------------------------------------------*/
    
    
    //! Get width of string
    inline float measureString(const string& str){
        return mTexFontRef->measureString(str).x * mFontScale;
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
        
        const vector<Cell*>& cells = mGrid->getCells();
        vector<int> cellsIndex;
        
        for(auto* cell : cells){
            if(mArea.contains(cell->getArea())){
                cellsIndex += cell->getIndex()[0] * GRID_NUM_XY + cell->getIndex()[1];
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
        
        int numCellsY = mGrid->getNumCellsY();
        
        int size = cellsIndex.size();
        int i    = -1;
        
        int j;

        while (++i < size) {
            index     = cellsIndex[i];
            index1    = cellsIndex[(i+1)%size];    // 0 if end
            rowIndex  = cells[index ]->getIndex()[0];
            rowIndex1 = cells[index1]->getIndex()[0];
            
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
                            mCellsIndex.back() += (index - j * numCellsY);
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
    
    inline void renderToTexture(){
        
    }
    
public:
    /*--------------------------------------------------------------------------------------------*/
    // QuoteTypesetter
    /*--------------------------------------------------------------------------------------------*/
    
    QuoteTypesetter(Grid* grid, const LayoutArea& area, int columnLengthMin = 4) :
        //mCells(cells),
        mGrid(grid),
        mArea(area),
        mColLengthMin(columnLengthMin),
        mManualBr(false),
        mConstrain(true){
            // Init props
            mTexFontFormat.enableMipmapping();
            mTexFontFormat.premultiply();
            mTexFontFormat.textureWidth(2048);
            mTexFontFormat.textureHeight(2048);
            
            // Init defaults
            setPadding(0, 0, 0, 0);
            setAlign(Align::LEFT);
            setFont("Arial");
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Methods
    /*--------------------------------------------------------------------------------------------*/
    
    //! Enable/Disable manual linebreaks
    inline void manualLineBreak(bool b){
        mManualBr = b;
    }
    
    //! Set text align vertical
    inline void setAlign(Align align){
        mAlign = align;
    }
    
    //! Set the cell padding per unit
    inline void setPadding(int top, int right, int bottom, int left){
        mCellPadding[0] = MAX(0, MIN(top,    mColLengthMin));
        mCellPadding[1] = MAX(0, MIN(right,  mColLengthMin));
        mCellPadding[2] = MAX(0, MIN(bottom, mColLengthMin));
        mCellPadding[3] = MAX(0, MIN(left,   mColLengthMin));
        
        if(mCellPadding[1] + mCellPadding[3] > mColLengthMin){
            return;
        }
        
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
    
    //! Set font scale
    inline void setFontScale(float scale){
        float fontSize = mTexFontRef->getFont().getSize();
        mFontScale     = 1.0f / fontSize * scale;
        
        float ascent  = mTexFontRef->getAscent()  / fontSize * scale;
        float descent = mTexFontRef->getDescent() / fontSize * scale;
        
        mFontDescentline = 0.5f * scale;
        mFontBaseline    = mFontDescentline - descent;
        mFontAscentline  = mFontDescentline - ascent;
    }
    
    //! Set the font
    inline void setFont(const string& fontName, float fontSize = 50.0f, float fontScale = 1.0f){
        mTexFontRef = gl::TextureFont::create(Font(fontName,fontSize),mTexFontFormat);
        setFontScale(fontScale);
        
        float pi_2 = float(M_PI) * 0.5f;
        //mFontTransMat.setToIdentity(); // hm
        mFontTransMat *= Matrix44f::createTranslation(Vec3f(0,0,0));
        mFontTransMat *= Matrix44f::createRotation(Vec3f::xAxis(), pi_2);
        mFontTransMat *= Matrix44f::createRotation(Vec3f::yAxis(), pi_2);
     }
    
    //! Clear
    inline void clear(){
        mQuoteLines.clear();
    }
    
    inline bool isValid(){
        return mValid;
    }
    
    //! Dont render text that exceeds the available cells
    inline void constrain(bool b = true){
        mConstrain = b;
    }
    
private:
    inline void addQuoteLine(vector<QuoteLine>& target, const string& line, int row){
        if(line.empty()){
            return;
        }
    
        const vector<int>&   columns = mCellsIndex[row];
        const vector<Cell*>& cells   = mGrid->getCells();
    
        
        float lineWidth = measureString(line);
        float colWidth  = float(columns.size());
        
        float offset    = (mAlign == Align::RIGHT  ? (-(colWidth-lineWidth)) :
                           mAlign == Align::CENTER ? (-(colWidth-lineWidth) * 0.5f) :
                           0.0f) + 0.5f;
        
        vector<Cell::Index> indices;
        int colIndexBegin = int(round(abs(offset)));
        int colIndexEnd   = int(round(colWidth - 0.5f)) - colIndexBegin;
        
        while(colIndexBegin < colIndexEnd){
            indices += cells[columns[colIndexBegin++]]->getIndex();
        }
        
        Vec3f posBegin = cells[columns[0]]->getCenter();
        Vec3f pos = posBegin + Vec3f(0,0,offset);
        target+= QuoteLine(line, pos, indices);
    }
public:
    
    //! Set the string
    inline bool setString(const string& str){
        if(!mValid){
            return false;
        }
        
        mQuoteLines.resize(0);
        
        if (str.size() == 0) {
            return true;
        }
        
        string input(str);
        trim(input);

        const char br('\n');
        
        int  numBr = count(input.begin(), input.end(), br);
        bool hasBr = numBr != 0;
        
        if(hasBr){
            if(!mManualBr){
                utils::removeChar(input, br);
            } else {
                // front / back
                while (input.front() == br) {
                    input.erase(input.begin());
                }
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
        
        Vec3f linePos;
        Vec3f lineOffset;
        float lineWidth = measureString(input);
        
        // Check if string size exceeds maximum available space
        if(mConstrain && (lineWidth > mStringWidthMax)){
            return false;
        }
        
        int    rowMax   = mCellsIndex.size();
        int    row      = 0;
        float  colWidth = float(mCellsIndex[row].size());
        
        // Check if string allready fits first column
        if(!hasBr && lineWidth <= colWidth){
            addQuoteLine(mQuoteLines, input, row);
            return true;
        }
        
        deque<string> words;
        split(words, input, is_any_of(" "));
        
        string token;
        int    tokenNumBr;
        bool   tokenHasBr;
        int    tokenCountBr = 0;
        
        string line;
        string space;
        
        vector<QuoteLine> lines;
        
        while (words.size() > 0) {
            token     = space + words.front();
            colWidth  = float(mCellsIndex[row].size());
            
            if(hasBr && tokenCountBr <= numBr){
                tokenNumBr = count(token.begin(),token.end(),br);
                tokenHasBr = tokenNumBr != 0;
                if(tokenHasBr){
                    utils::removeChar(token, br);
                }
                tokenCountBr += tokenNumBr;
            }
            
            lineWidth = measureString(line + token);
            /*
            if(tokenHasBr){
                if(lineWidth < colWidth){
                    line      += token;
                    addQuoteLine(lines, line, row)
       
                    line.clear();
                    space.clear();
                    words.pop_front();
                    row++;
                    
                } else { // manually breaked string exceeds row, sorry
                    // do auto resizing here
                    if(!mConstrain){
                        mQuoteLines = lines;
                        renderToTexture();
                    }
                    return false;
                }
            } else {
             */
                
                if(lineWidth< colWidth){
                    line      += token;
                    space = " ";
                    words.pop_front();
                
                } else {
                    addQuoteLine(lines, line, row);
                    
                    line.clear();
                    space.clear();
                    row++;
                }
            /*}*/
            
            // line breaked string exceeds rows, sorry
            if(row >= rowMax){
                // do auto resizing here
                if(!mConstrain){
                    mQuoteLines = lines;
                    renderToTexture();
                }
                return false;
            }
            
            if(words.size() == 0){
                addQuoteLine(lines, line, row);
            }
         }
        
        mQuoteLines = lines;
        
        renderToTexture();
        return true;
    }
    
    inline gl::Texture getTexture(){
        return mTexture;
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
        
        const vector<Cell*>& cells = mGrid->getCells();
        
        glPointSize(10);
        for (auto& row : mCellsIndex) {
            for(auto& column : row){
                glVertexPointer(3, GL_FLOAT, 0, &cells[column]->getCenter().x);
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
                const Vec3f& c0 = cells[column[j  ]]->getCenter();
                const Vec3f& c1 = cells[column[j+1]]->getCenter();
                
                
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
        if(!mValid || mQuoteLines.empty()){
            return;
        }
        static const Vec2f zero;
        
        for (auto& line : mQuoteLines) {
            glPushMatrix();
            //glTranslatef(line.pos.x,line.pos.y,line.pos.z + 0.5f);
            glTranslatef(line.pos.x,line.pos.y,line.pos.z);
            glColor3f(0.5,0.25,0.25);
            drawStringBoundingBox(line.str);
            glPopMatrix();
            glPushMatrix();
            //glTranslatef(line.pos.x + mFontBaseline,line.pos.y,line.pos.z + 0.5f);
            glTranslatef(line.pos.x + mFontBaseline,line.pos.y,line.pos.z);
            glMultMatrixf(&mFontTransMat[0]);
            glScalef(-mFontScale, mFontScale, mFontScale);
            glColor3f(1, 1, 1);
            mTexFontRef->drawString(line.str, zero);
            glPopMatrix();
            
            glColor3f(0,1,0);
            glLineWidth(10);
            for(auto& index : line.indices){
                mGrid->getCell(index[0], index[1])->debugDrawArea();
            }
            glLineWidth(1);
        }
    }
    
    inline void debugDrawTexture(){
        
    }
    
};



#endif
