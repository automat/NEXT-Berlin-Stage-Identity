//
//  Background.h
//  grid_00
//
//  Created by Henryk Wollik on 29/03/14.
//
//

#ifndef grid_00_Background_h
#define grid_00_Background_h

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
#include "util/FileWatcher.h"
#endif

#include "layout/geom/LayoutArea.h"
#include "stage/grid/Grid.h"
#include "stage/Oscillator.h"

using namespace ci;
class Background {
    Oscillator*  mOsc;
    TriMesh      mMesh;
    Matrix44f    mTransform;
    
    gl::GlslProg mShaderMesh;
    
#ifdef BACKGROUND_LIVE_EDIT_SHADER
    FileWatcherRef mFileWatcher;
#endif
    
public:
    Background(Grid* grid, const LayoutArea& area, Oscillator* osc, int width, int height);
    void draw();
    void update(Oscillator* osc, float t);
};

#endif