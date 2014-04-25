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
    gl::Fbo mFbo[2];
    int		mCurrentIndex;

public:
    PingPongFbo() :
    mCurrentIndex(0){
    }
    
    PingPongFbo(int width, int height, gl::Fbo::Format format = gl::Fbo::Format()){
        mFbo[0] = gl::Fbo(width,height,format);
        mFbo[1] = gl::Fbo(width,height,format);
        mCurrentIndex = 0;
    }
    
    inline void bindFramebuffer(){
        mFbo[mCurrentIndex].bindFramebuffer();
    }
    
    inline void unbindFramebuffer(){
        mFbo[mCurrentIndex].bindFramebuffer();
    }
    
    inline gl::Fbo& getTarget(){
        return mFbo[mCurrentIndex];
    }
    
    inline gl::Fbo& getSource(){
        return mFbo[1 - mCurrentIndex];
    }
    
    inline gl::Texture& getTargetTexture(){
        return getTarget().getTexture();
    }
    
    inline gl::Texture& getSourceTexture(){
        return getSource().getTexture();
    }
    
    inline void bindTargetTexture(int index = 0){
        getTarget().getTexture().bind(index);
    }
    
    inline void bindSourceTexture(int index = 0){
        getSource().getTexture().bind(index);
    }
    
    inline void unbindTargetTexture(int index = 0){
        getTarget().getTexture().unbind(index);
    }
    
    inline void unbindSourceTexture(int index = 0){
        getSource().getTexture().unbind(index);
    }
    
    inline void swap() {
        mCurrentIndex = 1 - mCurrentIndex;
    }
    
    inline Area getBounds(){
        return getTarget().getBounds();
    }
    
    inline Vec2f getSize(){
        return getTarget().getSize();
    }
    
    inline int getWidth(){
        return getTarget().getWidth();
    }
    
    inline int getHeight(){
        return getTarget().getHeight();
    }
};

#endif
