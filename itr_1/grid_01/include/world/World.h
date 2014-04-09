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

#include "util/PingPongFbo.h"
#include "util/FrustumOrtho.h"


#include "layout/geom/LayoutArea.h"
#include "layout/quote/QuoteTypesetter.h"
#include "layout/quote/json/QuoteJson.h"

#include "cinder/Matrix44.h"
#include "cinder/Vector.h"

#include "Config.h"

#include "world/Oscillator.h"
#include "world/bg/Background.h"
#include "world/grid/Grid.h"
#include "world/board/Board.h"
#include "world/env/Lantern.h"



/*--------------------------------------------------------------------------------------------*/

typedef std::shared_ptr<class World> WorldRef;


class World {
    CameraOrtho  mCamera;
    float        mCameraAspectRatio;
    float        mModelScale;
    Matrix44f    mTransform;
    FrustumOrtho mFrustum;
    
    Lantern*     mLantern;
    bool         mLanternDebugDraw;
    
    Oscillator*      mOscillator;
    Background*      mBackground;
    Grid*            mGrid;
    Board*           mBoard;
    QuoteTypesetter* mTypesetter;
    vector<Quote>    mQuotes;
    
#if defined(WORLD_LIVE_EDIT_FX_SHADER) && !defined(WORLD_SKIP_FX_SHADER)
    SharedFileWatcherRef mSharedFileWatcher;
#endif
#ifndef WORLD_SKIP_FX_SHADER
    gl::Texture      mTextureNoise;
    
    PingPongFbo      mFboPingPong_1;    //  ping pong fbo with actual screen size
    PingPongFbo      mFboPingPong_2;    //  ping pong fbo with half the screen size
    Area             mFboBounds_1;      //  cache area 1
    Area             mFboBounds_2;      //  cache area 2
    Vec2f            mFboSize_1f;       //  cache size 1 float
    Vec2i            mFboSize_2;        //  cache size 2
    Vec2f            mFboTexelSize_1;   //  cache texelSize 1
    Vec2f            mFboTexelSize_2;   //  cache texelSize 2

    gl::Fbo          mFboSceneSSAO;     //  original scene + ssao
    gl::Fbo          mFboSceneFinal;    //  final scene completely post-processed

    gl::GlslProg     mShaderNormalDepth;
    gl::GlslProg     mShaderSSAO;
    gl::GlslProg     mShaderBlurH;
    gl::GlslProg     mShaderBlurV;
    gl::GlslProg     mShaderMix;
    gl::GlslProg     mShaderMixRadial;

#endif
    Vec2i            mFboSize_1;        //  cache size 1
    gl::Fbo          mFboScene;         //  original scene

    void drawScene(bool useMaterialShaders);
    void processScene();
    
    void loadLightProperties();
    
public:
    World(const vector<QuoteJson>& quoteData);
    ~World();
    
    void update();
    void draw();
    
    void zoomModelIn();
    void zoomModelOut();
    
    void viewTop();
    void viewOrtho();
    void onConfigDidChange();
    
    //! stage leaves info layer, enters quote layer
    void wakeUp();
    //! stage dims down, enters info layer
    void tearDown();
    
    inline static WorldRef create(const vector<QuoteJson>& quoteData){
        return std::make_shared<World>(quoteData);
    }
};

#endif
