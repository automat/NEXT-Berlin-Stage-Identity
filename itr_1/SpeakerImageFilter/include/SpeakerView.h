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

public:
    SpeakerView();

    void draw();
    void update();
    void focus();
    void unfocus();

};


#endif //__SpeakerView_H_
