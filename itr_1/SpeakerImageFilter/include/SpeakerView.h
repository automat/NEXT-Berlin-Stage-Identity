//
// Created by Henryk Wollik on 14/04/14.
//


#ifndef __SpeakerView_H_
#define __SpeakerView_H_

#include <OpenGL/OpenGL.h>
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"


using namespace ci;
class SpeakerView {
    Vec2f mSize;
    Vec3f mPoints[4];
    Vec2f mTexcoords[4];


public:
    SpeakerView();

    void draw();
    void update();
    void focus();
    void unfocus();

    inline const Vec3f& getTL(){
        return mPoints[0];
    }

    inline const Vec3f& getTR(){
        return mPoints[1];
    }

    inline const Vec3f& getBL(){
        return mPoints[2];
    }

    inline const Vec3f& getBR(){
        return mPoints[3];
    }

};


#endif //__SpeakerView_H_
