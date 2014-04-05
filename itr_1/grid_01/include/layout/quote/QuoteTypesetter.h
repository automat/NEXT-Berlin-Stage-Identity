//
//  QuoteTypesetter.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_QuoteTypesetter_h
#define grid_00_QuoteTypesetter_h

#include <boost/assign/std/vector.hpp>



#include <OpenGL/OpenGL.h>

#include "Resources.h"
#include "cinder/app/App.h"

#include "cinder/gl/gl.h"
#include "cinder/Font.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Utilities.h"

#include "layout/geom/LayoutArea.h"
#include "layout/quote/QuoteAlign.h"
#include "layout/quote/Quote.h"
#include "layout/quote/QuoteLine.h"

#include "world/grid/Grid.h"
#include "world/grid/Cell.h"
#include "world/Index.h"

#include "util/Utils.h"

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
private:
    
    /*--------------------------------------------------------------------------------------------*/
    // Quote String
    /*--------------------------------------------------------------------------------------------*/
    
    struct Line_Internal{
        float               offset;
        string              str;
        vector<Index> indices;
        Line_Internal(){};
        Line_Internal(const string& str, float offset, const vector<Index> indices) :
        str(str),offset(offset),indices(indices){}
        
        inline const Index& getLeft() const{
            return indices.front();
        }
    };
    
    /*--------------------------------------------------------------------------------------------*/
    
    // font
    
    gl::TextureFont::Format mTexFontFormat;
    gl::TextureFontRef      mTexFontRef;        // font to be used
    float                   mFontAscent;        // font ascent
    float                   mFontDescent;       // font descent
    float                   mFontScale;
    float                   mFontScaleNorm;
    float                   mFontBaseline;      // text draw aling baseline
    float                   mFontAscentline;    // text draw align ascent
    float                   mFontDescentline;   // text draw align descent
    Matrix44f               mFontTransMat;      // font 3d translation matrix
    
    QuoteAlign              mAlign;             // left / right / center
    bool                    mManualBr;          // flag, whether manual linbreaks should be processed
    
    // line length
    
    float                   mStringWidthMax;    // maximum length of string according to cells
    int                     mColLengthMin;      // minumum number of cells a columnt should have
    
    // grid / area
    
    LayoutArea              mArea;              // area of cells which should be used
    Grid*                   mGrid;
    int                     mCellPadding[4];    // padding tblr
    vector<vector<int>>     mCellsIndex;        // cells indices defined by area
    
    // string / computed lines
    
    bool                    mConstrain;         // string doesnt fit?, draw anyway
    bool                    mBalancedBaseline;  // if ascent is less then descent, shift baselin
    vector<Line_Internal>   mLines;             // calculated string segments
    QuoteRef                mQuote;             // current resulting quote
    
    
    // texture
    
    int                     mTextureSize;       // Size of output texture
    gl::Fbo                 mFbo;               // fbo drawn to
    bool                    mDebugTexture;      // flag, wheter texture should show cells, and type measurements
    
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
        int gridNumCellsX = mGrid->getNumCellsX();
        
        for(const auto* cell : cells){
            if(mArea.contains(cell->getArea())){
                cellsIndex += cell->getIndex()[0] * gridNumCellsX + cell->getIndex()[1];
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
    
    //! create Quote output
    inline void createQuote(){
        int numLines  = mLines.size();
        vector<int>   indices;
        vector<Vec2f> texcoords;
        
        int numCellsX = 0;
        int numCellsY = numLines;
        int numIndices;
        
        vector<QuoteLine> lines;
        
        for(const auto& line : mLines){
            numIndices = line.indices.size();
            numCellsX  = MAX(numCellsX, numIndices);
        }
        
        int   unitsMax  = MAX(numCellsX,numCellsY);
        float unitsMaxf = float(unitsMax);
        
        float fboSize   = mFbo.getWidth();
        float invScale  = fboSize / unitsMaxf;
        float stepUV    = 1.0f / unitsMaxf;
        
        static const Rectf unit(0,0,1,1);
        static const Vec2f zero;
        
        mFbo.bindFramebuffer();
        
        glPushAttrib(GL_VIEWPORT_BIT);
        gl::setViewport(mFbo.getBounds());
        gl::setMatricesWindow(mFbo.getSize(),false);
        gl::clear(ColorA(0,0,0,0));
        
        gl::disableDepthRead();
        gl::enableAlphaTest();
        gl::enableAlphaBlending();
        
        glPushMatrix();
        glScalef(invScale,invScale,invScale);
        
        //
        //  Put data
        //
        
        int i,j;
        Vec2f down(0,stepUV);
        
        i = 0;
        for(const auto& line : mLines){
            numIndices = line.indices.size();
            texcoords.resize(0);
            
            texcoords += down * float(i);                                              // upper left
            texcoords += Vec2f(float(numIndices) * stepUV, 0) + *(texcoords.end()-1);  // upper right
            texcoords += *(texcoords.end() - 2) + down;                                // lower left
            texcoords += *(texcoords.end() - 2) + down;                                // lower right
            
            lines += QuoteLine(line.indices,texcoords,mAlign);
            i++;
        }
        
        //
        //  Draw debug
        //
        if(mDebugTexture){
            static const gl::TextureFontRef debugFont = gl::TextureFont::create(Font(app::loadResource(RES_FONT_APERCU_MONO),40));
            static const Vec2f debugFontOffsetT(0,debugFont->getFont().getSize() - debugFont->getDescent());
            static const Vec2f debugFontOffsetB(0,-debugFont->getDescent());
            static const float debugFontScale(0.00075f);
            
            int numLines = mLines.size();
            Vec2f lineProperty;
            i = -1;
            while(++i < numLines){
                const Line_Internal& line = mLines[i];
                const QuoteLine& line_ = lines[i];
                
                numIndices = line.indices.size();
                
                //
                // Draw line to texture
                //
                glPushMatrix();
                glTranslatef(0,i,0);
                glColor3f(0,0,0.25f);
                glLineWidth(10);
                glPushMatrix();
                j = -1;
                while(++j < numIndices){
                    glPushMatrix();
                    glTranslatef(j, 0, 0);
                    gl::drawStrokedRect(unit);
                    glPopMatrix();
                }
                glPopMatrix();
                
                glLineWidth(1);
                glPushMatrix();
                //
                //  Draw ascent / baselin / descent
                //
                glLineWidth(5);
                static const Vec2f p0;
                Vec2f p1(float(numIndices),0);
                
                glColor3f(0,0.25f,0.25f);
                lineProperty.y = 0.5f;
                gl::drawLine(p0 + lineProperty, p1 + lineProperty);
                
                glEnable(GL_LINE_STIPPLE);
                glLineStipple(20, 0xAAAA);
                
                lineProperty.y = mFontAscentline + 0.5f;
                glColor3f(1,0,0);
                gl::drawLine(p0 + lineProperty,p1 + lineProperty);
                
                lineProperty.y = mFontDescentline + 0.5f;
                glColor3f(0,0,1);
                gl::drawLine(p0 + lineProperty,p1 + lineProperty);
                
                lineProperty.y = mFontBaseline + 0.5f;
                glColor3f(1,0,1);
                gl::drawLine(p0 + lineProperty,p1 + lineProperty);
                
                glDisable(GL_LINE_STIPPLE);
                glLineWidth(1);
                
                //
                //  Draw string
                //
                glTranslatef(line.offset,0.5f + mFontBaseline,0);
                glScalef(mFontScale, mFontScale, mFontScale);
                glColor3f(1,1,1);
                mTexFontRef->drawString(line.str, zero);
                glPopMatrix();
                glPopMatrix();
                
                //
                //  Draw texcoords
                //
                glPushMatrix();
                glTranslatef(0, i, 0);
                glColor3f(0,0.25f,0.25f);
                
                const Vec2f& tl = line_.getTexcoords()[0];
                const Vec2f& tr = line_.getTexcoords()[1];
                const Vec2f& bl = line_.getTexcoords()[2];
                const Vec2f& br = line_.getTexcoords()[3];
                string tls = toString(tl);
                string trs = toString(tr);
                string bls = toString(bl);
                string brs = toString(br);
                
                glPushMatrix();
                glScalef(unitsMaxf,unitsMaxf,unitsMaxf);
                glScalef(debugFontScale,debugFontScale,debugFontScale);
                debugFont->drawString(tls, debugFontOffsetT);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(float(numIndices) - debugFont->measureString(trs).x / invScale * 1.5f, 0, 0);
                glScalef(unitsMaxf,unitsMaxf,unitsMaxf);
                glScalef(debugFontScale,debugFontScale,debugFontScale);
                debugFont->drawString(trs, debugFontOffsetT);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(0, 1, 0);
                glScalef(unitsMaxf,unitsMaxf,unitsMaxf);
                glScalef(debugFontScale,debugFontScale,debugFontScale);
                debugFont->drawString(bls, debugFontOffsetB);
                glPopMatrix();
                
                glPushMatrix();
                glTranslatef(float(numIndices) - debugFont->measureString(brs).x / invScale * 1.5f, 1, 0);
                glScalef(unitsMaxf,unitsMaxf,unitsMaxf);
                glScalef(debugFontScale,debugFontScale,debugFontScale);
                debugFont->drawString(brs, debugFontOffsetB);
                glPopMatrix();
                
                glPopMatrix();
            }
            
            //
            //  Draw Normal
            //
        } else {
            i = -1;
            for(const auto& line : mLines){
                glPushMatrix();
                glTranslatef(line.offset, 0.5f + mFontBaseline + (++i), 0);
                glScalef(mFontScale, mFontScale, mFontScale);
                glColor3f(1,1,1);
                mTexFontRef->drawString(line.str, zero);
                glPopMatrix();
            }
        }
        
        glPopMatrix();
        
        gl::disableAlphaBlending();
        gl::disableAlphaTest();
        glPopAttrib();
        mFbo.unbindFramebuffer();
        
        mQuote = make_shared<Quote>(lines,mFbo.getTexture());
    }
    
    inline void addLine_Internal(vector<Line_Internal>& target, const string& line, int row){
        if(line.empty()){
            return;
        }
        
        const vector<int>&   columns = mCellsIndex[row];
        const vector<Cell*>& cells   = mGrid->getCells();
        
        float lineWidth = measureString(line);
        
        if(lineWidth == 0){ // in case of char doesnt have any physical size, eg. cmd
            return;
        }
        
        int   colSize   = columns.size();
        float colWidth  = float(colSize);
        
        float offsetCenter = 0.5f;
        
        float offset        = 0.0f;
        float offsetScale   = 0.0f;
        float colOffset     = 0.0f;
        int   colIndexBegin = 0; // first cell of line
        int   colIndexEnd   = 0; // last cell of line
        
        switch (mAlign) {
            case QuoteAlign::RIGHT:
                colOffset     = -(colWidth-lineWidth);
                colIndexBegin = int(round(abs(colOffset + offsetCenter)));
                colIndexEnd   = MIN(colIndexBegin + colSize,colSize);
                offsetScale   = 1;
                break;
                
            case QuoteAlign::CENTER:
                colOffset     = -(colWidth-lineWidth) * 0.5f;
                colIndexBegin = int(round(abs(colOffset + offsetCenter)));
                colIndexEnd   = colSize - colIndexBegin;
                offsetScale   = 0.5f;
                break;
                
            default:
                colIndexEnd   = int(round(lineWidth + offsetCenter));
                break;
        }
        
        vector<Index> indices;
        
        while(colIndexBegin < colIndexEnd){
            indices += cells[columns[colIndexBegin]]->getIndex();
            colIndexBegin++;
        }
        
        offset = offsetScale * (float(indices.size()) - lineWidth);
        target+= Line_Internal(line, offset, indices);
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
    mConstrain(true),
    mTextureSize(2048),
    mDebugTexture(false),
    mBalancedBaseline(false){
        //
        // Init props
        //
        mTexFontFormat.enableMipmapping();
        mTexFontFormat.premultiply();
        mTexFontFormat.textureWidth(2048);
        mTexFontFormat.textureHeight(2048);
        
        //
        // Init Fbo
        //
        gl::Fbo::Format fboFormat;
        fboFormat.setSamples(4);
        //fboFormat.setColorInternalFormat(GL_RGBA_FLOAT32_APPLE);
        fboFormat.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        
        mFbo = gl::Fbo(mTextureSize,mTextureSize, fboFormat);
        
        // Init defaults
        setPadding(0, 0, 0, 0);
        setAlign(QuoteAlign::LEFT);
        setFont("Arial");
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Methods
    /*--------------------------------------------------------------------------------------------*/
    
    //! Enable/Disable balanced baseline
    inline void balanceBaseline(bool b = true){
        mBalancedBaseline = b;
        
        if(mBalancedBaseline){
            mFontBaseline    = (abs(mFontAscent) - abs(mFontDescent)) * 0.5f;
            mFontDescentline = mFontBaseline + mFontDescent;
            mFontAscentline  = mFontDescentline - mFontAscent;
        } else {
            mFontDescentline = 0.5f * mFontScaleNorm;
            mFontBaseline    = mFontDescentline - mFontDescent;
            mFontAscentline  = mFontDescentline - mFontAscent;
        }
    }
    
    //! Enable/Disable manual linebreaks
    inline void manualLineBreak(bool b){
        mManualBr = b;
    }
    
    //! Set text align vertical
    inline void setAlign(QuoteAlign align){
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
        mFontScaleNorm = scale;
        
        if(mBalancedBaseline){
            float fontScale   = 1.0f / fontSize * mFontScaleNorm;
            float fontAscent  = mTexFontRef->getAscent() / fontSize * mFontScaleNorm;
            float fontDescent = mTexFontRef->getDescent() / fontSize * mFontScaleNorm;
            
            static const float threshold = 1.0f;
            static const float scaleStep = 0.9995f;
            
            // make sure the descentline doesnt exceed the cells height,
            // otherwise scale down till it fits
            while((fontAscent + fontDescent) > threshold){
                fontScale   *= scaleStep;
                fontAscent  *= scaleStep;
                fontDescent *= scaleStep;
            }
            
            mFontScale     = fontScale;
            mFontAscent    = fontAscent;
            mFontDescent   = fontDescent;
        } else {
            mFontScale     = 1.0f / fontSize * mFontScaleNorm;
            mFontAscent    = mTexFontRef->getAscent()  / fontSize * mFontScaleNorm;
            mFontDescent   = mTexFontRef->getDescent() / fontSize * mFontScaleNorm;
        }
        
        balanceBaseline(mBalancedBaseline);
    }
    
private:
    inline void setFont_Internal(float fontScale){
        setFontScale(fontScale);
        float pi_2 = float(M_PI) * 0.5f;
        //mFontTransMat.setToIdentity(); // hm
        mFontTransMat *= Matrix44f::createTranslation(Vec3f(0,0,0));
        mFontTransMat *= Matrix44f::createRotation(Vec3f::xAxis(), pi_2);
        mFontTransMat *= Matrix44f::createRotation(Vec3f::yAxis(), pi_2);
        
    }
public:
    //! Return area used to clip the grid
    inline const LayoutArea& getArea() const{
        return mArea;
    }
    
    inline const LayoutArea& getArea(){
        return mArea;
    }
    
    //! Set the font
    inline void setFont(const string& fontName, float fontSize = 50.0f, float fontScale = 1.0f){
        mTexFontRef = gl::TextureFont::create(Font(fontName,fontSize),mTexFontFormat);
        setFont_Internal(fontScale);
    }
    
    inline void setFont(const Font& font, float fontScale = 1.0f){
        mTexFontRef = gl::TextureFont::create(font,mTexFontFormat);
        setFont_Internal(fontScale);
    }
    
    //! Clear
    inline void clear(){
        mLines.clear();
    }
    
    inline bool isValid(){
        return mValid;
    }
    
    //! Dont render text that exceeds the available cells
    inline void constrain(bool b = true){
        mConstrain = b;
    }
    
    //! Set the string
    inline bool setString(const string& str){
        if(!mValid){
            return false;
        }
        
        mLines.resize(0);
        
        if (str.size() == 0) {
            createQuote();
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
                
                // inbetween
                for(string::iterator itr = input.begin(); itr != input.end() - 1; ++itr){
                    if (*itr == br) {   // current is break
                        string::iterator _itr(itr+1);
                        if(*_itr != ' '){
                            //remove double breaks, not seperated by space
                            while(*(_itr) == br){
                                input.erase(_itr++);
                            }
                        }
                    }
                }
                
                // add space after linebreak
                for(int i = 0; i < input.size(); ++i){
                    if(input[i] == br && input[i+1] != ' '){
                        input.insert(++i, " ");
                    }
                    
                }
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
            addLine_Internal(mLines, input, row);
            createQuote();
            return true;
        }
        
        deque<string> words;
        split(words, input, is_any_of(" "));
        
        string token;
        int    tokenNumBr;
        bool   tokenHasBr = false;
        int    tokenCountBr = 0;
        
        string line;
        string space;
        
        vector<Line_Internal> lines;
        
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
            
            if(tokenHasBr){
                if(lineWidth < colWidth){
                    line      += token;
                    addLine_Internal(lines, line, row);
                    
                    line.clear();
                    space.clear();
                    words.pop_front();
                    row++;
                    
                } else { // manually breaked string exceeds row, sorry
                    // do auto resizing here
                    if(!mConstrain){
                        mLines = lines;
                        createQuote();
                    }
                    return false;
                }
                
            } else {
                if(lineWidth< colWidth){
                    line      += token;
                    space = " ";
                    words.pop_front();
                    
                } else {
                    addLine_Internal(lines, line, row);
                    
                    line.clear();
                    space.clear();
                    row++;
                }
            }
            
            // line breaked string exceeds rows, sorry
            if(row >= rowMax){
                // do auto resizing here
                if(!mConstrain){
                    mLines = lines;
                    createQuote();
                }
                return false;
            }
            
            if(words.size() == 0){
                addLine_Internal(lines, line, row);
            }
        }
        
        mLines = lines;
        
        createQuote();
        return true;
    }
    
    inline QuoteRef getQuote(){
        return mQuote;
    }
    
    
    void debugTexture(bool b = true){
        mDebugTexture = b;
        
    }
};




#endif
