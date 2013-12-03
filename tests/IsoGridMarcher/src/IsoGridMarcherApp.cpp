#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include <vector>
#include "LightCube.h"
#include "LightBulb.h"
#include "cinder/Rand.h"
#include "Utils.h"
#include "LightCubeMatrix.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*----------------------------------------------------------------------------------*/

class IsoGridMarcherApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void resize();
    void updateCamRot(const ci::Vec2f& pos);
    void setupGridCube();
    
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
    
    LightCubeMatrix* mLightMatrix;

};

void IsoGridMarcherApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void IsoGridMarcherApp::setup(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 3.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
    
    mLightMatrix = new LightCubeMatrix();
    mLightMatrix->setPointsNum(9,9,9);

}


void IsoGridMarcherApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
}

void IsoGridMarcherApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    mCameraZoom = std::max(-5.0f,std::min(mCameraZoom, 5.0f));
    
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
    drawAxes();
    
    mLightMatrix->switchOff();
    
    int cubeIndex = floorf(abs(sinf(mTime*0.025f))*mLightMatrix->getNumLightCubes());
    
    mLightMatrix->switchRandom(abs(sinf(mTime)*0.025f));
    
    mLightMatrix->drawEmissive();
    mLightMatrix->drawOcclusive();
    
    /*
    ci::gl::disableDepthRead();
    ci::gl::color(Color::white());
    ci::gl::enableAlphaBlending();
    mLightMatrix->drawDebugLightBulbsX();
    mLightMatrix->drawDebugLightBulbsY();
    mLightMatrix->drawDebugLightBulbsZ();
    mLightMatrix->drawDebugLightBulbsXD();
    mLightMatrix->drawDebugLightBulbsYD();
    mLightMatrix->drawDebugLightBulbsZD();
    ci::gl::disableAlphaBlending();
    ci::gl::enableDepthRead();
     */
    
    
    mTimeLast = mTime;
}

void IsoGridMarcherApp::drawAxes(){
    glPushMatrix();
    glTranslatef(0.0f, -0.00005f, 0.0f);
    glColor3f(1.0f,0,0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::xAxis() ,0.05f,0.025f );
    glColor3f(0, 1.0f, 0);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::yAxis() ,0.05f,0.025f);
    glColor3f(0, 0, 1.0f);
    gl::drawVector(ci::Vec3f::zero(), ci::Vec3f::zAxis() ,0.05f,0.025f);
    glPopMatrix();
}

/*----------------------------------------------------------------------------------*/

void IsoGridMarcherApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void IsoGridMarcherApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void IsoGridMarcherApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void IsoGridMarcherApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void IsoGridMarcherApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( IsoGridMarcherApp, RendererGl )
