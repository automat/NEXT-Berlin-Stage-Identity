#include "cinder/app/AppNative.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"

#include "cinder/params/Params.h"

#include "FragSurface.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const Vec2f WINDOW_SIZE(1000,1000);
static const float FPS(60);


class Fragment_01App : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
    void resize();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
    
    CameraPersp mCamera;
    Vec3f mCameraEye;
    Vec3f mCameraEyeTarget;
    float     mCameraZoom;
    float     mCameraRotX;
    float     mCameraRotY;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    
    params::InterfaceGlRef mParams;
    
    FragSurface* mFragSurface;

};

void Fragment_01App::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_SIZE);
    settings->setFrameRate(FPS);
}

void Fragment_01App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}

/*--------------------------------------------------------------------------------------------*/

void Fragment_01App::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001, 10.0f);
    mCameraEye.set(0, 1.5, 0);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mTime = mTimeLast = mTimeDelta = 0.0f;
    
    mFragSurface = new FragSurface();
    
    mParams = params::InterfaceGl::create(this->getWindow(), "Controls", Vec2f(300,100));
    mParams->addParam("Animate", &FragSurface::ANIMATE);
    mParams->addParam("Draw Triangles", &FragSurface::DRAW_TRIANGLES);
    mParams->addParam("Debug Draw Field", &FragSurface::DEBUG_DRAW_FIELD);
    mParams->addParam("Debug Draw Tangents", &FragSurface::DEBUG_DRAW_TANGENTS);
}

void Fragment_01App::mouseDown( MouseEvent event )
{
}

void Fragment_01App::update(){
    mTime = (float)app::getElapsedSeconds();
    mTimeDelta = mTime - mTimeLast;
    //mCameraEye.y = 1.0f + (0.5f + sinf(mTime * (float)M_1_PI) * 0.5f);
    mFragSurface->update(mTime);
}

void Fragment_01App::draw(){
	// clear out the window with black
	
    
    
    mFragSurface->drawFbo();

    
    gl::clear( Color( 0, 0, 0 ) );
    mCamera.setEyePoint(mCameraEye);
    gl::setMatrices(mCamera);
    
    gl::enableDepthRead();
    gl::clear( Color( 0, 0, 0 ) );
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    
    //gl::drawColorCube(zero, ci::Vec3f(0.25f,0.25f,0.25f));
    
    
    mFragSurface->draw(mTime);
    
    glPopMatrix();
    
    mParams->draw();
    
    mTimeLast = mTime;
}

CINDER_APP_NATIVE( Fragment_01App, RendererGl )
