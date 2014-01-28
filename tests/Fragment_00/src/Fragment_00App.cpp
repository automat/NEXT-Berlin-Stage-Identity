#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Vector.h"
#include "cinder/gl/Texture.h"


#include "FragmentSurface.h"



using namespace ci;
using namespace ci::app;
using namespace std;

static const ci::Vec2f WINDOW_SIZE(600,600);
static const float FPS(30.0f);


class Fragment_00App : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void resize();
    void mouseDown( MouseEvent event );
	void update();
	void draw();
    
    ci::CameraPersp mCamera;
    ci::Vec3f mCameraEye;
    ci::Vec3f mCameraEyeTarget;
    float     mCameraZoom;
    float     mCameraRotX;
    float     mCameraRotY;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;


    FragmentSurface* mFragSurface;
    
};

void Fragment_00App::prepareSettings(cinder::app::AppBasic::Settings *settings){
    settings->setWindowSize(WINDOW_SIZE);
    settings->setFrameRate(FPS);
}

void Fragment_00App::resize(){
    mCamera.setAspectRatio(app::getWindowAspectRatio());
}

void Fragment_00App::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001, 10.0f);
    mCameraEye.set(0, 1, 0);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mTime = mTimeLast = mTimeDelta = 0.0f;
    
    mFragSurface = new FragmentSurface();
}

void Fragment_00App::mouseDown( MouseEvent event )
{
}

void Fragment_00App::update(){
    mTime = (float)app::getElapsedSeconds();
    mTimeDelta = mTime - mTimeLast;
    
    //mCameraEye.y = 1.0f + (0.5f + sinf(mTime * (float)M_1_PI) * 0.5f);
    mFragSurface->update(mTime);
}

void Fragment_00App::draw(){
    static const ci::Vec3f zero(0,0,0);
    

    
    mCamera.setEyePoint(mCameraEye);
    
	gl::setMatrices(mCamera);
    
    gl::enableDepthRead();
    gl::clear( Color( 0, 0, 0 ) );
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    
    //gl::drawColorCube(zero, ci::Vec3f(0.25f,0.25f,0.25f));
    
    
    mFragSurface->draw();
    
    glPopMatrix();
    
    mTimeLast = mTime;
    
}

CINDER_APP_NATIVE( Fragment_00App, RendererGl )
