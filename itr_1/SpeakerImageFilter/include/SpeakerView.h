//
// Created by Henryk Wollik on 14/04/14.
//

#ifndef __SpeakerView_H_
#define __SpeakerView_H_

#include <OpenGL/OpenGL.h>
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Color.h"
#include "Speaker.h"
#include "PingPongFbo.h"

#include "cinder/Timeline.h"

using namespace ci;

class SpeakerView {
    const static Vec2f  sTexCoordsNorm[4];
    const static Vec3f  sCubeVertices[8];
    const static size_t sCardVerticesLen;
    const static Vec3f  sCardVertices[18];
    const static Vec3f  sCardNormals[18];

    Speaker* mData;
    
    Vec2f mImageSize;

    Vec2f mSize;
    Vec2f mTexcoordsNorm[4];    // center image texcoords facing ortho cam
    Vec2f mTexcoords[18];       // texcoords distributed across unique vertices
    Vec2f mTexelSize;
    
    gl::GlslProgRef mShaderBlurHRef;
    gl::GlslProgRef mShaderBlurVRef;

    gl::Fbo mFbo0;
    gl::Fbo mFbo1;
    
    void drawFocus(float factor = 1.0f);   //0 = unfocused, 1 = focused

public:
    SpeakerView(Speaker* data);
    ~SpeakerView();
    
    void draw();
    void update();
    void focus();
    void unfocus();

};


#endif //__SpeakerView_H_
