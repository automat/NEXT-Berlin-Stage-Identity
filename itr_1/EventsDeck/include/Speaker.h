//
// Created by Henryk Wollik on 14/04/14.
//


#ifndef __Speaker_H_
#define __Speaker_H_

#include "cinder/gl/Texture.h"

// Dummy
namespace next {
    class Speaker {
        ci::gl::Texture mTextureRef;
    public:
        Speaker(){};
        inline const ci::gl::Texture getImageRef(){
            return mTextureRef.weakClone();
        };
        
        static Speaker Create(const ci::gl::Texture& image){
            Speaker speaker;
            speaker.mTextureRef = image.weakClone();
            return speaker;
        }
    };
}

#endif //__Speaker_H_
