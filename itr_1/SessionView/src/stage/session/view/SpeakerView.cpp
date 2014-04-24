//
// Created by Henryk Wollik on 14/04/14.
//

#include "stage/session/view/SpeakerView.h"
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include <iostream>
#include <math.h>

#include "util/FxResources.h"
#include "Config.h"

namespace next {
    using namespace std;
    
    typedef EaseOutQuad   AnimEaseOut;
    typedef EaseOutInSine AnimEaseHide;
    
    /*--------------------------------------------------------------------------------------------*/
    //  Const coords texture facing orthographic camera
    /*--------------------------------------------------------------------------------------------*/
    
    // for calc see: https://github.com/preciousforever/NEXT-Everything/blob/ae1c84c506b1d06c590cc32cf6680075e15c4b93/itr_1/SpeakerImageFilter/src/SpeakerImageFilterApp.cpp
    const Vec2f SpeakerView::sTexCoordsNorm[4] = {
        Vec2f(0.5, 0),
        Vec2f(1.0, 0.28867516),
        Vec2f(0,   0.28867516),
        Vec2f(0.5, 0.57735032)
    };
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Const geometry props
    /*--------------------------------------------------------------------------------------------*/
    
    const Vec3f SpeakerView::sCubeVertices[8] = {
        Vec3f(-SESSION_VIEW_SPEAKER_SIZE.x * 0.5, SESSION_VIEW_SPEAKER_SIZE.y * 0.5f,-SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  tl
        Vec3f( SESSION_VIEW_SPEAKER_SIZE.x * 0.5, SESSION_VIEW_SPEAKER_SIZE.y * 0.5f,-SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  tr
        Vec3f(-SESSION_VIEW_SPEAKER_SIZE.x * 0.5, SESSION_VIEW_SPEAKER_SIZE.y * 0.5f, SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  bl
        Vec3f( SESSION_VIEW_SPEAKER_SIZE.x * 0.5, SESSION_VIEW_SPEAKER_SIZE.y * 0.5f, SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  br
        
        Vec3f(-SESSION_VIEW_SPEAKER_SIZE.x * 0.5,-SESSION_VIEW_SPEAKER_SIZE.y * 0.5f,-SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  tl
        Vec3f( SESSION_VIEW_SPEAKER_SIZE.x * 0.5,-SESSION_VIEW_SPEAKER_SIZE.y * 0.5f,-SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  tr
        Vec3f(-SESSION_VIEW_SPEAKER_SIZE.x * 0.5,-SESSION_VIEW_SPEAKER_SIZE.y * 0.5f, SESSION_VIEW_SPEAKER_SIZE.x * 0.5),  //  bl
        Vec3f( SESSION_VIEW_SPEAKER_SIZE.x * 0.5,-SESSION_VIEW_SPEAKER_SIZE.y * 0.5f, SESSION_VIEW_SPEAKER_SIZE.x * 0.5)  //  br
    };
    
    
    const size_t SpeakerView::sCardVerticesLen = 6 * 3;
    const Vec3f SpeakerView::sCardVertices[18] = {
        //  top
        sCubeVertices[0],
        sCubeVertices[1],
        sCubeVertices[3],
        sCubeVertices[3],
        sCubeVertices[0],
        sCubeVertices[2],
        
        //  right
        sCubeVertices[3],
        sCubeVertices[1],
        sCubeVertices[5],
        sCubeVertices[5],
        sCubeVertices[3],
        sCubeVertices[7],
        
        // bottom
        sCubeVertices[2],
        sCubeVertices[3],
        sCubeVertices[7],
        sCubeVertices[7],
        sCubeVertices[2],
        sCubeVertices[6]
        
    };
    
    static const Vec3f xAxis(Vec3f::xAxis());
    static const Vec3f yAxis(Vec3f::yAxis());
    static const Vec3f zAxis(Vec3f::zAxis());
    
    const Vec3f SpeakerView::sCardNormals[18] = {
        yAxis,yAxis,yAxis,yAxis,yAxis,yAxis,
        xAxis,xAxis,xAxis,xAxis,xAxis,xAxis,
        zAxis,zAxis,zAxis,zAxis,zAxis,zAxis
    };
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  SpeakerView
    /*--------------------------------------------------------------------------------------------*/
    
    SpeakerView::SpeakerView(Speaker* data) :
        AbstractAnimView(),
        mData(data){
        //
        //  Setup Fbo
        //
        gl::Fbo::Format fboFormat;
        fboFormat.setSamples(4);
        
        Vec2i imageSize = data->imageRef.getSize();
        int imageWidth  = imageSize.x;
        int imageHeight = imageSize.y;
        
        mFbo0 = gl::Fbo(imageWidth,imageHeight,fboFormat);
        mFbo1 = gl::Fbo(imageWidth,imageHeight,fboFormat);
        
        mShaderBlurHRef = FxResources::GetBlurH();
        mShaderBlurVRef = FxResources::GetBlurV();
        
        //
        //  recalc camera-faced quad coords according to speaker image
        //
        float aspectRatio = data->imageRef.getAspectRatio();
        Vec2f scalar      = Vec2f(1,aspectRatio);
        Vec2f offset      = Vec2f(0,(1 - sTexCoordsNorm[3].y) * 0.5f) * 0.5f;
        
        mTexcoordsNorm[0] = sTexCoordsNorm[0] * scalar + offset; // tl
        mTexcoordsNorm[1] = sTexCoordsNorm[1] * scalar + offset; // tr
        mTexcoordsNorm[2] = sTexCoordsNorm[2] * scalar + offset; // bl
        mTexcoordsNorm[3] = sTexCoordsNorm[3] * scalar + offset; // br
        
        mTexelSize.x = 0.5f / static_cast<float>(imageWidth);
        mTexelSize.y = 0.5f / static_cast<float>(imageHeight);
        
        //
        //  distripute to unique vertices
        //
        mTexcoords[ 0] = mTexcoordsNorm[0];
        mTexcoords[ 1] = mTexcoordsNorm[1];
        mTexcoords[ 2] = mTexcoordsNorm[3];
        mTexcoords[ 3] = mTexcoordsNorm[3];
        mTexcoords[ 4] = mTexcoordsNorm[0];
        mTexcoords[ 5] = mTexcoordsNorm[2];
        
        mTexcoords[ 6] = mTexcoordsNorm[3];
        mTexcoords[ 7] = mTexcoordsNorm[1];
        mTexcoords[ 8] = mTexcoordsNorm[1];
        mTexcoords[ 9] = mTexcoordsNorm[1];
        mTexcoords[10] = mTexcoordsNorm[3];
        mTexcoords[11] = mTexcoordsNorm[3];
        
        mTexcoords[12] = mTexcoordsNorm[2];
        mTexcoords[13] = mTexcoordsNorm[3];
        mTexcoords[14] = mTexcoordsNorm[3];
        mTexcoords[15] = mTexcoordsNorm[3];
        mTexcoords[16] = mTexcoordsNorm[2];
        mTexcoords[17] = mTexcoordsNorm[2];
        
        clearStates();
    }


    void SpeakerView::drawFocus(){
        static const float scale = 10.0f;

        float focusColor    = mFocusColorState();
        float focusBlur     = mFocusBlurState();
        float focusBlurInv  = 1.0f - focusBlur;
        float focusColorInv = 1.0f - focusColor;
        
        const gl::Texture& image = mData->imageRef.weakClone();
        
        glPushAttrib(GL_VIEWPORT_BIT);
            gl::setViewport(image.getBounds());
            gl::pushMatrices();
                gl::setMatricesWindow(image.getSize(), false);
                
                mFbo0.bindFramebuffer();
                    gl::clear();
                    glColor3f(1,1,1);
                    gl::draw(image);
                mFbo0.unbindFramebuffer();
                
                mFbo1.bindFramebuffer();
                    mShaderBlurHRef->bind();
                    mShaderBlurHRef->uniform("uTexture", 0);
                    mShaderBlurHRef->uniform("uTexelSize", mTexelSize.x);
                    mShaderBlurHRef->uniform("uScale", scale * focusBlurInv);
                    gl::clear();
                    glColor3f(1,1,1);
                    gl::draw(mFbo0.getTexture());
                    mShaderBlurHRef->unbind();
                mFbo1.unbindFramebuffer();
                
                
                mFbo0.bindFramebuffer();
                    mShaderBlurVRef->bind();
                    mShaderBlurVRef->uniform("uTexture", 0);
                    mShaderBlurVRef->uniform("uTexelSize", mTexelSize.y);
                    mShaderBlurVRef->uniform("uScale", scale * focusBlurInv);
                    gl::clear();
                    glColor3f(1,1,1);
                    gl::draw(mFbo1.getTexture());
                    mShaderBlurHRef->unbind();
                mFbo0.unbindFramebuffer();
                
                mFbo1.bindFramebuffer();
                    gl::clear();
                    //blue to red
                    glColor3f(0.87450980392157f * focusColor + 0.0f * focusColorInv,
                              0.06274509803922f * focusColor + 0.39607843137255f * focusColorInv,
                              0.39607843137255f * focusColor + 0.89019607843137f * focusColorInv);
                    gl::draw(mFbo0.getTexture());
                mFbo1.unbindFramebuffer();
                
            gl::popMatrices();
        glPopAttrib();
    }
    
    /*--------------------------------------------------------------------------------------------*/
    //  Draw / Update
    /*--------------------------------------------------------------------------------------------*/
    
    void SpeakerView::draw(){
        Vec3f pos   = mPositionState();
        float scale = mScaleState();
        
        glPushMatrix();
            glTranslatef(pos.x,pos.y,pos.z);
            glScalef(scale,scale,scale);
            
            glEnableClientState(GL_VERTEX_ARRAY);
            
            glColor3f(1,1,1);
            mFbo1.getTexture().enableAndBind();
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            
            glNormalPointer(   3, GL_FLOAT,    &sCardNormals[0]);
            glTexCoordPointer( 2, GL_FLOAT, 0, &mTexcoords[0]);
            glColorPointer(    4, GL_FLOAT, 0, &mVertexColors[0]);
            glVertexPointer(   3, GL_FLOAT, 0, &sCardVertices[0]);
            glDrawArrays(GL_TRIANGLES, 0,sCardVerticesLen);
            
            glDisableClientState(GL_COLOR_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            mFbo1.unbindTexture();
            mFbo1.getTexture().disable();
         
            /*
            glColor4f(1,1,1,mColorState * 0.135f);
            static int indices[3] = {1,3,2};
            
            glVertexPointer(3, GL_FLOAT, 0, &sCubeVertices[0]);
            glDrawElements(GL_LINE_STRIP, 3, GL_UNSIGNED_INT, &indices[0]);
            glDisableClientState(GL_VERTEX_ARRAY);
            */
        glPopMatrix();
    }
    
    void SpeakerView::updateAlpha(){
        float alpha = mAlphaState();
        int i = -1;
        while(++i < 18){
            mVertexColors[i].set(1, 1, 1, alpha);
        }
    }

    void SpeakerView::focus(){
        tween(&mFocusColorState,0.0f, 1.0f, SESSION_SPEAKER_VIEW_ANIM_FOCUS, AnimEaseOut());
        tween(&mFocusBlurState, 0.0f, 1.0f, SESSION_SPEAKER_VIEW_ANIM_FOCUS, AnimEaseOut(),
              std::bind(&SpeakerView::drawFocus, this));
    }
    
    void SpeakerView::unfocus(){
        tween(&mFocusColorState,1.0f, 0.0f, SESSION_SPEAKER_VIEW_ANIM_UNFOCUS, AnimEaseOut());
        tween(&mFocusBlurState, 0.0f,       SESSION_SPEAKER_VIEW_ANIM_UNFOCUS, AnimEaseOut(),
              std::bind(&SpeakerView::drawFocus, this));
}

    void SpeakerView::unfocusOut(){
        tween(&mFocusColorState, 1.0f, 0.0f, SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_OUT, AnimEaseOut());
        tween(&mFocusBlurState,  0.0f,       SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_OUT, AnimEaseOut(),
              std::bind(&SpeakerView::drawFocus, this));
    }
    
    void SpeakerView::focusIn(){
        tween(&mFocusColorState, 0.0f, 1.0f, SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IN, AnimEaseOut());
        tween(&mFocusBlurState,  0.0f, 1.0f, SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IN, AnimEaseOut(),
              std::bind(&SpeakerView::drawFocus, this));
    }

    void SpeakerView::show() {
        tween(&mAlphaState, 1.0f, SESSION_SPEAKER_VIEW_ANIM_SHOW, AnimEaseOut(),
                std::bind(&SpeakerView::updateAlpha, this));
    }

    void SpeakerView::hide() {
        tween(&mAlphaState, 0.0f, SESSION_SPEAKER_VIEW_ANIM_HIDE, AnimEaseHide(),
                std::bind(&SpeakerView::updateAlpha, this));
    }

    void SpeakerView::unfocusImage(){
        tween(&mFocusBlurState, 0.0f ,SESSION_SPEAKER_VIEW_ANIM_UNFOCUS_IMAGE, AnimEaseOut(),
                std::bind(&SpeakerView::drawFocus, this));
    }

    void SpeakerView::clearStates(){
        mFocusColorState = 0.0f;
        mFocusBlurState  = 0.0f;
        mAlphaState      = 1.0f;
        mScaleState      = 1.0f;
        
        updateAlpha();
        drawFocus();
    }
    
}
