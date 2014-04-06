//
//  PingPongFbo.h
//  grid_00
//
//  Created by Henryk Wollik on 02/04/14.
//
//

#ifndef grid_00_PingPongFbo_h
#define grid_00_PingPongFbo_h

#include "cinder/gl/Fbo.h"

using namespace ci;

class PingPongFbo : public gl::Fbo{
private:
    GLenum  mBuffers[2];
    int		mCurrentIndex;

public:
    PingPongFbo() : gl::Fbo(){}
    
    PingPongFbo(int width, int height, gl::Fbo::Format format = gl::Fbo::Format()) :
    gl::Fbo(width, height, format){
        mBuffers[0] = GL_COLOR_ATTACHMENT0;
        mBuffers[1] = GL_COLOR_ATTACHMENT1;
        mCurrentIndex = 0;
    }
    
    inline void bindFramebuffer(){
        gl::Fbo::bindFramebuffer();
        glDrawBuffers(2, mBuffers);
    }
    
    inline GLenum getTarget() {
        return mBuffers[mCurrentIndex];
    }
    
    inline GLenum getSource() {
        return mBuffers[1-mCurrentIndex];
    }
    
    inline gl::Texture& getTargetTexture(){
        return getTexture(getTarget());
    }
    
    inline gl::Texture& getSourceTexture(){
        return getTexture(getSource());
    }
    
    void swap() {
        mCurrentIndex = 1 - mCurrentIndex;
    }
};

#endif
