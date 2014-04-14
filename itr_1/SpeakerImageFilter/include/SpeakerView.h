//
// Created by Henryk Wollik on 14/04/14.
//

#ifndef __SpeakerView_H_
#define __SpeakerView_H_

#include <OpenGL/OpenGL.h>
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Color.h"
#include "Speaker.h"
#include "PingPongFbo.h"

#include "cinder/Timeline.h"

using namespace ci;

class SpeakerView {
    static gl::GlslProg __shaderBlurH;
    static gl::GlslProg __shaderBlurV;
    static gl::GlslProg __shaderMixTint;

    Speaker* mData;

    Vec2f mSize;
    Vec2f mTexcoordsNorm[4];    // center image texcoords facing ortho cam
    Vec2f mTexcoords[18];       // texcoords distributed across unique vertices

    PingPongFbo* mFboPingPong;

    void drawFocus(float factor = 1.0f);   //0 = unfocused, 1 = focused

public:
    SpeakerView(Speaker* data);

    void draw();
    void update();
    void focus();
    void unfocus();

};


#endif //__SpeakerView_H_
