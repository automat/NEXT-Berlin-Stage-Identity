#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Quaternion.h"


struct TNBFrame{
    ci::Vec3f tangent;
    ci::Vec3f normal;
    ci::Vec3f binormal;
};

using namespace ci;
using namespace ci::app;
using namespace std;

class AxesRotationApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void resize();
    void drawRotation();
    void drawTNBFrame(const ci::Vec3f& center, const TNBFrame& frame, float scale = 0.25f);
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

void AxesRotationApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void AxesRotationApp::setup(){
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



void AxesRotationApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
}



void AxesRotationApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    
    
    
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta*1.5f, mCameraEyeTarget);
    
    
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::setMatrices(mCamera);
    
    gl::enableDepthRead();
    gl::clear( Color( 0.085f, 0, 0.085f ) );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    gl::enableAlphaTest();
    drawAxes();
    gl::disableAlphaTest();
    glColor3f(0.25f, 0.25f, 0.25f);
    gl::drawStrokedCube(ci::Vec3f::zero(), ci::Vec3f(0.5f,0.5f,0.5f));
    drawRotation();
    glPopMatrix();
    
    mTimeLast = mTime;
}

void AxesRotationApp::drawRotation(){
    ci::Vec3f zero  = ci::Vec3f::zero();
    ci::Vec3f xAxis = ci::Vec3f::xAxis();
    ci::Vec3f yAxis = ci::Vec3f::yAxis();
    ci::Quatf quat;
    
    ci::Vec3f p0;
    ci::Vec3f p1;
    TNBFrame  frame;
    
    
    float pi  = (float)M_PI;
    float pi2 = pi * 2;
    float scale = 0.25f;
    float theta;
    
    p0.set(cosf(pi + mTime) * scale, sinf(pi + mTime) * scale,0.0f);
    p1.set(cosf(pi2+ mTime) * scale, sinf(pi2+ mTime) * scale,0.0f);
    
    frame.tangent = p1 - p0;
    frame.tangent.normalize();
    
    theta = frame.tangent.dot(yAxis);
    cout << theta << endl;
    
    frame.binormal = frame.tangent.cross(yAxis);
    frame.binormal.normalize();
    
    
    frame.normal = frame.tangent.cross(frame.binormal);
    frame.normal.normalize();
    

    
    
    //frame.normal = frame.tangent.cross(frame.binormal);
    //frame.normal.normalize();
    
    
    drawTNBFrame(p0, frame);
    
    glColor3f(1,1,1);
    gl::drawSphere(p0, 0.0025f);
    gl::drawSphere(p1, 0.0025f);
    glColor3f(0.25f,0.25f,0.25f);
    gl::drawVector(p0, p1, 0.02f, 0.01f);
    
}

void AxesRotationApp::drawTNBFrame(const ci::Vec3f& center, const TNBFrame& frame, float scale){
    glColor3f(1, 0, 0);
    gl::drawVector(center, center + frame.tangent * scale,0.025f,0.0125f);
    glColor3f(0, 1, 0);
    gl::drawVector(center, center + frame.normal * scale,0.025f,0.0125f);
    glColor3f(0, 0, 1);
    gl::drawVector(center, center + frame.binormal * scale,0.025f,0.0125f);
    
    
    
}

void AxesRotationApp::drawAxes(){
    glPushMatrix();
    glTranslatef(0.0f, -0.005, 0.0f);
    glColor3f(0.45f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    glColor3f(0, 0.45f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glColor3f(0, 0, 0.45f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPopMatrix();
}

/*----------------------------------------------------------------------------------*/

void AxesRotationApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void AxesRotationApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void AxesRotationApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void AxesRotationApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void AxesRotationApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( AxesRotationApp, RendererGl )
