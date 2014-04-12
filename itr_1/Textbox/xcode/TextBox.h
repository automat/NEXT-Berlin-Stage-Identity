//
//  TextBox.h
//  Textbox
//
//  Created by Henryk Wollik on 12/04/14.
//
//

#ifndef Textbox_TextBox_h
#define Textbox_TextBox_h

#include <algorithm>
#include <vector>
#include <string>

#include <OpenGL/OpenGL.h>
#include <boost/assign/std/vector.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "Resources.h"
#include "cinder/app/App.h"

#include "cinder/gl/gl.h"
#include "cinder/Font.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Matrix44.h"
#include "cinder/Utilities.h"
#include "cinder/Vector.h"


//
//  this thing is a stripped down version of the quote setter
//

namespace utils {
    using namespace std;
    using namespace ci;
    using namespace boost;
    using namespace boost::assign;
    class TextBox{
        /*--------------------------------------------------------------------------------------------*/
        // Internal
        /*--------------------------------------------------------------------------------------------*/
 
        struct Line_Internal{
            Vec2f pos;
            string str;
            float width;
            Line_Internal(){}
            Line_Internal(const Vec2f& pos, const string& str, float width) :
                pos(pos),str(str),width(width){}
        };
        
        
        gl::TextureFont::Format mTexFontFormat;
        gl::TextureFontRef      mTexFontRef;        // font to be used
        float                   mFontAscent;        // font ascent
        float                   mFontDescent;       // font descent
        float                   mFontScale;         // relation texture font size / desired size
        float                   mFontOffset;        // font offset top to baseline
        float                   mFontHeight;        // font height including ascent / descent
        
        float                   mLineHeight;        // font height scale factor
        float                   mLineStep;          // product lineheight * fontheight
        Matrix44f               mTransform;
        
        vector<Line_Internal> mLines;
        string                mString;
        
        float                 mWidth;
        float                 mHeight;
        
        ColorAf mColorFont;
        ColorAf mColorUnderline;
        ColorAf mColorDropShadow;
        float   mDropShadowScale;
        
        bool mDrawUnderline;
        bool mDrawDropShadow;
        
        /*--------------------------------------------------------------------------------------------*/
        // handle string
        /*--------------------------------------------------------------------------------------------*/
        
        //! Get width of string
        inline float measureString(const string& str){
            return mTexFontRef->measureString(str).x * mFontScale;
        }
        
        //! Add processed line
        inline void addLine(const string& line, int row){
            if(line.empty()){
                return;
            }
            float lineWidth = measureString(line);
            if(lineWidth == 0){
                return;
            }
            mLines += Line_Internal(Vec2f::zero(), line, lineWidth);
        }
        
        
        /*--------------------------------------------------------------------------------------------*/
        // debug
        /*--------------------------------------------------------------------------------------------*/
        
        //! Draw bounding box of string
        inline void drawStringBoundingBox(const string& str){
            float width = measureString(str);
            float vertices[8] = {
                0,     0,
                width, 0,
                width, mFontHeight,
                0,     mFontHeight
            };
            
            glLineWidth(2);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
            glEnable(GL_LINE_STIPPLE);
            glLineStipple(10, 0xAAAA);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
            glDisable(GL_LINE_STIPPLE);
            glDisableClientState(GL_VERTEX_ARRAY);
            glLineWidth(1);
        }
        
    public:
        /*--------------------------------------------------------------------------------------------*/
        // Constructor
        /*--------------------------------------------------------------------------------------------*/
        
        TextBox(int fontTextureSize = 2048) :
            mWidth(100),
            mLineHeight(1.0f),
            mHeight(0),
            mDrawUnderline(false),
            mDrawDropShadow(false),
            mColorFont(1,1,1,1),
            mColorUnderline(1,0,1,1),
            mColorDropShadow(0,0,0,1),
            mDropShadowScale(0.125f){
                mTexFontFormat.enableMipmapping();
                mTexFontFormat.premultiply();
                mTexFontFormat.textureWidth(fontTextureSize);
                mTexFontFormat.textureHeight(fontTextureSize);
        };
        
        /*--------------------------------------------------------------------------------------------*/
        // set / get font properties
        /*--------------------------------------------------------------------------------------------*/

        inline void setFont(const Font& font){
            mTexFontRef = gl::TextureFont::create(font,mTexFontFormat);
            setFontSize(mTexFontRef->getFont().getSize());
        }
        
        inline void setFontSize(float size){
            float fontSize = mTexFontRef->getFont().getSize();
     
            mFontScale     = size / fontSize;
            mFontAscent    = mTexFontRef->getAscent();
            mFontDescent   = mTexFontRef->getDescent();
            mFontOffset    = (mFontAscent - mFontDescent) * mFontScale;
            mFontHeight    = mFontAscent * mFontScale;
            
            mTransform.setToIdentity();
            mTransform.translate(Vec3f(0,int(mFontOffset),0));
            mTransform.scale(Vec3f(mFontScale,mFontScale,1));
            
            setLineHeight(mLineHeight);
        }
        
        inline void setLineHeight(float height){
            mLineHeight = height;
            mLineStep   = (mFontAscent * mFontScale) * mLineHeight;
        }
        
        
        inline void setWidth(float width){
            mWidth = width;
        }
        
        inline float getWidth(){
            return mWidth;
        }
        
        inline float getHeight(){
            return mHeight;
        }
        
        inline float getLineHeight(){
            return mLineHeight;
        }
        
        /*--------------------------------------------------------------------------------------------*/
        // set / get string
        /*--------------------------------------------------------------------------------------------*/

        inline void setString(const string& str){
            mLines.resize(0);
            
            if(str.empty()){
                return;
            }
            
            string input(str);
            
            const char br('\n');
            int  numBr = count(input.begin(), input.end(), br);
            bool hasBr = numBr != 0;
            
            if(hasBr){
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

            
            float lineWidth = measureString(input);
            int row = 0;
            
            if(!hasBr && lineWidth <= mWidth){
                addLine(input, row);
                return;
            }
            
            deque<string> words;
            
            split(words, input, is_any_of(" "));
            
            string token;
            int    tokenNumBr;
            int    tokenHasBr = false;
            int    tokenCountBr = 0;
            
            string line;
            string space;
            
            while(words.size() > 0){
                token     = space + words.front();
                
                if(measureString(token) > mWidth){
                    break;
                }
                //
                //  This is a bit wonky
                //
                if(hasBr){
                    if(tokenCountBr <= numBr){
                        tokenNumBr = count(token.begin(),token.end(),br);
                        tokenHasBr = tokenNumBr != 0;
                        if(tokenHasBr){
                            // remove br char
                            token.erase(remove(token.begin(), token.end(), br), token.end());
                            tokenCountBr += tokenNumBr;
                            
                        }
                    } else {
                        tokenHasBr = false;
                    }
                }

                lineWidth = measureString(line + token);

                if(tokenHasBr){
                    if (lineWidth < mWidth) {
                        line += token;
                        addLine(line, row);
                        
                        line.clear();
                        space.clear();
                        words.pop_front();
                        row++;
                    } else{
                        addLine(line, row);
                        
                        line.clear();
                        space.clear();
                        row++;
                    
                    }
                } else {
                    
                    if(lineWidth< mWidth){
                        line      += token;
                        space = " ";
                        words.pop_front();
                        
                    } else {
                        addLine(line, row);
                        
                        line.clear();
                        space.clear();
                        row++;
                    }
                }
                
                if(words.size() == 0){
                    addLine(line,row);
                }
            }
            
            mString = str;
        }
        
        inline const string& getString(){
            return mString;
        }
        
        /*--------------------------------------------------------------------------------------------*/
        // get / set draw properties
        /*--------------------------------------------------------------------------------------------*/
        
        inline void setColorFont(const ColorAf& color){
            mColorFont = color;
        }
        
        inline const ColorAf& getColorFont(){
            return mColorFont;
        }
        
        inline void setColorUnderline(const ColorAf& color){
            mColorUnderline = color;
        }
        
        inline const ColorAf& getColorUnderline(){
            return mColorUnderline;
        }
        
        inline void setColorDropShadow(const ColorAf& color){
            mDrawDropShadow = color;
        }
        
        inline const ColorAf& getColorDropShadow(){
            return mColorDropShadow;
        }
        
        inline void setDropShadowScale(float scale){
            mDrawDropShadow = scale;
        }
        
        inline float getDropShadowScale(){
            return mDropShadowScale;
        }
        
        inline void dropShadow(bool dropShadow = true){
            mDrawDropShadow = dropShadow;
        }
        
        inline bool getDropShadow(){
            return mDrawDropShadow;
        }
        
        inline void underline(bool underline = true){
            mDrawUnderline = underline;
        }
        
        inline bool getUnderline(){
            return mDrawUnderline;
        }
        
        
        
        
        /*--------------------------------------------------------------------------------------------*/
        // debug
        /*--------------------------------------------------------------------------------------------*/
        
        inline void debugDraw(){
            
            Vec2f zero;
            
            Vec2f linePos;
            Vec2f textPos;
            
            float line = 0;
            
            
            
            gl::enableAlphaTest();
            gl::enableAdditiveBlending();
            for(vector<Line_Internal>::const_iterator itr = mLines.begin(); itr != mLines.end(); ++itr){
                glPushMatrix();
                glTranslatef(0,line,0);
                
                glPushMatrix();
                drawStringBoundingBox(itr->str);
                glPopMatrix();
                
                glPushMatrix();
                glMultMatrixf(&mTransform[0]);
                mTexFontRef->drawString(itr->str, zero);
                glPopMatrix();
                
                glPopMatrix();
                line += mLineStep;
            }
            gl::disableAlphaBlending();
            gl::disableAlphaTest();
            
            float prevColor[4];
            glGetFloatv(GL_CURRENT_COLOR, prevColor);
            
            glPushMatrix();
            glColor3f(1,0,1);
            gl::drawStrokedRect(Rectf(0,0,mWidth, line));
            glPopMatrix();
            
            glColor3f(prevColor[0], prevColor[1], prevColor[2]);
        }
        
        
        

        
        
        
        
    };
}

#endif
