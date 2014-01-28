#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"
#include "cinder/Camera.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class OrthonormalBasisApp : public AppNative {
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
    
    void drawString(const std::string& str, const ci::Vec3f& pos);
    void drawVector(const ci::Vec3f& s, const ci::Vec3f& e);
    ci::gl::TextureFontRef mTexFont;
};

void OrthonormalBasisApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void OrthonormalBasisApp::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 2.0f;
    mCameraRotX = (float)M_PI * 0.25f;
    mCameraRotY = (float)M_PI * 0.5f;
    
    mMouseWheelOffset = 0.0f;
    
    mTexFont = ci::gl::TextureFont::create(ci::Font("Arial",12));
}



void OrthonormalBasisApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
}

void OrthonormalBasisApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    
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
    this->drawAxes();
    glPopMatrix();
    
    /*
    ci::Vec3f e(0.125,0.125f,0.125);
    ci::Vec3f l(-0.125,0.5f,0.125);
    */
    ci::Vec3f e(0.125 + sinf(mTime * 0.0125f * (float)M_PI * 2) ,0.125f + sinf(mTime * 0.0125f * (float)M_PI * 2),0.125f);
    ci::Vec3f l(-0.125f,0.5f,0.125f);
    
    
    ci::Vec3f le((l-e).normalized());
    ci::Vec3f el((e-l).normalized());
    ci::Vec3f w(el);
    ci::Vec3f up(0,1,0);
    ci::Vec3f u(up.cross(w).normalized());
    ci::Vec3f v(w.cross(u));
    
    
    glColor3f(0, 1, 1);
    gl::drawSphere(e, 0.0125f);
    drawString("e", e);
    
    glColor3f(1, 0.125f, 0);
    gl::drawSphere(l, 0.0125f);
    drawString("l", l);
    
    glColor3f(1, 0.5f, 0);
    drawVector(e, l);
    drawString("l - e", (e + l) * 0.5);
    
    glColor3f(0, 0, 1);
    gl::drawSphere(e + w, 0.0125f);
    drawString("w",e + w);
    
    
    drawVector(e, e + w);
    drawString("e-l", e + el * 0.5);
    
    glColor3f(0.15f, 0.15f, 0.15f);
    drawVector(e, e + up);
    drawString("up", e + up);
    
    glColor3f(1,1,1);
    drawString(ci::toString(le.dot(up)), e + up * 0.25f);
    
    glColor3f(1, 0, 0);
    drawVector(e, e + u);
    drawString("u", e + u);
    
    glColor3f(1, 0, 0);
    drawVector(e, e + v);
    drawString("u", e + v);
    
    
    mTimeLast = mTime;
}

void OrthonormalBasisApp::drawAxes(){
    glPushMatrix();
    glTranslatef(0.0f, -0.005, 0.0f);
    glColor3f(1.0f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    glPushMatrix();
    glColor3f(0, 1.0f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glColor3f(0, 0, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPopMatrix();
}

void OrthonormalBasisApp::drawString(const std::string& str, const ci::Vec3f& pos){
    gl::enableAlphaBlending();
    static const ci::Vec2f zero(0,0);
    glPushMatrix();
    glTranslatef(pos.x, pos.y + 0.025f, pos.z);
    glScalef(0.0025f, -0.0025f, 0.0025f);
    mTexFont->drawString(str, zero);
    glPopMatrix();
    gl::disableAlphaBlending();
}

void OrthonormalBasisApp::drawVector(const ci::Vec3f &s, const ci::Vec3f &e){
    gl::drawVector(s, e, 0.03f, 0.015f);
}

/*----------------------------------------------------------------------------------*/

void OrthonormalBasisApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void OrthonormalBasisApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void OrthonormalBasisApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void OrthonormalBasisApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void OrthonormalBasisApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( OrthonormalBasisApp, RendererGl )
