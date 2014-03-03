#include "cinder/app/AppNative.h"
#include "Settings.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include "cinder/Frustum.h"
#include "Controller.h"
#include "FrustumOrtho.h"
#include "InfoPanel.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*--------------------------------------------------------------------------------------------*/

static int   MODEL_ZOOM(WORLD_MODEL_ZOOM_INITIAL);
static float MODEL_SCALE(WORLD_MODEL_SCALE_INITIAL); //0.65

static bool SHOW_INFO(true);

/*--------------------------------------------------------------------------------------------*/

class grid_04App : public AppNative {
public:
    void prepareSettings(Settings* settings);
	void setup();
    void resize();
	void mouseDown( MouseEvent event );
    void keyDown(KeyEvent event);
	void update();
	void draw();
    
    void updateView();
    
    InfoPanel*   mInfoPanel;
    
    //mem
    FrustumOrtho   mFrustum;
    CameraOrtho    mCamera;
    CameraOrtho    mCameraDebug;
    Controller*    mController;
    
    
};

void grid_04App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_04App::setup(){
    mInfoPanel = new InfoPanel(Rectf(0,0,300,app::getWindowHeight()));
    mInfoPanel->setModelScale(&MODEL_SCALE);
    mInfoPanel->setModelZoom(&MODEL_ZOOM);
    
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10, 1.0f);
    mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());
    
    float cameraDebugZoom = 6.0f;
    mCameraDebug.setOrtho(-aspectRatio * cameraDebugZoom, aspectRatio * cameraDebugZoom, -cameraDebugZoom, cameraDebugZoom, -10, 100.0f);
    mCameraDebug.lookAt(Vec3f(0,1,0), Vec3f::zero());
    
    mController     = new Controller(WORLD_NUM_CELLS_XY,WORLD_NUM_CELLS_XY);
    
    gl::enableDepthRead();
    glEnable(GL_SCISSOR_TEST);
}

void grid_04App::mouseDown( MouseEvent event ){
}

void grid_04App::update(){
    mFrustum.set(mCamera,1.1f);
    
#ifdef WORLD_UPDATE
    mController->update(app::getElapsedSeconds());
#endif
    mController->checkFrustum(mFrustum);
    mController->transform(MODEL_SCALE);
    
    
    mInfoPanel->setFps(getAverageFps());
    
    
}

void grid_04App::draw(){
    glScissor(0, 0, app::getWindowWidth(), app::getWindowHeight());
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glEnable(GL_DEPTH_TEST);
    
    glPushMatrix();
    glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
#ifdef WORLD_DRAW
#ifdef WORLD_DEBUG_DRAW_CELL_AREA
    mController->debugArea();
#endif
#ifdef WORLD_DEBUG_DRAW_CELL
    mController->debugDraw();
#endif
#ifdef WORLD_DRAW_CELL
    mController->draw();
#endif
#endif
    glPopMatrix();
    
    
    
    if(SHOW_INFO){
        mInfoPanel->draw();
        //  debug frustum
        glPushAttrib(GL_VIEWPORT_BIT);
        static const float frustumViewBoxWidth = 300;
        float aspectRatio = 1.0f / app::getWindowAspectRatio();
        glScissor(0, 0, frustumViewBoxWidth  ,frustumViewBoxWidth * aspectRatio);
        glViewport(0, 0, frustumViewBoxWidth ,frustumViewBoxWidth * aspectRatio);
        gl::clear(Color(0,0,0));
        gl::setMatrices(mCameraDebug);
        mFrustum.draw();
        
        //draw frustum plane intersection
        glPushMatrix();
        glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
        mController->debugArea();
        glPopMatrix();
        glPopAttrib();
        
    }
    
    
    
    
    
    
}

void grid_04App::keyDown(KeyEvent event){
    
    switch (event.getCode()) {
        case KeyEvent::KEY_5:
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_6:
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_7:
            mCamera.lookAt(Vec3f(-1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_8:
            mCamera.lookAt(Vec3f(1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_9:
            mCamera.lookAt(Vec3f(-1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_UP:
            MODEL_ZOOM = MAX(WORLD_MODEL_ZOOM_MIN,MIN(MODEL_ZOOM+1,WORLD_MODEL_ZOOM_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_DOWN:
            MODEL_ZOOM = MAX(WORLD_MODEL_ZOOM_MIN,MIN(MODEL_ZOOM-1,WORLD_MODEL_ZOOM_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_LEFT:
            MODEL_SCALE = MAX(WORLD_MODEL_SCALE_MIN,MIN(MODEL_SCALE-WORLD_MODEL_SCALE_STEP,WORLD_MODEL_SCALE_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_RIGHT:
            MODEL_SCALE = MAX(WORLD_MODEL_SCALE_MIN,MIN(MODEL_SCALE+WORLD_MODEL_SCALE_STEP,+WORLD_MODEL_SCALE_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_f:
            SHOW_INFO = !SHOW_INFO;
            break;
        default:
            break;
    }
}

void grid_04App::updateView(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio*MODEL_ZOOM, aspectRatio*MODEL_ZOOM, -MODEL_ZOOM, MODEL_ZOOM, 0.0001, 5.0f);
}

void grid_04App::resize(){
    this->updateView();
    
}

CINDER_APP_NATIVE( grid_04App, RendererGl )
