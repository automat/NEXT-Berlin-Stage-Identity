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
#include "cinder/gl/Material.h"

#include "cinder/Timeline.h"

#include "stage/session/AbstractAnimBase.h"
#include "data/session/Speaker.h"


using namespace ci;
namespace next {
    class SpeakerView : public AbstractAnimBase{
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
        
        Vec2f   mTexcoordsNorm[4];    //  center image texcoords facing ortho cam
        Vec2f   mTexcoords[18];       //  texcoords distributed across unique vertices
        ColorAf mVertexColors[18];    //  vertex colors;
        Vec2f   mTexelSize;           //  texel size for fx shader
        
        gl::GlslProgRef mShaderBlurHRef;    //  ref fx resource blur h
        gl::GlslProgRef mShaderBlurVRef;    //  ref fx resource blur v
        
        gl::Fbo mFbo0;
        gl::Fbo mFbo1;
        
        Anim<Vec3f> mPositionState;
        Anim<float> mScaleState;
        
        Anim<float> mAlphaState;
        Anim<float> mFocusColorState;
        Anim<float> mFocusBlurState;
        
        gl::Material mMaterial;
        
        bool mFboDirty;
        
        void activate();
        void deactivate();
        
        void beginPaint();
        void repaint();
        void endPaint();
        
    public:
        SpeakerView(Speaker* data);
        
        void update();
        void draw();
        
        void unfocusImage();
        void unfocus();
        
        void focus();
        
        void show();
        void hide();
        
        void focusIn();
        void unfocusOut();
        
        void clearStates();
        
        void loadMaterialProperties();
    };
}

#endif //__SpeakerView_H_
