#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "PRDCB/gl/glUtil.h"
#include "PRDCB/math/PRMath.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class TemplateApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void resize();
    void updateCamRot(const ci::Vec2f& pos);
    
	void update();
	void draw();
    void drawAxes();
    
    /*----------------------------------------------------------------------------------*/
    
    //Camera
    
    ci::CameraPersp  mCamera;
    ci::Vec3f        mCameraEye;
    ci::Vec3f        mCameraEyeTarget;
    float            mCameraZoom;
    float            mCameraRotX;
    float            mCameraRotY;
    float            mMouseWheelOffset;
    
    /*----------------------------------------------------------------------------------*/
    
    ci::Timer mTimer;
    float mTimerDiff;
    
    float mTime;
    float mTimeLast;
    float mTimeDelta;
    int   mTick;
    
    /*----------------------------------------------------------------------------------*/
};

void TemplateApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void TemplateApp::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
}



void TemplateApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
}

void TemplateApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    pr::math::clampf(&mCameraZoom, -5.0f, 5.0f);
 
    
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta*1.5f, mCameraEyeTarget);
    
    
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::setMatrices(mCamera);
    
    gl::clear( Color( 0.085f, 0, 0.085f ) );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    pr::gl::color1f(0.125f);
    pr::gl::drawGridCube(16);
    pr::gl::drawGrid(16);
    pr::gl::drawAxes();
    glPopMatrix();
    
    mTimeLast = mTime;
}

void TemplateApp::drawAxes(){
    glPushMatrix();
    glTranslatef(0.0f, -0.005, 0.0f);
    glColor3f(1.0f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    glColor3f(0, 1.0f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glColor3f(0, 0, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPopMatrix();
}

/*----------------------------------------------------------------------------------*/

void TemplateApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void TemplateApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void TemplateApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void TemplateApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void TemplateApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( TemplateApp, RendererGl )
