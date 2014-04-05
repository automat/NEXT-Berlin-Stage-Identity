//
//  Background.h
//  grid_00
//
//  Created by Henryk Wollik on 29/03/14.
//
//

#ifndef grid_00_Background_h
#define grid_00_Background_h

#include "layout/geom/LayoutArea.h"
#include "world/grid/Grid.h"
#include "world/Oscillator.h"

#include "cinder/TriMesh.h"
#include "cinder/gl/Vbo.h"
#include "cinder/TriMesh.h"
#include "cinder/Matrix44.h"

#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/GlslProg.h"

#include "Config.h"

#ifdef BACKGROUND_LIVE_EDIT_SHADER
#include "util/SharedFileWatcher.h"
#endif

using namespace ci;
class Background {
    Oscillator*  mOsc;
    TriMesh      mMesh;
    Matrix44f    mTransform;

    gl::Fbo      mFboGradient;
    gl::GlslProg mShaderGradient;
    
    gl::Fbo      mFboMesh;
    gl::GlslProg mShaderMesh;
    
    gl::Fbo      mFboMix;
    gl::GlslProg mShaderMix;
    
    
    bool         mTextureIsDirty;
    gl::Texture  mTexture; // resulting texture
    
#ifdef BACKGROUND_LIVE_EDIT_SHADER
    SharedFileWatcherRef mSharedFileWatcher;
#endif
    
    void renderGradient();
    void renderMesh();
    void renderTexture();
    
public:
    Background(Grid* grid, const LayoutArea& area, Oscillator* osc, int width, int height);
    
    void draw();
    void update();

};

#endif
