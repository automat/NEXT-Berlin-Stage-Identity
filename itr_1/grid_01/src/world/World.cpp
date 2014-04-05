#include "world/World.h"
#include "cinder/Plane.h"
#include "util/GeomUtil.h"
#include "world/board/path/PathSurface.h"
#include "layout/quote/QuoteAlign.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/

World::World(const vector<QuoteJson>& quoteData){
    
    /*--------------------------------------------------------------------------------------------*/
    // Camera
    /*--------------------------------------------------------------------------------------------*/
    
    mCameraAspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM, mCameraAspectRatio * WORLD_MODEL_CAM_ZOOM,
                     -WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_ZOOM, WORLD_MODEL_CAM_NEAR_CLIP, WORLD_MODEL_CAM_FAR_CLIP);
    viewOrtho();
    
    mModelScale = WORLD_MODEL_SCALE;
    mTransform  = Matrix44f::createScale(Vec3f(mModelScale,mModelScale,mModelScale));
    mFrustum.set(mCamera);
    
   
    /*--------------------------------------------------------------------------------------------*/
    // Environment
    /*--------------------------------------------------------------------------------------------*/

    
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
    
    LayoutArea area(tl,tr,bl,br);
    LayoutArea areaScaled;
    
    area       *= mTransform.inverted();
    areaScaled  = area;
    areaScaled *= Matrix44f::createScale(Vec3f(1.125f,1.125f,1.125f));
    
    /*--------------------------------------------------------------------------------------------*/
    // Typesetter
    /*--------------------------------------------------------------------------------------------*/

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
    
    /*--------------------------------------------------------------------------------------------*/
    // Init layers
    /*--------------------------------------------------------------------------------------------*/

    mOscillator = new Oscillator();
    mBackground = new Background(mGrid, areaScaled, mOscillator, app::getWindowWidth(), app::getWindowHeight());
    mBoard      = new Board(mGrid, areaScaled, &mQuotes);
    
    mFboRender = gl::Fbo(app::getWindowWidth(),app::getWindowHeight());
    
}

World::~World(){
    delete mBoard;
    delete mBackground;
    delete mOscillator;
    delete mTypesetter;
    delete mGrid;
    
    cout << "World destructed." << endl;
}

/*--------------------------------------------------------------------------------------------*/
// Quote handling
/*--------------------------------------------------------------------------------------------*/

void World::playPrevQuote(){
    
}

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
    
    glColor3f(0.5f,0,0.125f);
    glVertexPointer(3, GL_FLOAT, 0, &mTypesetter->getArea().getTL().x);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
    
    glColor3f(1.0f,0,0.125f);
    glVertexPointer(3, GL_FLOAT, 0, &mBoard->getArea().getTL().x);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, &indices[0]);
    
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

#ifdef DEBUG_WORLD_CAM_FRUSTUM
    mFrustum.draw();
#endif
    
    glPushMatrix();
    glMultMatrixf(&mTransform[0]);
    drawScene();
    
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
    
    glPopMatrix();
    
#ifdef DEBUG_WORLD_TYPESETTER_TEXTURE
    const Quote* currentQuote = mBoard->getCurrentQuote();
    if(currentQuote != nullptr){
        gl::disableDepthRead();
        gl::pushMatrices();
        gl::setMatricesWindow(app::getWindowSize(),true);
        static int textureSize = 256;
        static Rectf pos(app::getWindowWidth() - textureSize,0,app::getWindowWidth(),textureSize);
        glColor3f(1,1,1);
        gl::draw(currentQuote->getTexture(),pos);
        gl::popMatrices();
        gl::enableDepthRead();
    }
#endif
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
