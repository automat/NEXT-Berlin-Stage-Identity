#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "GridController.h"
#include "cinder/Utilities.h"
#include "cinder/Frustum.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*--------------------------------------------------------------------------------------------*/

//SETTINGS

static const int   RES_SHADOW_MAP = 2048;
//static const float TEXEL_SIZE = 1.0f / (float)RES_SHADOW_MAP;
static const int   STAGE_WIDTH(3552), STAGE_HEIGHT(1105);
static const int   STAGE_SCALE(2);

static const int   APP_WIDTH(STAGE_WIDTH / STAGE_SCALE), APP_HEIGHT(STAGE_HEIGHT / STAGE_SCALE);
static const float APP_FPS(30.0);

static int MODE_VIEW(0);
const static int MODEL_ZOOM_MAX(10), MODEL_ZOOM_MIN(1);
static int MODEL_ZOOM(1);
const static float MODEL_SCALE_STEP(0.05f);
const static float MODEL_SCALE_MAX(1), MODEL_SCALE_MIN(MODEL_SCALE_STEP);
static float MODEL_SCALE(0.65); //1

static bool DRAW_FRUSTUM_DEBUG(false);

/*--------------------------------------------------------------------------------------------*/

class grid_02App : public AppNative {
public:
    void prepareSettings(Settings* settings);
	void setup();
    void resize();
	void mouseDown( MouseEvent event );
    void keyDown(KeyEvent event);
	void update();
	void draw();
    
    void updateView();
    
    //mem
    CameraOrtho     mCamera;
    CameraOrtho     mCameraDebug;
    GridController* mController;
    
    
};

void grid_02App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_02App::setup(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10, 30.0f);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    float cameraDebugZoom = 6.0f;
    mCameraDebug.setOrtho(-aspectRatio * cameraDebugZoom, aspectRatio * cameraDebugZoom, -cameraDebugZoom, cameraDebugZoom, -10, 100.0f);
    mCameraDebug.lookAt(Vec3f(0,1,0), Vec3f::zero());
    
    mController = new GridController(11,11);
    
    gl::enableDepthRead();
    glEnable(GL_SCISSOR_TEST);
}

void grid_02App::mouseDown( MouseEvent event ){
}

void grid_02App::update(){
    ci::Frustumf frustum(mCamera);
    
    mController->update();
    mController->checkFrustum(frustum);
    
}

void grid_02App::draw(){
    glScissor(0, 0, app::getWindowWidth(), app::getWindowHeight());
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
    mController->draw();
    mController->debugArea();
    glPopMatrix();
    
    
    //  info
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setMatricesWindow(app::getWindowSize());
    gl::enableAlphaBlending();
    gl::drawString("Model Zoom:  " + toString(MODEL_ZOOM), Vec2f(10,10));
    gl::drawString("Model Scale: " + toString(MODEL_SCALE), Vec2f(10,30));
    gl::drawString("FPS:" + toString(getAverageFps()), Vec2f(app::getWindowWidth() - 100, 10));
    gl::disableAlphaBlending();
    glPopAttrib();
    
    
    if(DRAW_FRUSTUM_DEBUG){
        //  debug frustum
        glPushAttrib(GL_VIEWPORT_BIT);
        static const float frustumViewBoxWidth  = 300;
        float aspectRatio = app::getWindowAspectRatio();
        glScissor(0, 0, frustumViewBoxWidth  * aspectRatio,frustumViewBoxWidth);
        glViewport(0, 0, frustumViewBoxWidth * aspectRatio,frustumViewBoxWidth);
        gl::clear(Color(0,0,0));
        gl::setMatrices(mCameraDebug);
        gl::drawFrustum(mCamera);
        
        //draw frustum plane intersection
        GLfloat modelViewMatrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
        glPushMatrix();
        glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
        mController->debugArea();
        glPopMatrix();
        glPopAttrib();
        
    }
    
}

void grid_02App::keyDown(KeyEvent event){
    
    switch (event.getCode()) {
        case KeyEvent::KEY_1:
            MODE_VIEW = 0;
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_2:
            MODE_VIEW = 1;
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_3:
            mCamera.lookAt(Vec3f(-1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_4:
            mCamera.lookAt(Vec3f(1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_5:
            mCamera.lookAt(Vec3f(-1,1,-1),Vec3f::zero());
            break;
        case KeyEvent::KEY_UP:
            MODEL_ZOOM = MAX(MODEL_ZOOM_MIN,MIN(MODEL_ZOOM+1,MODEL_ZOOM_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_DOWN:
            MODEL_ZOOM = MAX(MODEL_ZOOM_MIN,MIN(MODEL_ZOOM-1,MODEL_ZOOM_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_LEFT:
            MODEL_SCALE = MAX(MODEL_SCALE_MIN,MIN(MODEL_SCALE-MODEL_SCALE_STEP,MODEL_SCALE_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_RIGHT:
            MODEL_SCALE = MAX(MODEL_SCALE_MIN,MIN(MODEL_SCALE+MODEL_SCALE_STEP,MODEL_SCALE_MAX));
            this->updateView();
            break;
        case KeyEvent::KEY_f:
            DRAW_FRUSTUM_DEBUG = !DRAW_FRUSTUM_DEBUG;
            break;
        default:
            break;
    }
}

void grid_02App::updateView(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio*MODEL_ZOOM, aspectRatio*MODEL_ZOOM, -MODEL_ZOOM, MODEL_ZOOM, 0.0001, 30.0f);
}

void grid_02App::resize(){
    this->updateView();
    
}

CINDER_APP_NATIVE( grid_02App, RendererGl )
