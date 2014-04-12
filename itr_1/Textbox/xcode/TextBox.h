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
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Matrix44.h"
#include "cinder/Utilities.h"
#include "cinder/Vector.h"



namespace utils {
    using namespace std;
    using namespace ci;
    using namespace boost;
    using namespace boost::assign;
    class TextBox{
        struct Line_Internal{
            Vec2f pos;
            string str;
            float width;
            int row;
            Line_Internal(){}
            Line_Internal(const Vec2f& pos, const string& str, float width, int row) :
                pos(pos),str(str),width(width), row(row){}
        };
        
        
        gl::TextureFont::Format mTexFontFormat;
        gl::TextureFontRef      mTexFontRef;        // font to be used
        float                   mFontAscent;        // font ascent
        float                   mFontDescent;       // font descent
        float                   mFontScale;
        float                   mFontScaleNorm;
        float                   mFontBaseline;      // text draw aling baseline
        float                   mFontAscentline;    // text draw align ascent
        float                   mFontDescentline;   // text draw align descent
        
        vector<Line_Internal> mLines;
        string                mString;
        float                 mWidth;
        float                 mHeight;
        Matrix44f             mTransform;
        
        
        
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
        
        inline void addLine(const string& line, int row){
            if(line.empty()){
                return;
            }
            float lineWidth = measureString(line);
            if(lineWidth == 0){
                return;
            }
            
            mLines += Line_Internal(Vec2f::zero(), line, lineWidth, row);
        }
        
    public:
        
        
        TextBox(int fontTextureSize = 2048) :
            mWidth(100),
            mHeight(0){
            mTexFontFormat.enableMipmapping();
            mTexFontFormat.premultiply();
            mTexFontFormat.textureWidth(fontTextureSize);
            mTexFontFormat.textureHeight(fontTextureSize);
        };
        
        inline void setFont(const Font& font){
            mTexFontRef = gl::TextureFont::create(font,mTexFontFormat);
            setFontSize(mTexFontRef->getFont().getSize());
            //setFontScale(1.0f);
        }
        
        inline void setFontSize(float size){
            float fontSize = mTexFontRef->getFont().getSize();
            mFontScaleNorm = size;
            mFontScale     = mFontScaleNorm / fontSize;
            
            cout << mFontScaleNorm << " / " << fontSize << " " << mFontScale << endl;
            
            
            mFontAscent   = mTexFontRef->getAscent();
            mFontDescent  = mTexFontRef->getDescent();
            
        
            mTransform.setToIdentity();
            mTransform.translate(Vec3f(0,int((mFontAscent - mFontDescent) * mFontScale),0));
            mTransform.scale(Vec3f(mFontScale,mFontScale,1));

            
            
        }
        
        inline void setString(const string& str){
            if(str == mString){
                return;
            }
            
            mString = str;
            mLines.resize(0);
            
            if(str.empty()){
                return;
            }
            
            string input(str);
            
            const char br('\n');
            int  numBr = count(input.begin(), input.end(), br);
            bool hasBr = numBr != 0;
            
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
                
                if(hasBr && tokenCountBr <= numBr){
                    tokenNumBr = count(token.begin(),token.end(),br);
                    tokenHasBr = tokenNumBr != 0;
                    if(tokenHasBr){
                        // remove br char
                        token.erase(remove(token.begin(), token.end(), br), token.end());
                    }
                    tokenCountBr += tokenNumBr;
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
                
                
                /*
                if(tokenHasBr){
                    if(lineWidth < mWidth){
                        line      += token;
                        addLine(line, row);
                        
                        line.clear();
                        space.clear();
                        words.pop_front();
                        row++;
                        
                    } else {
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
                 */
                
                

            }
            
            
            
            
            
            
            
           
            
            
            
            
            
            
            
            
            
        }
        
        
        inline void debugDraw(){
            
            Vec2f zero;
            
            Vec2f linePos;
            Vec2f textPos;
            

            
            for(vector<Line_Internal>::const_iterator itr = mLines.begin(); itr != mLines.end(); ++itr){
                glPushMatrix();
                glTranslatef(0,itr->row * mFontScaleNorm,0);
                glMultMatrixf(&mTransform[0]);
                mTexFontRef->drawString(itr->str, zero);
                glPopMatrix();
            }
            
            float prevColor[4];
            glGetFloatv(GL_CURRENT_COLOR, prevColor);
            
            glPushMatrix();
            glColor3f(1,0,1);
            gl::drawStrokedRect(Rectf(0,0,mWidth, 200));
            glPopMatrix();
            
            glColor3f(prevColor[0], prevColor[1], prevColor[2]);
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
        
        inline const string& getString(){
            return mString;
        }

        
        
        
        
    };
}

#endif
