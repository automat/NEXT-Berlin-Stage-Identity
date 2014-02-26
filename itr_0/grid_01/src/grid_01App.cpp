#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "GridController.h"
#include "cinder/Utilities.h"

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
static float MODEL_SCALE(1.0f); //1

/*--------------------------------------------------------------------------------------------*/

class grid_01App : public AppNative {
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
    GridController* mController;
    
    
};

void grid_01App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_01App::setup(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, 0.0001, 30.0f);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    mController = new GridController(11,11);
    
    gl::enableDepthRead();
}

void grid_01App::mouseDown( MouseEvent event ){
}

void grid_01App::update(){
    mController->update();
    
}

void grid_01App::draw(){
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    /*
    gl::drawCoordinateFrame();
    glColor3f(1, 1, 1);
    gl::drawStrokedCube(Vec3f::zero(), Vec3f(1,1,1));
    */
    glPushMatrix();
    glScalef(MODEL_SCALE, MODEL_SCALE, MODEL_SCALE);
    mController->draw();
    glPopMatrix();
    
    glPushAttrib(GL_VIEWPORT_BIT);
    gl::setMatricesWindow(app::getWindowSize());
    gl::enableAlphaBlending();
    gl::drawString("Model Zoom:  " + toString(MODEL_ZOOM), Vec2f(10,10));
    gl::drawString("Model Scale: " + toString(MODEL_SCALE), Vec2f(10,30));
    gl::drawString("FPS:" + toString(getAverageFps()), Vec2f(app::getWindowWidth() - 100, 10));
    
    
    gl::disableAlphaBlending();
    
    glPopAttrib();
    
}

void grid_01App::keyDown(KeyEvent event){
    
    switch (event.getCode()) {
        case KeyEvent::KEY_1:
            MODE_VIEW = 0;
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_2:
            MODE_VIEW = 1;
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
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
        default:
            break;
    }
}

void grid_01App::updateView(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio*MODEL_ZOOM, aspectRatio*MODEL_ZOOM, -MODEL_ZOOM, MODEL_ZOOM, 0.0001, 30.0f);
}

void grid_01App::resize(){
    this->updateView();
    
}

CINDER_APP_NATIVE( grid_01App, RendererGl )
