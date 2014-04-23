#include "util/TextBox.h"

#include <boost/assign/std/vector.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "FxResources.h"

namespace next {
    using namespace std;
    using namespace ci;
    using namespace boost;
    using namespace boost::assign;
    
    /*--------------------------------------------------------------------------------------------*/
    // handle string
    /*--------------------------------------------------------------------------------------------*/

    //! Get width of string
    float TextBox::measureString(const string& str){
        return mTexFontRef->measureString(str).x * mFontScale;
    }

    //! Add processed line
    void TextBox::addLine(const string& line, int row){
        if(line.empty()){
            return;
        }
        float lineWidth = measureString(line);
        mMaxLineWidth   = MAX(lineWidth,mMaxLineWidth);
        mHeight        += mLineStep;


        if(lineWidth == 0){
            return;
        }
        mLines += Line_Internal(Vec2f::zero(), line, lineWidth);
    }

    /*--------------------------------------------------------------------------------------------*/
    // debug
    /*--------------------------------------------------------------------------------------------*/

    //! Draw bounding box of string
    void TextBox::drawStringBoundingBox(const string& str){
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

    /*--------------------------------------------------------------------------------------------*/
    // render to Texture
    /*--------------------------------------------------------------------------------------------*/

    void TextBox::enableAlphaBlending(){
        glAlphaFunc(GL_GREATER, 0.0);
        glEnable(GL_ALPHA_TEST);
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    void TextBox::disableAlphaBlending(){
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.5); // reset what seems to be cinders default
    }

    void TextBox::drawClearedScreenRect(const Vec2i& size, bool originUpperLeft){
        gl::pushMatrices();
        glClearColor( 0,0,0,0 );
        glClearDepth(1.0f);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        gl::setMatricesWindow(size,originUpperLeft);
        gl::drawSolidRect( Rectf( 0, 0, size.x, size.y) );
        gl::popMatrices();
    }

    void TextBox::renderToTexture(){
        float offset       = mDrawUnderline ? mUnderlineOffsetH_2 : 0;
        float maxLineWidth = mMaxLineWidth + offset;

        //  Calc offsets
        float shadowOffsetX = mShadowOffset.x - offset;
        float shadowOffsetY = mShadowOffset.y;
        float shadowTranslationX = MAX(0,mShadowOffset.x + offset);
        float shadowTranslationY = MAX(0,shadowOffsetY);

        //  Calc Texture offset for final texture translation
        mOrigin.x = MIN(-offset, shadowOffsetX);
        mOrigin.y = MIN(0,       shadowOffsetY);

        //  Calc output texture size
        mTextureBounds.set(mOrigin.x, mOrigin.y,
                MAX(shadowOffsetX + maxLineWidth, maxLineWidth),
                MAX(shadowOffsetY + mHeight, mHeight));

        //  Resize fbos / viewport / shader props
        float textureWidth    = mTextureBounds.getWidth();
        float textureHeight   = mTextureBounds.getHeight();
        Area  textureViewport = Area(0, 0, textureWidth, textureHeight);
        Vec2f texelSize       = Vec2f(1.0f / textureWidth, 1.0f / textureHeight);

        mFbo0 = gl::Fbo(textureWidth, textureHeight, mFboFormat);
        mFbo1 = gl::Fbo(textureWidth, textureHeight, mFboFormat);

        Vec2f zero;
        float row = 0;
        
        //
        //  Draw dropshadow
        //
        if(mDrawDropShadow){

            //  1st pass pure

            mFbo0.bindFramebuffer();
            glPushAttrib(GL_VIEWPORT_BIT);
            gl::pushMatrices();
            gl::setViewport(textureViewport);
            gl::setMatricesWindow(mFbo0.getSize(),false);

            gl::clear(ColorAf(0,0,0,0));

            float prevColor[4];
            glGetFloatv(GL_CURRENT_COLOR, prevColor);

            enableAlphaBlending();

            glColor4f(mColorDropShadow.r,
                    mColorDropShadow.g,
                    mColorDropShadow.b,
                    mColorDropShadow.a);

            glPushMatrix();
            glTranslatef(shadowTranslationX, shadowTranslationY, 0);
            for(const auto& line : mLines){
                glPushMatrix();
                glTranslatef(0,row,0);
                glMultMatrixf(mTransform);
                mTexFontRef->drawString(line.str, zero);
                glPopMatrix();
                row += mLineStep;
            }
            glPopMatrix();

            disableAlphaBlending();
            gl::popMatrices();
            glPopAttrib();
            mFbo0.unbindFramebuffer();


            //  2nd pass blur hotizontal

            mFbo1.bindFramebuffer();
            mFbo0.bindTexture(0);
            mBlurShaderHRef->bind();
            mBlurShaderHRef->uniform("uTexture", 0);
            mBlurShaderHRef->uniform("uTexelSize", texelSize.x);
            mBlurShaderHRef->uniform("uScale", mDropShadowScale);
            glPushAttrib(GL_VIEWPORT_BIT);
            gl::pushMatrices();
            gl::setViewport(textureViewport);
            drawClearedScreenRect(mFbo0.getSize(),true);
            gl::popMatrices();
            glPopAttrib();
            mBlurShaderHRef->unbind();
            mFbo0.getTexture().unbind(0);
            mFbo1.unbindFramebuffer();


            //  3rd pass vertical

            mFbo0.bindFramebuffer();
            mFbo1.bindTexture(0);
            mBlurShaderVRef->bind();
            mBlurShaderVRef->uniform("uTexture", 0);
            mBlurShaderVRef->uniform("uTexelSize", texelSize.y);
            mBlurShaderVRef->uniform("uScale", mDropShadowScale);
            glPushAttrib(GL_VIEWPORT_BIT);
            gl::pushMatrices();
            gl::setViewport(textureViewport);
            drawClearedScreenRect(mFbo1.getSize(),true);
            gl::popMatrices();
            glPopAttrib();
            mBlurShaderVRef->unbind();
            mFbo1.getTexture().unbind(0);
            mFbo0.unbindFramebuffer();
        }

         
        //
        //  Overlay the blurred shadow with the type
        //
        mFbo1.bindFramebuffer();

        glPushAttrib(GL_VIEWPORT_BIT);
        gl::pushMatrices();
        gl::setViewport(textureViewport);
        gl::setMatricesWindow(mFbo1.getSize(),false);
        gl::clear(ColorAf(0,0,0,0));
        gl::disableDepthRead();

        
        //
        //  Draw underline
        //
        if(mDrawUnderline){
            float underlineHeight_2 = mUnderlineHeight * 0.5f;
            float underlineTop      = mFontBaseline + mUnderlineBaselineOffset - underlineHeight_2;
            row = 0;

            glPushMatrix();
            glTranslatef(-mOrigin.x,-mOrigin.y,0);
            for(const auto& line : mLines){
                glPushMatrix();
                glTranslatef(0,row + underlineTop,0);
                drawTrapezoid(line.width);
                glPopMatrix();
                row += mLineStep;
            }
            glPopMatrix();
        }

        
        //
        //  Draw dropshadow
        //
        if(mDrawDropShadow){
            enableAlphaBlending();
            gl::draw(mFbo0.getTexture());
            disableAlphaBlending();
        }

        //
        //  Draw font
        //
        glPushMatrix();
        glTranslatef(-mOrigin.x,-mOrigin.y,0);

        gl::enableAlphaTest();
        gl::enableAlphaBlending();

        glColor4f(mColorFont.r,
                  mColorFont.g,
                  mColorFont.b,
                  mColorFont.a);
        row = 0;
        for(const auto& line : mLines){
            glPushMatrix();
            glTranslatef(0,row,0);
            glMultMatrixf(mTransform);
            mTexFontRef->drawString(line.str, zero);
            glPopMatrix();
            row += mLineStep;
        }

        gl::disableAlphaBlending();
        gl::disableAlphaTest();
        glPopMatrix();


        gl::enableDepthRead();
        gl::popMatrices();
        glPopAttrib();

        
        mFbo1.unbindFramebuffer();

        genTexcoords();
    }

    void TextBox::genTexcoords(){
        Vec2f fontTextureSize(mTextureBounds.getSize());

        mTexcoords.resize(0);
        vector<Vec2f> texcoords;

        float lineStep = 1.0f / static_cast<float>(mLines.size());
        Vec2f down(0,lineStep);
        Vec2f lineHeight(0,mFontHeight / fontTextureSize.y);

        float shadowOffset = mShadowOffset.y / fontTextureSize.y;
        float rowStep = MAX(0,1.0f - shadowOffset);


        float row = 0;
        for(const auto& line : mLines){
            texcoords.resize(0);

            texcoords += down * row;
            texcoords += Vec2f((line.width + mUnderlineOffsetH) / fontTextureSize.x, texcoords.back().y);
            texcoords += *(texcoords.end() - 2) + lineHeight;
            texcoords += *(texcoords.end() - 2) + lineHeight;

            mTexcoords += texcoords;

            row += rowStep;
        }
    }


    /*--------------------------------------------------------------------------------------------*/
    // Constructor
    /*--------------------------------------------------------------------------------------------*/

    TextBox::TextBox(int fontTextureSize) :
    mWidth(100),
    mLineHeight(1.0f),
    mHeight(0),
    mMaxLineWidth(0),
    mDrawUnderline(false),
    mDrawDropShadow(false),
    mColorFont(1,1,1,1),
    mColorUnderline(1,0,1,1),
    mColorDropShadow(0,0,0,1),
    mDropShadowScale(0.125f),
    mUnderlineHeight(10),
    mUnderlineBaselineOffset(0),
    mUnderlineUseGradient(false){
        mTexFontFormat.enableMipmapping();
        mTexFontFormat.premultiply();
        mTexFontFormat.textureWidth(fontTextureSize);
        mTexFontFormat.textureHeight(fontTextureSize);

        mFboFormat.setSamples(8);
        mFboFormat.setColorInternalFormat(GL_RGBA16F_ARB);
        mFboFormat.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        
        mBlurShaderHRef = FxResources::GetBlurH();
        mBlurShaderVRef = FxResources::GetBlurV();

        setUnderlineHeight(mUnderlineHeight);
        
        mColorUnderlineGradient[0] = ColorAf::white();
    };

    /*--------------------------------------------------------------------------------------------*/
    // set / get font properties
    /*--------------------------------------------------------------------------------------------*/

    void TextBox::setFont(const Font& font){
        mTexFontRef = gl::TextureFont::create(font,mTexFontFormat);
        setFontSize(mTexFontRef->getFont().getSize());
    }

    void TextBox::setFontSize(float size){
        size = MAX(1,size);
        float fontSize = mTexFontRef->getFont().getSize();
        
        const static float aliasingOffset = 3.0f; // prevent cutoffs

        mFontScale     = size / fontSize;
        mFontAscent    = mTexFontRef->getAscent();
        mFontDescent   = mTexFontRef->getDescent();
        mFontBaseline  = (mFontAscent - mFontDescent) * mFontScale + aliasingOffset;
        mFontHeight    = mFontAscent * mFontScale;

        mTransform.setToIdentity();
        mTransform.translate(Vec3f(0,int(mFontBaseline),0));
        mTransform.scale(Vec3f(mFontScale,mFontScale,1));

        setLineHeight(mLineHeight);
    }

    void TextBox::setLineHeight(float height){
        mLineHeight = MAX(height,1.0f);
        mLineStep   = (mFontAscent * mFontScale) * mLineHeight;
    }


    void TextBox::setWidth(float width){
        mWidth = width;
        setUnderlineHeight(mUnderlineHeight);
    }

    float TextBox::getWidth(){
        return mWidth;
    }

    float TextBox::getHeight(){
        return MAX(0,mHeight);
    }

    Vec2f TextBox::getSize(){
        return Vec2f(getWidth(), getHeight());
    }

    Vec2f TextBox::getCalculatedSize(){
        return mTextureBounds.getSize();
    }

    Rectf TextBox::getCalculatedBounds(){
        return mTextureBounds;
    }

    float TextBox::getLineHeight(){
        return mLineHeight;
    }

    const vector<vector<Vec2f>>& TextBox::getTexcoords(){
        return mTexcoords;
    }

    int TextBox::getNumLines(){
        return mLines.size();
    }

    /*--------------------------------------------------------------------------------------------*/
    // set / get string
    /*--------------------------------------------------------------------------------------------*/

    void TextBox::reset(){
        mLines.resize(0);
        mMaxLineWidth = 0;
        mOrigin.set(0,0);
        mHeight = 0;

        mTextureBounds.set(0,0,0,0);
    }


    void TextBox::setString(const string& str){
        reset();
        
        if(str.empty()){
            renderToTexture();  // well, in this case reset
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

        if(!hasBr && lineWidth <= mWidthSafe){
            addLine(input, row);
            mString = str;
            renderToTexture();
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

            if(measureString(token) > mWidthSafe){
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
        renderToTexture();
    }

    const string& TextBox::getString(){
        return mString;
    }

    const Vec2f& TextBox::getTopLeft(){
        return mOrigin;
    }

    /*--------------------------------------------------------------------------------------------*/
    // get / set draw properties
    /*--------------------------------------------------------------------------------------------*/

    void TextBox::setColorFont(const ColorAf& color){
        mColorFont = color;
    }

    ColorAf TextBox::getColorFont(){
        return mColorFont;
    }

    void TextBox::setColorUnderline(const ColorAf& color){
        mColorUnderline = color;
    }

    ColorAf TextBox::getColorUnderline(){
        return mColorUnderline;
    }
    
    void TextBox::setColorGradientUnderline(const ColorAf &start, const ColorAf &end){
        mColorUnderlineGradient[0] = start;
        mColorUnderlineGradient[1] = end;
    }
    
    void TextBox::gradientUnderline(bool gradient){
        mUnderlineUseGradient = gradient;
    }
    
    ColorAf TextBox::getGradientColorUnderlineStart(){
        return mColorUnderlineGradient[0];
    }
    
    ColorAf TextBox::getGradientColorUnderlineEnd(){
        return mColorUnderlineGradient[1];
    }

    void TextBox::setColorDropShadow(const ColorAf& color){
        mColorDropShadow = color;
    }

    ColorAf TextBox::getColorDropShadow(){
        return mColorDropShadow;
    }

    void TextBox::setDropShadowScale(float scale){
        mDropShadowScale = scale;
    }

    float TextBox::getDropShadowScale(){
        return mDropShadowScale;
    }

    void TextBox::setDropShadowOffset(const Vec2f&positiveOffset){
        mShadowOffset.x = MAX(0, positiveOffset.x);
        mShadowOffset.y = MAX(0, positiveOffset.y);
    }

    Vec2f TextBox::getDropShadowOffset(){
        return mShadowOffset;
    }

    void TextBox::setDropShadowAlpha(float alpha){
        mColorDropShadow.a = alpha;
    }

    float TextBox::getDropShadowAlpha(){
        return mColorDropShadow.a;
    }

    void TextBox::dropShadow(bool dropShadow){
        mDrawDropShadow = dropShadow;
    }

    bool TextBox::getDropShadow(){
        return mDrawDropShadow;
    }

    void TextBox::underline(bool underline){
        mDrawUnderline = underline;
    }

    bool TextBox::getUnderline(){
        return mDrawUnderline;
    }

    void TextBox::setUnderlineHeight(float height){
        mUnderlineOffsetH   = height * 1.705f;
        mUnderlineOffsetH_2 = mUnderlineOffsetH * 0.5f;
        mUnderlineHeight    = height;
        mWidthSafe          = mWidth - mUnderlineOffsetH_2;
    }

    float TextBox::getUnderlineHeight(){
        return mUnderlineHeight;
    }

    void TextBox::setUnderlineBaselineOffset(float offset){
        mUnderlineBaselineOffset = offset;
    }

    float TextBox::getUnderLineBaselineOffset(){
        return mUnderlineBaselineOffset;
    }

    /*--------------------------------------------------------------------------------------------*/
    // debug
    /*--------------------------------------------------------------------------------------------*/

    void TextBox::drawTrapezoid(float width){
        float vertices[8] = {
                -mUnderlineOffsetH_2 + mUnderlineOffsetH,0,
                -mUnderlineOffsetH_2 + mUnderlineOffsetH+width,0,
                -mUnderlineOffsetH_2,  mUnderlineHeight,
                -mUnderlineOffsetH_2 + width,mUnderlineHeight
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        
        if(mUnderlineUseGradient){
            const ColorAf& start = mColorUnderlineGradient[0];
            const ColorAf& end   = mColorUnderlineGradient[1];
            float colors[16] = {
                start.r,start.g,start.b,start.a,
                end.r,end.g,end.b,end.a,
                start.r,start.g,start.b,start.a,
                end.r,end.g,end.b,end.a,
            };
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, colors);
            
        } else {
            glColor4f(mColorUnderline[0],mColorUnderline[1],mColorUnderline[2],1.0f);
        }
        
        glVertexPointer(2, GL_FLOAT, 0, &vertices[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        if(mUnderlineUseGradient){
            glDisableClientState(GL_COLOR_ARRAY);
        }
        
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    void TextBox::debugDraw(){
        glPushMatrix();
        glTranslatef(-mOrigin.x, -mOrigin.y, 0);
        Vec2f zero;
        Vec2f right;

        float prevColor[4];
        glGetFloatv(GL_CURRENT_COLOR, prevColor);

        float underlineHeight_2 = mUnderlineHeight * 0.5f;
        float underlineMid      = mFontBaseline + mUnderlineBaselineOffset;
        float underlineTop      = underlineMid - underlineHeight_2;

        float row = 0;
        for(const auto& line : mLines){
            right.x = line.width;

            glColor3f(1, 1, 1);
            glPushMatrix();
            glTranslatef(0,row,0);
            drawStringBoundingBox(line.str);

            glColor3f(0, 0, 1);
            glTranslatef(0, underlineTop, 0);
            gl::drawLine(zero, right);
            glTranslatef(0, mUnderlineHeight, 0);
            gl::drawLine(zero, right);

            glPopMatrix();
            row += mLineStep;
        }

        glPushMatrix();
        glColor3f(1,0,1);
        gl::drawStrokedRect(Rectf(0,0,mWidth, row));
        glPopMatrix();


        float prevLineWidth;

        glGetFloatv(GL_LINE_WIDTH, &prevLineWidth);
        glLineWidth(2);
        glPushMatrix();
        glColor3f(0.125,0,0.75f);
        gl::drawStrokedRect(mTextureBounds);
        glPopMatrix();
        glLineWidth(prevLineWidth);

        glColor3f(1,0,0);
        gl::drawSolidCircle(mOrigin, 5);

        glPopMatrix();

        float width  = mTextureBounds.getWidth();
        float height = mTextureBounds.getHeight();

        for (const auto& texcoords : mTexcoords) {
            glPushMatrix();
            for (const auto& texcoord : texcoords) {
                gl::drawSolidCircle(Vec2f(texcoord.x * width, texcoord.y * height), 2.0);
            }

            glPopMatrix();
        }
        
        glColor4f(prevColor[0],prevColor[1],prevColor[2],prevColor[3]);
    }

    const gl::Texture& TextBox::getTexture(){
        return mFbo1.getTexture();
    }

}