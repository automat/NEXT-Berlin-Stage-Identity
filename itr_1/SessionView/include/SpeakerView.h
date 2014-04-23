//
// Created by Henryk Wollik on 14/04/14.
//

#ifndef __SpeakerView_H_
#define __SpeakerView_H_

#include <OpenGL/OpenGL.h>

#include "cinder/Color.h"
#include "cinder/Matrix44.h"
#include "cinder/Vector.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"

#include "cinder/Timeline.h"

#include "AbstractAnimView.h"
#include "Speaker.h"


using namespace ci;
namespace next {
    class SpeakerView : public AbstractAnimView{
        friend class SpeakerStackView;

        //
        // shared
        //
        const static Vec2f  sTexCoordsNorm[4];
        const static Vec3f  sCubeVertices[8];
        const static size_t sCardVerticesLen;
        const static Vec3f  sCardVertices[18];
        const static Vec3f  sCardNormals[18];
        
        Speaker* mData;
        
        Vec2f   mImageSize;           //  cache image size
        Vec2f   mTexcoordsNorm[4];    //  center image texcoords facing ortho cam
        Vec2f   mTexcoords[18];       //  texcoords distributed across unique vertices
        ColorAf mVertexColors[18];    //  vertex colors;
        Vec2f   mTexelSize;           //  texel size for fx shader
        
        gl::GlslProgRef mShaderBlurHRef;    //  ref fx resource blur h
        gl::GlslProgRef mShaderBlurVRef;    //  ref fx resource blur v
        
        gl::Fbo mFbo0;
        gl::Fbo mFbo1;
        
        Anim<float> mIntrplState;
        Anim<float> mColorState;
        Anim<Vec3f> mPositionState;
        Anim<float> mScaleState;
        
        void drawFocus(float factorFocus = 1.0f, float factorColor = 1.0f);   //   0 = unfocused, 1 = focused

    public:
        SpeakerView(Speaker* data);
        ~SpeakerView();
        
        void draw();

        void updateFocusState();    //  focus color deactive -> active
        void updateFocusImage();    //  focus image blur -> unblur
        void updateColorState();
        
        void unfocusImage();
    };
}

#endif //__SpeakerView_H_
