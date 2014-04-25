//
//  TextBox.h
//  grid_01
//
//  Created by Henryk Wollik on 11/04/14.
//
//

#ifndef grid_01_TextBox_h
#define grid_01_TextBox_h

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
#include "cinder/Matrix33.h"
#include "cinder/Utilities.h"

namespace next{

    using namespace ci;
    namespace util {

        class TextBox {
            struct Line_Internal{

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





        public:

            TextBox(int fontTextureSize = 2048){
                mTexFontFormat.enableMipmapping();
                mTexFontFormat.premultiply();
                mTexFontFormat.textureWidth(fontTextureSize);
                mTexFontFormat.textureHeight(fontTextureSize);
            };

            inline void setFont(const string& fontName, float fontSize = 50.0f, float fontScale = 1.0f){
                mTexFontRef = gl::TextureFont::create(Font(fontName,fontSize),mTexFontFormat);
                setFontScale(fontScale);
            }

            inline void setFontScale(float scale){

            }

            inline void setString(const string& str){

            }


            inline void debugDraw(){

            }




        };
    }
}



#endif
