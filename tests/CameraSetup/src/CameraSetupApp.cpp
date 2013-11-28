#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Arcball.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class CameraSetupApp : public AppNative {
public:
	void setup();
    void prepareSettings(cinder::app::App::Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void keyDown(KeyEvent event);
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
    ci::Arcball      mArcball;
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

void CameraSetupApp::prepareSettings(cinder::app::App::Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}


void CameraSetupApp::keyDown(cinder::app::KeyEvent event){
    if(event.getChar() == ' '){
        mArcball.setConstraintAxis(ci::Vec3f::yAxis());
    }else if(event.getChar() == 'x'){
        mArcball.setConstraintAxis(ci::Vec3f::zAxis());
    }
}


void CameraSetupApp::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    

    
    mCameraEye.set(1, 1, 1);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    //mArcball.setConstraintAxis(ci::Vec3f(1,0,0));
    
    mMouseWheelOffset = 0.0f;
}



void CameraSetupApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
}

void CameraSetupApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    
   
    /*
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta*1.5f, mCameraEyeTarget);
    */

    
    
    
    float dist = mCameraZoom;
    float camDist = dist * dist + 1.0f;
    ci::Quatf quat = mArcball.getQuat();
    //quat.w *= -1.0f;
    ci::Vec3f camTarget = Vec3f::zero();
    ci::Vec3f camOffset = quat * ci::Vec3f(0.0f,0.0f,camDist);
    
    
    ci::Vec3f cam_eye    = camTarget - camOffset;
    ci::Vec3f cam_up     = quat * Vec3f(0,1,0);
    mCamera.lookAt(cam_eye, camTarget, cam_up);
    
    //mCamera.setEyePoint(mCameraEye);
    //mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::setMatrices(mCamera);
    gl::enableDepthRead();
    gl::clear( Color( 0.085f, 0, 0.085f ) );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    //gl::rotate( mArcball.getQuat() );
    drawAxes();
    gl::drawColorCube(ci::Vec3f::zero(), ci::Vec3f(0.25f,0.25f,0.25f));
    glPopMatrix();
    
    mTimeLast = mTime;
}

void CameraSetupApp::drawAxes(){
    
    glPushMatrix();
    glTranslatef(0.0f, -0.005, 0.0f);
    glColor3f(1.0f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    gl::drawStrokedCircle(ci::Vec2f::zero(), 1.0f,50);
    glColor3f(0, 1.0f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glPushMatrix();
    glRotatef(90, ci::Vec3f::yAxis());
    gl::drawStrokedCircle(ci::Vec2f::zero(), 1.0f,50);
    glPopMatrix();
    glColor3f(0, 0, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPushMatrix();
    glRotatef(90, ci::Vec3f::xAxis());
    gl::drawStrokedCircle(ci::Vec2f::zero(), 1.0f,50);
    glPopMatrix();
    
    
}

/*----------------------------------------------------------------------------------*/

void CameraSetupApp::mouseDown( MouseEvent event ){
   // updateCamRot(event.getPos());
    ci::Vec2f mpos = event.getPos();
    mpos.y = app::getWindowHeight()-mpos.y;
    mArcball.mouseDown( mpos );
}

void CameraSetupApp::mouseDrag( MouseEvent event ){
  //  updateCamRot(event.getPos());
    ci::Vec2f mpos = event.getPos();
    mpos.y = app::getWindowHeight()-mpos.y;
    mArcball.mouseDrag(event.getPos());
}

void CameraSetupApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void CameraSetupApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void CameraSetupApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mArcball.setWindowSize( getWindowSize() );
	mArcball.setCenter( Vec2f( getWindowWidth() / 2.0f, getWindowHeight() / 2.0f ) );
	mArcball.setRadius( getWindowHeight() * 0.5f);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( CameraSetupApp, RendererGl )
