//
// Created by Henryk Wollik on 14/04/14.
//

#include "SpeakerView.h"
#include <OpenGL/OpenGL.h>
#include "cinder/gl/gl.h"
#include <iostream>
#include <math.h>

#include "FxResources.h"

using namespace std;



/*--------------------------------------------------------------------------------------------*/
//  Const coords texture facing orthographic camera
/*--------------------------------------------------------------------------------------------*/

// for calc see: https://github.com/preciousforever/NEXT-Everything/blob/ae1c84c506b1d06c590cc32cf6680075e15c4b93/itr_1/SpeakerImageFilter/src/SpeakerImageFilterApp.cpp
const Vec2f SpeakerView::sTexCoordsNorm[4] = {
        Vec2f(0.5, 0),
        Vec2f(1.0, 0.28867516),
        Vec2f(0,   0.28867516),
        Vec2f(0.5, 0.57735032)
};


/*--------------------------------------------------------------------------------------------*/
//  Const geometry props
/*--------------------------------------------------------------------------------------------*/

const Vec2f SpeakerView::sSize(2,0.065f);

const Vec3f SpeakerView::sCubeVertices[8] = {
        Vec3f(-sSize.x * 0.5, sSize.y * 0.5f,-sSize.x * 0.5),  //  tl
        Vec3f( sSize.x * 0.5, sSize.y * 0.5f,-sSize.x * 0.5),  //  tr
        Vec3f(-sSize.x * 0.5, sSize.y * 0.5f, sSize.x * 0.5),  //  bl
        Vec3f( sSize.x * 0.5, sSize.y * 0.5f, sSize.x * 0.5),  //  br

        Vec3f(-sSize.x * 0.5,-sSize.y * 0.5f,-sSize.x * 0.5),  //  tl
        Vec3f( sSize.x * 0.5,-sSize.y * 0.5f,-sSize.x * 0.5),  //  tr
        Vec3f(-sSize.x * 0.5,-sSize.y * 0.5f, sSize.x * 0.5),  //  bl
        Vec3f( sSize.x * 0.5,-sSize.y * 0.5f, sSize.x * 0.5)  //  br
};


const size_t SpeakerView::sCardVerticesLen = 6 * 3;
const Vec3f SpeakerView::sCardVertices[18] = {
        //  top
        sCubeVertices[0],
        sCubeVertices[1],
        sCubeVertices[3],
        sCubeVertices[3],
        sCubeVertices[0],
        sCubeVertices[2],

        //  right
        sCubeVertices[3],
        sCubeVertices[1],
        sCubeVertices[5],
        sCubeVertices[5],
        sCubeVertices[3],
        sCubeVertices[7],

        // bottom
        sCubeVertices[2],
        sCubeVertices[3],
        sCubeVertices[7],
        sCubeVertices[7],
        sCubeVertices[2],
        sCubeVertices[6]

};

static const Vec3f xAxis(Vec3f::xAxis());
static const Vec3f yAxis(Vec3f::yAxis());
static const Vec3f zAxis(Vec3f::zAxis());

const Vec3f SpeakerView::sCardNormals[18] = {
        yAxis,yAxis,yAxis,yAxis,yAxis,yAxis,
        xAxis,xAxis,xAxis,xAxis,xAxis,xAxis,
        zAxis,zAxis,zAxis,zAxis,zAxis,zAxis
};


/*--------------------------------------------------------------------------------------------*/
//  SpeakerView
/*--------------------------------------------------------------------------------------------*/

SpeakerView::SpeakerView(Speaker* data) :
mData(data){
    //
    //  Setup Fbo
    //
    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(4);
    
    mImageSize = data->getImageRef().getSize();
    
    int imageWidth  = mImageSize.x;
    int imageHeight = mImageSize.y;
    
    mFbo0 = gl::Fbo(imageWidth,imageHeight,fboFormat);
    mFbo1 = gl::Fbo(imageWidth,imageHeight,fboFormat);
    
    mShaderBlurHRef = FxResources::GetBlurH();
    mShaderBlurVRef = FxResources::GetBlurV();
    
    //
    //  recalc camera-faced quad coords according to speaker image
    //
    float aspectRatio = data->getImageRef().getAspectRatio();
    Vec2f scalar      = Vec2f(1,aspectRatio);
    Vec2f offset      = Vec2f(0,(1 - sTexCoordsNorm[3].y) * 0.5f) * 0.5f;
    
    mTexcoordsNorm[0] = sTexCoordsNorm[0] * scalar + offset; // tl
    mTexcoordsNorm[1] = sTexCoordsNorm[1] * scalar + offset; // tr
    mTexcoordsNorm[2] = sTexCoordsNorm[2] * scalar + offset; // bl
    mTexcoordsNorm[3] = sTexCoordsNorm[3] * scalar + offset; // br
    
    mTexelSize.x = 0.5f / static_cast<float>(imageWidth);
    mTexelSize.y = 0.5f / static_cast<float>(imageHeight);
    
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
    
    setPosition(Vec3f::zero());
    setScale(1.0f);
    
        drawFocus(1.0f);
}

SpeakerView::~SpeakerView(){
    //delete mFboPingPong;
}


void SpeakerView::drawFocus(float factor){
    const static float scale = 6.0f;
    float factorInv = 1.0f - factor;
    
    const gl::Texture& image = mData->getImageRef();
    
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setViewport(image.getBounds());
    gl::pushMatrices();
    gl::setMatricesWindow(image.getSize(), false);
    
    mFbo0.bindFramebuffer();
    gl::clear();
    glColor3f(1,1,1);
    gl::draw(image);
    mFbo0.unbindFramebuffer();

    mFbo1.bindFramebuffer();
    mShaderBlurHRef->bind();
    mShaderBlurHRef->uniform("uTexture", 0);
    mShaderBlurHRef->uniform("uTexelSize", mTexelSize.x);
    mShaderBlurHRef->uniform("uScale", scale * factorInv);
    gl::clear();
    glColor3f(1,1,1);
    gl::draw(mFbo0.getTexture());
    mShaderBlurHRef->unbind();
    mFbo1.unbindFramebuffer();
    
    
    mFbo0.bindFramebuffer();
    mShaderBlurVRef->bind();
    mShaderBlurVRef->uniform("uTexture", 0);
    mShaderBlurVRef->uniform("uTexelSize", mTexelSize.y);
    mShaderBlurVRef->uniform("uScale", scale * factorInv);
    gl::clear();
    glColor3f(1,1,1);
    gl::draw(mFbo1.getTexture());
    mShaderBlurHRef->unbind();
    mFbo0.unbindFramebuffer();
    
    mFbo1.bindFramebuffer();
    gl::clear();
    glColor3f(0.87450980392157f * factor + 0.0f * factorInv,
              0.06274509803922f * factor + 0.39607843137255f * factorInv,
              0.39607843137255f * factor + 0.89019607843137f * factorInv);
    gl::draw(mFbo0.getTexture());
    mFbo1.unbindFramebuffer();
    
    
    gl::popMatrices();
    glPopAttrib();
}

/*--------------------------------------------------------------------------------------------*/
//  Draw / Update
/*--------------------------------------------------------------------------------------------*/

void SpeakerView::draw(){
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    
    glEnableClientState(GL_VERTEX_ARRAY);
   
    glColor3f(1,1,1);
    mFbo1.getTexture().enableAndBind();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glNormalPointer(   3, GL_FLOAT,    &sCardNormals[0]);
    glTexCoordPointer( 2, GL_FLOAT, 0, &mTexcoords[0]);
    glVertexPointer(   3, GL_FLOAT, 0, &sCardVertices[0]);
    glDrawArrays(GL_TRIANGLES, 0,sCardVerticesLen);

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    mFbo1.unbindTexture();
    
    glColor4f(1,1,1,0.65f);
    static int indices[4] = {0,1,3,2};
    glVertexPointer(3, GL_FLOAT, 0, &sCubeVertices[0]);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}


void SpeakerView::update(){}
void SpeakerView::focus(){}
void SpeakerView::unfocus(){}
