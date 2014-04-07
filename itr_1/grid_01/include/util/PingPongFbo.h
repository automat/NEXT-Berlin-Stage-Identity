//
//  PingPongFbo.h
//  grid_00
//
//  Created by Henryk Wollik on 02/04/14.
//
//

#ifndef grid_00_PingPongFbo_h
#define grid_00_PingPongFbo_h

#include <OpenGL/OpenGL.h>
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

using namespace ci;

class PingPongFbo{
private:
    gl::Fbo mFbo;
    
    GLenum  mBuffers[2];
    int		mCurrentIndex;

public:
    PingPongFbo(){}
    
    PingPongFbo(int width, int height, gl::Fbo::Format format = gl::Fbo::Format()){
        format.enableColorBuffer(true, 2);
        mFbo = gl::Fbo(width,height,format);
        mBuffers[0] = GL_COLOR_ATTACHMENT0;
        mBuffers[1] = GL_COLOR_ATTACHMENT0 + 1;
        mCurrentIndex = 0;
    }
    
    inline void bindFramebuffer(){
        mFbo.bindFramebuffer();
        //glDrawBuffer(1,getTarget());
        glDrawBuffer(getTarget());
    }
    
    inline void unbindFramebuffer(){
        mFbo.unbindFramebuffer();
    }
    
    inline GLenum getTarget() {
        return mBuffers[mCurrentIndex];
    }
    
    inline GLenum getSource() {
        return mBuffers[1-mCurrentIndex];
    }
    
    inline void bindTexture(int index, int attachment){
        mFbo.bindTexture(index, attachment);
    }
    
    inline void unbindTexture(int index){
        mFbo.unbindTexture();
    }
    
    inline void bindTargetTexture(int index){
        mFbo.bindTexture(index,getTarget());
    }
    
    inline void bindSourceTexture(int index){
        //mFbo.bindTexture(index,getSource());
        //mFbo.bindTexture(index, getTarget());
    }
    
    inline gl::Texture& getTexture(int attachment = 0){
        return mFbo.getTexture(attachment);
    }
    
    
    inline gl::Texture& getTargetTexture(){
        return mFbo.getTexture(getTarget());
    }
    
    inline gl::Texture& getSourceTexture(){
        return mFbo.getTexture(getSource());
    }
    
    inline void swap() {
        mCurrentIndex = 1 - mCurrentIndex;
    }
    
    inline Area getBounds(){
        return mFbo.getBounds();
    }
    
    inline Vec2f getSize(){
        return mFbo.getSize();
    }
};

#endif
