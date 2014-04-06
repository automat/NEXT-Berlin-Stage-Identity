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
    
    gl::Fbo          mFboScene;
    gl::Fbo          mFboFxTiltShift;
    
    void drawScene();
    
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
