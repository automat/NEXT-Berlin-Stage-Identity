//
// Created by Henryk Wollik on 14/04/14.
//

#include "SpeakerView.h"
#include <OpenGL/OpenGL.h>
#include <iostream>
#include <math.h>

using namespace std;

const Vec2f SpeakerView::TEX_COORDS_NORM[4] = {
        Vec2f(0.5, 0),
        Vec2f(1.0, 0.28867516),
        Vec2f(0,   0.28867516),
        Vec2f(0.5, 0.57735032)
};

SpeakerView::SpeakerView() :
    mSize(1,1){

    float sizeH_2 = mSize.x * 0.5f;
    float sizeV_2 = mSize.y * 0.5f;

    mPoints[0] = Vec3f(-sizeH_2,0,-sizeV_2);
    mPoints[1] = Vec3f( sizeH_2,0,-sizeV_2);
    mPoints[2] = Vec3f(-sizeH_2,0, sizeV_2);
    mPoints[3] = Vec3f( sizeH_2,0, sizeV_2);

    mTexture = gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SpeakerImageFilter/resources/26395.jpg"));

    float textureHeight      = mTexture.getHeight();
    float textureWidth       = mTexture.getWidth();

    float aspectRatio;
    Vec2f scalar;
    Vec2f offset;

    if(textureWidth < textureHeight){
        aspectRatio = textureWidth / textureHeight;
        scalar = Vec2f(1,aspectRatio);
        offset = Vec2f(0,(textureHeight - abs(TEX_COORDS_NORM[0].y + TEX_COORDS_NORM[3].y)/*unit height*/ * textureHeight) / textureHeight);
    } else {
        aspectRatio = textureHeight / textureWidth;
        scalar = Vec2f(aspectRatio,1);
        //offset = Vec2f(0,0);
        offset = Vec2f((textureWidth - abs(TEX_COORDS_NORM[2].x + TEX_COORDS_NORM[1].x)/*unit width*/ * textureWidth) / textureWidth,0);
    }

    mTexcoords[0] = TEX_COORDS_NORM[0] * scalar + offset; // tl
    mTexcoords[1] = TEX_COORDS_NORM[1] * scalar + offset; // tr
    mTexcoords[2] = TEX_COORDS_NORM[2] * scalar + offset; // bl
    mTexcoords[3] = TEX_COORDS_NORM[3] * scalar + offset; // br
}


void SpeakerView::draw(){
    glColor3f(1,1,1);

    mTexture.enableAndBind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &mPoints[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &mTexcoords[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    mTexture.disable();
}


void SpeakerView::update(){}
void SpeakerView::focus(){}
void SpeakerView::unfocus(){}
