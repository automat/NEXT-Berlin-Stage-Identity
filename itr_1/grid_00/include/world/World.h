//
//  World.h
//  grid_00
//
//  Created by Henryk Wollik on 28/03/14.
//
//

#ifndef grid_00_World_h
#define grid_00_World_h

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "util/FrustumOrtho.h"

#include "util/Utils.h"
#include "layout/geom/LayoutArea.h"
#include "layout/quote/QuoteTypesetter.h"


#include "cinder/Matrix44.h"
#include "cinder/Vector.h"

#include "Config.h"

#include "world/Oscillator.h"
#include "world/grid/Grid.h"
#include "world/board/Board.h"

/*--------------------------------------------------------------------------------------------*/

typedef std::shared_ptr<class World> WorldRef;

/*--------------------------------------------------------------------------------------------*/

using namespace ci;
class World {
    
    CameraOrtho  mCamera;
    float        mCameraAspectRatio;
    float        mModelScale;
    Matrix44f    mTransform;
    FrustumOrtho mFrustum;
    
    LayoutArea       mArea;
    LayoutArea       mAreaN; // area unscaled
    Grid*            mGrid;
    Board*           mBoard;
    Oscillator*      mOscillator;
    QuoteTypesetter* mTypesetter;
    
    void initCells();
    void drawScene();

public:
    
    World();
    ~World();
    
    void update();
    void draw();
    
    void zoomModelIn();
    void zoomModelOut();
    
    void viewTop();
    void viewOrtho();

    /*--------------------------------------------------------------------------------------------*/

    inline static WorldRef create(){
        return std::make_shared<World>();
    }
};

#endif
