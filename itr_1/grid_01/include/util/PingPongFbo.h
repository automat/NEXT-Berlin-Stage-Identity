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

class PingPongFbo {
private:
    gl::Fbo mFbo[2];
    int		mCurrentIndex;

public:
    
    PingPongFbo(int width, int height, gl::Fbo::Format format = gl::Fbo::Format()) {
        mFbo[0] = gl::Fbo(width, height, format);
        mFbo[1] = gl::Fbo(width, height, format);
        
        mCurrentIndex = 0;
    }
    
    inline gl::Fbo& getTarget() {
        return mFbo[mCurrentIndex];
    }
    
    inline gl::Fbo& getSource() {
        return mFbo[1-mCurrentIndex];
    }
    
    void swap() {
        mCurrentIndex = 1 - mCurrentIndex;
    }
};

#endif
