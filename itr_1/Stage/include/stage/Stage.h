//
//  World.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_World_h
#define grid_00_World_h

#include <vector>

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/Matrix44.h"
#include "cinder/Vector.h"

#include "Config.h"

#include "util/gl/PingPongFbo.h"
#include "util/gl/FxResources.h"
#include "util/FrustumOrtho.h"

#include "util/LayoutArea.h"
#include "data/quote/QuoteTypesetter.h"
#include "data/quote/json/QuoteJson.h"

#include "stage/Oscillator.h"
#include "stage/bg/Background.h"
#include "stage/grid/Grid.h"
#include "stage/env/Lantern.h"

#include "stage/theme/ThemeView.h"
#include "stage/overlay/NEXTLogo.h"

#include "data/session/Session.h"


/*--------------------------------------------------------------------------------------------*/

namespace next{

    class Stage {
        CameraOrtho  mCamera;
        float        mCameraAspectRatio;
        float        mModelScale;
        Matrix44f    mTransform;
        FrustumOrtho mFrustum;

        Lantern*     mLantern0;
        Lantern*     mLantern1;

        Oscillator*      mOscillator;
        Background*      mBackground;
        Grid*            mGrid;

        QuoteTypesetter* mTypesetter;
        vector<Quote>    mQuotes;

#ifndef STAGE_SKIP_THEME_VIEW
        ThemeView*       mThemeView;
#endif
#ifndef STAGE_SKIP_LOGO
        NEXTLogo*   mLogoNEXT;
#endif
#if defined(STAGE_LIVE_EDIT_FX_SHADER) && !defined(STAGE_SKIP_FX_SHADER)
        FileWatcherRef mFileWatcher;
#endif
#ifndef STAGE_SKIP_FX_SHADER
        gl::Texture      mTextureNoise;

        PingPongFbo      mFboPingPong_1;    //  ping pong fbo with actual screen size
        PingPongFbo      mFboPingPong_2;    //  ping pong fbo with half the screen size
        Area             mFboBounds_1;      //  cache area 1
        Area             mFboBounds_2;      //  cache area 2
        Vec2f            mFboSize_1f;       //  cache size 1 float
        Vec2i            mFboSize_2;        //  cache size 2
        Vec2f            mFboTexelSize_1;   //  cache texelSize 1
        Vec2f            mFboTexelSize_2;   //  cache texelSize 2

        gl::Fbo mFboThemeViewSSAO;      //  original theme view + ssao
        gl::Fbo mFboThemeViewFinal;     //  final theme view completely post-processed

        gl::GlslProg     mShaderNormalDepth;
        gl::GlslProg     mShaderSSAO;
        gl::GlslProgRef  mShaderBlurHRef;
        gl::GlslProgRef  mShaderBlurVRef;
        gl::GlslProg     mShaderMix;
        gl::GlslProg     mShaderMixRadial;

#endif
        Vec2i   mFboSize_1;         //  cache size 1
        gl::Fbo mFboThemeView;      //  original theme view unprocessed


        void drawThemeScene(bool useMaterialShaders);
        void processThemeScene();


        void loadLightProperties();

    public:
        Stage(vector<QuoteJson>* quoteData, Session* sessionData);
        ~Stage();

        void update();
        void draw();

        void zoomModelIn();
        void zoomModelOut();

        void viewTop();
        void viewOrtho();
        void onConfigDidChange();
    };
}

#endif
