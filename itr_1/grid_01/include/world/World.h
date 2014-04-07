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

#include "debug/VboObj.h"


/*--------------------------------------------------------------------------------------------*/

typedef std::shared_ptr<class World> WorldRef;


class World {
    CameraOrtho  mCamera;
    float        mCameraAspectRatio;
    float        mModelScale;
    Matrix44f    mTransform;
    FrustumOrtho mFrustum;
    
    Oscillator*      mOscillator;
    Background*      mBackground;
    Grid*            mGrid;
    Board*           mBoard;
    QuoteTypesetter* mTypesetter;
    vector<Quote>    mQuotes;
    
#ifdef WORLD_LIVE_EDIT_FX_SHADER
    SharedFileWatcherRef mSharedFileWatcher;
#endif
    
    Vec2i            mWindowSize;
    Vec2i            mWindowSize_2;
    Vec2f            mTexelSize;
    Vec2f            mTexelSize_2;
    
    gl::Texture      mTextureNoise;
    gl::Texture      mTextureRandom;
    
    gl::Fbo          mFboScene;
    gl::Fbo          mFboNormalDepth;
    gl::Fbo          mFboSSAO;
    gl::Fbo          mFboNormal;
    gl::Fbo          mFboBlurH;
    gl::Fbo          mFboBlurV;
    gl::Fbo          mFboMix;
    
    PingPongFbo      mFboFx;
    
    gl::GlslProg     mShaderNormal;
    gl::GlslProg     mShaderNormalDepth;
    gl::GlslProg     mShaderSSAO;
    gl::GlslProg     mShaderBlurH;
    gl::GlslProg     mShaderBlurV;
    gl::GlslProg     mShaderMix;
    
    
    void drawScene(bool useMaterialShaders);
    void renderFboNormal();
    void renderFboNormalDepth();
    void renderFboScene();
    void renderFboSSAO();
    void renderFboBlur();
    void renderFboFinal();
    
public:
    World(const vector<QuoteJson>& quoteData);
    ~World();
    
    void update();
    void draw();
    
    void zoomModelIn();
    void zoomModelOut();
    
    void viewTop();
    void viewOrtho();
    
    inline static WorldRef create(const vector<QuoteJson>& quoteData){
        return std::make_shared<World>(quoteData);
    }
};

#endif
