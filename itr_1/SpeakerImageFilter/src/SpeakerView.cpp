//
// Created by Henryk Wollik on 14/04/14.
//

#include "SpeakerView.h"
#include <OpenGL/OpenGL.h>
#include <iostream>
#include <math.h>

using namespace std;

/*--------------------------------------------------------------------------------------------*/
//  Const coords texture facing orthographic camera
/*--------------------------------------------------------------------------------------------*/

// for calc see: SpeakerImageFilter
const Vec2f texCoordsNorm[4] = {
        Vec2f(0.5, 0),
        Vec2f(1.0, 0.28867516),
        Vec2f(0,   0.28867516),
        Vec2f(0.5, 0.57735032)
};


/*--------------------------------------------------------------------------------------------*/
//  Const geometry props
/*--------------------------------------------------------------------------------------------*/

const float cubeHeight_2 = 0.025f;

const Vec3f cubeVertices[8] = {
        Vec3f(-0.5, cubeHeight_2,-0.5),  //  tl
        Vec3f( 0.5, cubeHeight_2,-0.5),  //  tr
        Vec3f(-0.5, cubeHeight_2, 0.5),  //  bl
        Vec3f( 0.5, cubeHeight_2, 0.5),  //  br

        Vec3f(-0.5,-cubeHeight_2,-0.5),  //  tl
        Vec3f( 0.5,-cubeHeight_2,-0.5),  //  tr
        Vec3f(-0.5,-cubeHeight_2, 0.5),  //  bl
        Vec3f( 0.5,-cubeHeight_2, 0.5)  //  br
};


const size_t cardVerticesLen = 6 * 3;

const Vec3f cardVertices[cardVerticesLen] = {
        //  top
        cubeVertices[0],
        cubeVertices[1],
        cubeVertices[3],
        cubeVertices[3],
        cubeVertices[0],
        cubeVertices[2],

        //  right
        cubeVertices[3],
        cubeVertices[1],
        cubeVertices[5],
        cubeVertices[5],
        cubeVertices[3],
        cubeVertices[7],

        // bottom
        cubeVertices[2],
        cubeVertices[3],
        cubeVertices[7],
        cubeVertices[7],
        cubeVertices[2],
        cubeVertices[6]

};

const Vec3f xAxis(Vec3f::xAxis());
const Vec3f yAxis(Vec3f::yAxis());
const Vec3f zAxis(Vec3f::zAxis());

const Vec3f cardNormals[cardVerticesLen] = {
        yAxis,yAxis,yAxis,yAxis,yAxis,yAxis,
        xAxis,xAxis,xAxis,xAxis,xAxis,xAxis,
        zAxis,zAxis,zAxis,zAxis,zAxis,zAxis
};


/*--------------------------------------------------------------------------------------------*/
//  SpeakerView
/*--------------------------------------------------------------------------------------------*/

SpeakerView::SpeakerView(Speaker* data) :
    mData(data),
    mSize(2,2){

    //
    //  recalc camera-faced quad coords according to speaker image
    //
    float aspectRatio = data->getImageRef().getAspectRatio();
    Vec2f scalar      = Vec2f(1,aspectRatio);
    Vec2f offset      = Vec2f(0,(1 - texCoordsNorm[3].y) * 0.5f) * 0.5f;

    mTexcoordsNorm[0] = texCoordsNorm[0] * scalar + offset; // tl
    mTexcoordsNorm[1] = texCoordsNorm[1] * scalar + offset; // tr
    mTexcoordsNorm[2] = texCoordsNorm[2] * scalar + offset; // bl
    mTexcoordsNorm[3] = texCoordsNorm[3] * scalar + offset; // br

    //
    //  distripute to unique vertices
    //
    mTexcoords[ 0] = mTexcoordsNorm[0];
    mTexcoords[ 1] = mTexcoordsNorm[1];
    mTexcoords[ 2] = mTexcoordsNorm[3];
    mTexcoords[ 3] = mTexcoordsNorm[3];
    mTexcoords[ 4] = mTexcoordsNorm[0];
    mTexcoords[ 5] = mTexcoordsNorm[2];

    mTexcoords[ 6] = mTexcoordsNorm[3];
    mTexcoords[ 7] = mTexcoordsNorm[1];
    mTexcoords[ 8] = mTexcoordsNorm[1];
    mTexcoords[ 9] = mTexcoordsNorm[1];
    mTexcoords[10] = mTexcoordsNorm[3];
    mTexcoords[11] = mTexcoordsNorm[3];

    mTexcoords[12] = mTexcoordsNorm[2];
    mTexcoords[13] = mTexcoordsNorm[3];
    mTexcoords[14] = mTexcoordsNorm[3];
    mTexcoords[15] = mTexcoordsNorm[3];
    mTexcoords[16] = mTexcoordsNorm[2];
    mTexcoords[17] = mTexcoordsNorm[2];
}

/*--------------------------------------------------------------------------------------------*/
//  Draw / Update
/*--------------------------------------------------------------------------------------------*/

void SpeakerView::draw(){
    glColor3f(1,1,1);

    glPushMatrix();
    glScalef(mSize.x, 1, mSize.y);
    glPointSize(5);
    glColor3f(1,1,1);

    mData->getImageRef().enableAndBind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, 0, &mTexcoords[0]);
    glVertexPointer(3, GL_FLOAT, 0, &cardVertices[0]);
    glColorPointer(3, GL_FLOAT, 0, &cardNormals[0]);

    glDrawArrays(GL_TRIANGLES, 0,cardVerticesLen);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    mData->getImageRef().disable();

    glPopMatrix();

}


void SpeakerView::update(){}
void SpeakerView::focus(){}
void SpeakerView::unfocus(){}
