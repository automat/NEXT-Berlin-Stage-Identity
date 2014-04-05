#include "world/World.h"
#include "cinder/Plane.h"
#include "util/GeomUtil.h"
#include "world/board/path/PathSurface.h"
#include "layout/quote/QuoteAlign.h"
#include "layout/quote/Quote.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "Resources.h"

using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/


World::World(const vector<QuoteJson>& quoteData){
    /*--------------------------------------------------------------------------------------------*/
    //  View
    /*--------------------------------------------------------------------------------------------*/

    mCameraAspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM, mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM,
                     -WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_NEAR_CLIP, WORLD_MODEL_CAM_FAR_CLIP);
    viewOrtho();
    
    mModelScale = WORLD_MODEL_SCALE;
    mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    mFrustum.set(mCamera);
    
    
    /*--------------------------------------------------------------------------------------------*/
    //  Environment
    /*--------------------------------------------------------------------------------------------*/

    mGrid = new Grid(WORLD_GRID_NUM_CELLS_XY,WORLD_GRID_NUM_CELLS_XY);
    
    // get Area from initial frustum
    const vector<Vec3f>& frPlaneNear = mFrustum.getNearPlane();
    const vector<Vec3f>& frPlaneFar  = mFrustum.getFarPlane();
    Planef xzPlane(Vec3f::zero(), Vec3f::yAxis());
    Vec3f tl,tr,bl,br;
    utils::getIntersection(xzPlane, frPlaneNear[0], frPlaneFar[0], &tl);
    utils::getIntersection(xzPlane, frPlaneNear[1], frPlaneFar[1], &tr);
    utils::getIntersection(xzPlane, frPlaneNear[2], frPlaneFar[2], &br);
    utils::getIntersection(xzPlane, frPlaneNear[3], frPlaneFar[3], &bl);
    
    LayoutArea area(tl,tr,bl,br);
    LayoutArea areaScaled;
    
    area       *= mTransform.inverted();
    areaScaled  = area;
    areaScaled *= Matrix44f::createScale(Vec3f(1.125f,1.125f,1.125f));
    
    //  Typesetter init
    
    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),WORLD_TYPESETTER_FONT_SIZE), WORLD_TYPESETTER_FONT_SCALE);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(true);
#ifdef DEBUG_WORLD_TYPESETTER_TEXCOORDS
    mTypesetter->debugTexture();
#endif
    
    for(auto& data : quoteData){
        const QuoteJson::Format& format = data.format;
        
        mTypesetter->balanceBaseline(format.balance);
        mTypesetter->setAlign(format.align);
        mTypesetter->setPadding(format.padding[0],format.padding[1],format.padding[2],format.padding[3]);
        mTypesetter->setFontScale(format.scale);
        mTypesetter->setString(data.str);
        mQuotes += *mTypesetter->getQuote();
    }
    
    int windowWidth  = app::getWindowWidth();
    int windowHeight = app::getWindowHeight();
    
    mOscillator = new Oscillator();
    mBackground = new Background(mGrid, areaScaled, mOscillator, app::getWindowWidth(), app::getWindowHeight());
    mBoard      = new Board(mGrid, areaScaled, mOscillator, &mQuotes);

    
    /*--------------------------------------------------------------------------------------------*/
    //  Fbo + Post Process
    /*--------------------------------------------------------------------------------------------*/

    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(8);
    mFboScene = gl::Fbo(windowWidth, windowHeight, fboFormat);
    
}

/*--------------------------------------------------------------------------------------------*/
//  Destructor
/*--------------------------------------------------------------------------------------------*/

World::~World(){
    delete mBackground;
    delete mBoard;
    delete mOscillator;
    delete mTypesetter;
    delete mGrid;
    
    cout << "World destructed." << endl;
}

/*--------------------------------------------------------------------------------------------*/
//  Draw / Update
/*--------------------------------------------------------------------------------------------*/


void World::drawScene(){
    mFboScene.bindFramebuffer();
    gl::enableDepthRead();
    
    gl::clear(Color(0,0,0));
    gl::pushMatrices();
    gl::setMatrices(mCamera);
    
    mBackground->draw();
    
#ifdef DEBUG_WORLD_CAM_FRUSTUM
    mFrustum.draw();
#endif
    glMultMatrixf(&mTransform[0]);
    
#ifdef DEBUG_WORLD_COORDINATE_FRAME
    gl::drawCoordinateFrame();
#endif

    mBoard->draw(mCamera);

#ifdef DEBUG_WORLD_GRID_DRAW_INDICES
    gl::disableDepthRead();
    mGrid->debugDrawIndices(mCamera);
    gl::enableDepthRead();
#endif
    gl::popMatrices();
    gl::disableDepthRead();
    
    mFboScene.unbindFramebuffer();
}

void World::update(){
    mBoard->update();
}

void World::draw(){
    const static Vec2i windowSize(app::getWindowSize());
    drawScene();
    
    gl::pushMatrices();
    gl::setMatricesWindow(windowSize, false);
    gl::disableDepthRead();
    glColor3f(1,1,1);
    gl::draw(mFboScene.getTexture(),mFboScene.getBounds());

#ifdef DEBUG_WORLD_TYPESETTER_TEXTURE
    gl::setMatricesWindow(windowSize);
    const Quote* quote = mBoard->getCurrentQuote();
    if(quote != nullptr){
        gl::draw(quote->getTexture(),Rectf(windowSize.x - 256, 0, windowSize.x, 256));
    }
#endif
    
    gl::popMatrices();
}


/*--------------------------------------------------------------------------------------------*/
//  View
/*--------------------------------------------------------------------------------------------*/

void World::zoomModelIn(){
    mModelScale = MIN(WORLD_MODEL_SCALE_MIN, mModelScale * 2);
    mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
}

void World::zoomModelOut(){
    mModelScale = MIN(mModelScale / 2, WORLD_MODEL_SCALE_MAX);
    mTransform = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
}

void World::viewTop(){
    mCamera.lookAt(Vec3f(0,1,0), Vec3f::zero());
}

void World::viewOrtho(){
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
}