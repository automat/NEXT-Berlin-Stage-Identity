#include "world/World.h"
#include "cinder/Plane.h"
#include "util/GeomUtil.h"
#include "world/board/path/PathSurface.h"
#include "layout/quote/QuoteAlign.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/
// Constructor
/*--------------------------------------------------------------------------------------------*/

World::World(const vector<QuoteJson>& quoteData){
    //
    //  Camera
    //
    mCameraAspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM, mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM,
                     -WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_NEAR_CLIP, WORLD_MODEL_CAM_FAR_CLIP);
    viewOrtho();
    
    mModelScale = WORLD_MODEL_SCALE;
    mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    mFrustum.set(mCamera);
    
    
    //
    //  Environment
    //
    
    //  Init Grid
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
    mArea = LayoutArea(tl,tr,bl,br);
    mArea*= mTransform.inverted();
    mAreaN= mArea;
    mArea*= Matrix44f::createScale(Vec3f(1.125f,1.125f,1.125f)); // scaled to include adjacent cells
    
    //
    //  Typesetter
    //
    mTypesetter = new QuoteTypesetter(mGrid, mAreaN);
    mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),400.0f),0.7f);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(true);
    mTypesetter->setAlign(QuoteAlign::CENTER);
    mTypesetter->debugTexture();
    
    for(auto& data : quoteData){
        const QuoteJson::Format& format = data.format;
        
        mTypesetter->balanceBaseline(format.balance);
        mTypesetter->setAlign(format.align);
        mTypesetter->setPadding(format.padding[0],format.padding[1],format.padding[2],format.padding[3]);
        mTypesetter->setFontScale(format.scale);
        mTypesetter->setString(data.str);
        mQuotes += *mTypesetter->getQuote();
    } 

    //  Init osc
    mOscillator = new Oscillator();
    
    //  Init Bg
    mBackground = new Background(mGrid, mArea, mOscillator, app::getWindowWidth(), app::getWindowHeight());
    
    //  Init Board
    mBoard = new Board(mGrid,mArea,&mQuotes);
  
    
    //
    //
    //

    
}

World::~World(){
    delete mBoard;
    delete mBackground;
    delete mOscillator;
    delete mTypesetter;
    //delete mTestSurface;
    delete mGrid;
    
    cout << "World destructed." << endl;
}

/*--------------------------------------------------------------------------------------------*/
// Quote handling
/*--------------------------------------------------------------------------------------------*/

void World::playNextQuote(){
    
}

/*--------------------------------------------------------------------------------------------*/
// Draw Scene
/*--------------------------------------------------------------------------------------------*/


void World::drawScene(){
    mBackground->draw();

#ifdef DEBUG_WORLD_GRID_DRAW_INDICES
    mGrid->debugDrawIndices(mCamera);
#endif
    
#ifdef DEBUG_WORLD_AREA_DRAW
    static const int indices[] = {0,1,3,2};
    glEnableClientState(GL_VERTEX_ARRAY);
    {
        Vec3f vertices[] = {
            mArea.getTL(),
            mArea.getTR(),
            mArea.getBL(),
            mArea.getBR()
        };
        glColor3f(0.5f,0,0.125f);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
    }
    {
        Vec3f vertices[] = {
            mAreaN.getTL(),
            mAreaN.getTR(),
            mAreaN.getBL(),
            mAreaN.getBR()
        };
        glColor3f(1.0f,0,0.125f);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0].x);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
#endif

#ifdef DEBUG_WORLD_GRID_CELL_DRAW
    {
        const vector<Cell*> cells = mGrid->getCells();
        for(vector<Cell*>::const_iterator itr = cells.begin(); itr != cells.end(); ++itr){
            (*itr)->debugDrawArea();
        }
    }
#endif

    mBoard->draw(mCamera);

#ifdef DEBUG_WORLD_TYPESETTER
    gl::disableDepthRead();
    mTypesetter->debugDrawArea();
    gl::enableAlphaTest();
    gl::enableAdditiveBlending();
    mTypesetter->debugDrawString();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    gl::enableDepthRead();
#endif
    
#ifdef DEBUG_WORLD_COORDINATE_FRAME
    gl::drawCoordinateFrame();
#endif



}

/*--------------------------------------------------------------------------------------------*/
// Update / Draw
/*--------------------------------------------------------------------------------------------*/

void World::update(){
    mBackground->update();
    mBoard->update();
}

void World::draw(){
    gl::enableDepthRead();
    gl::setMatrices(mCamera);
/*
#ifdef DEBUG_WORLD_CAM_FRUSTUM
    mFrustum.draw();
#endif
*/
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    drawScene();
 
    glPopMatrix();

    
}

/*--------------------------------------------------------------------------------------------*/
// Model zoom / Camera ortho,top
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
