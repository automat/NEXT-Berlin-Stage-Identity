//
// Created by Henryk Wollik on 14/04/14.
//

#include "SpeakerView.h"
#include <OpenGL/OpenGL.h>
#include <iostream>

using namespace std;

SpeakerView::SpeakerView() :
    mSize(1,1){

    float sizeH_2 = mSize.x * 0.5f;
    float sizeV_2 = mSize.y * 0.5f;

    mPoints[0] = Vec3f(-sizeH_2,0,-sizeV_2);
    mPoints[1] = Vec3f( sizeH_2,0,-sizeV_2);
    mPoints[2] = Vec3f(-sizeH_2,0, sizeV_2);
    mPoints[3] = Vec3f( sizeH_2,0, sizeV_2);

    mTexcoords[0] = Vec2f(0,0);
    mTexcoords[1] = Vec2f(0,0);
    mTexcoords[2] = Vec2f(0,0);
    mTexcoords[3] = Vec2f(0,0);
}


void SpeakerView::draw(){
    glColor3f(1,1,1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}


void SpeakerView::update(){}
void SpeakerView::focus(){}
void SpeakerView::unfocus(){}
