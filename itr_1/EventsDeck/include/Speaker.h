//
// Created by Henryk Wollik on 14/04/14.
//

#ifndef __Speaker_H_
#define __Speaker_H_

#include "cinder/gl/Texture.h"

// Dummy
namespace next {
    using namespace ci;
    class Speaker {
        gl::Texture mTextureRef;
    
    public:
        Speaker(){};
    
        inline const gl::Texture getImageRef(){
            return mTextureRef.weakClone();
        };
        
        static Speaker Create(const gl::Texture& image){
            Speaker speaker;
            speaker.mTextureRef = image.weakClone();
            return speaker;
        }
    };
}

#endif
