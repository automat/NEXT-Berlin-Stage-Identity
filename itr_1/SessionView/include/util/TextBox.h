//
//  TextBox.h
//  Textbox
//
//  Created by Henryk Wollik on 12/04/14.
//
//

#ifndef Textbox_TextBox_h
#define Textbox_TextBox_h

#include <vector>
#include <string>

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

#include "cinder/gl/GlslProg.h"

//
//  this thing is a stripped down version of the quote setter
//

namespace next {
    using namespace std;
    using namespace ci;
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
        
        Vec2f mOrigin;        // top left if offsetted by fbo texture if dropshadow is used
        Rectf mTextureBounds; // bounds of the result texture including drophshadow
        
        
        gl::TextureFont::Format mTexFontFormat;
        gl::TextureFontRef      mTexFontRef;        // font to be used
        float                   mFontAscent;        // font ascent
        float                   mFontDescent;       // font descent
        float                   mFontScale;         // relation texture font size / desired size
        float                   mFontBaseline;      // font offset top to baseline
        float                   mFontHeight;        // font height including ascent / descent
        
        float                   mLineHeight;        // font height scale factor
        float                   mLineStep;          // product lineheight * fontheight
        Matrix44f               mTransform;
        
        vector<Line_Internal> mLines;
        string                mString;
        float                 mMaxLineWidth;
        
        vector<vector<Vec2f>> mTexcoords;
        
        float                 mWidth;
        float                 mWidthSafe; //mWidth - mUnderlineoffsetH_2
        float                 mHeight;
        
        ColorAf mColorFont;
        
        bool    mDrawUnderline;
        ColorAf mColorUnderline;
        ColorAf mColorUnderlineGradient[2];
        bool    mUnderlineUseGradient;
        float   mUnderlineHeight;
        float   mUnderlineBaselineOffset;
        float   mUnderlineOffsetH;
        float   mUnderlineOffsetH_2;
        
        
        bool    mDrawDropShadow;
        ColorAf mColorDropShadow;
        float   mDropShadowScale;
        Vec2f   mShadowOffset;
        
        //
        //  Render
        //
        
        gl::GlslProgRef mBlurShaderHRef;
        gl::GlslProgRef mBlurShaderVRef;
        
        gl::Fbo::Format mFboFormat;
        gl::Fbo         mFbo0;
        gl::Fbo         mFbo1;
        
        /*--------------------------------------------------------------------------------------------*/
        // handle string
        /*--------------------------------------------------------------------------------------------*/
        
        //! Get width of string
        float measureString(const string& str);
        //! Add processed line
        void addLine(const string& line, int row);
        
        
        /*--------------------------------------------------------------------------------------------*/
        // debug
        /*--------------------------------------------------------------------------------------------*/
        
        //! Draw bounding box of string
        void drawStringBoundingBox(const string& str);
        
        /*--------------------------------------------------------------------------------------------*/
        // render to Texture
        /*--------------------------------------------------------------------------------------------*/
        
        void enableAlphaBlending();
        void disableAlphaBlending();
        void drawClearedScreenRect(const Vec2i& size, bool originUpperLeft = true);
        void renderToTexture();
        
        //  This only calculates the coords of the actual font and its underline offset,
        //  dropshadow offset is not included
        void genTexcoords();
        
    public:
        /*--------------------------------------------------------------------------------------------*/
        // Constructor
        /*--------------------------------------------------------------------------------------------*/
        
        TextBox(int fontTextureSize = 2048);
        
        /*--------------------------------------------------------------------------------------------*/
        // set / get font properties
        /*--------------------------------------------------------------------------------------------*/
        
        void setFont(const Font& font);
        void setFontSize(float size);
        void setLineHeight(float height);
        
        
        void  setWidth(float width);
        float getWidth();
        float getHeight();
        
        Vec2f getSize();
        Vec2f getCalculatedSize();
        Rectf getCalculatedBounds();
        float getLineHeight();
        
        //! Get normalized texcoords of every single line, excluding offset by shadow
        const vector<vector<Vec2f>>& getTexcoords();
        
        int getNumLines();

        /*--------------------------------------------------------------------------------------------*/
        // set / get string
        /*--------------------------------------------------------------------------------------------*/
        
        void  reset();
        void  setString(const string& str);
        const string& getString();
        const Vec2f& getTopLeft();
        
        /*--------------------------------------------------------------------------------------------*/
        // get / set draw properties
        /*--------------------------------------------------------------------------------------------*/
        
        void    setColorFont(const ColorAf& color);
        ColorAf getColorFont();

        void    setColorUnderline(const ColorAf& color);
        ColorAf getColorUnderline();
        
        
        void    gradientUnderline(bool gradient = true);
        void    setColorGradientUnderline(const ColorAf& start, const ColorAf& end);
        ColorAf getGradientColorUnderlineStart();
        ColorAf getGradientColorUnderlineEnd();
        
        void    setColorDropShadow(const ColorAf& color);
        ColorAf getColorDropShadow();
        
        void  setDropShadowScale(float scale);
        float getDropShadowScale();
        
        void  setDropShadowOffset(const Vec2f& positiveOffset);
        Vec2f getDropShadowOffset();
        
        void  setDropShadowAlpha(float alpha);
        float getDropShadowAlpha();
        
        void dropShadow(bool dropShadow = true);
        bool getDropShadow();
        
        void underline(bool underline = true);
        bool getUnderline();
        
        void  setUnderlineHeight(float height);
        float getUnderlineHeight();
        
        void  setUnderlineBaselineOffset(float offset);
        float getUnderLineBaselineOffset();
        
        /*--------------------------------------------------------------------------------------------*/
        // debug
        /*--------------------------------------------------------------------------------------------*/
    private:
        void drawTrapezoid(float width);
        
    public:
        void debugDraw();
        const gl::Texture& getTexture();
    };
}

#endif
