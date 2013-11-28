#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include <vector>
#include "cinder/Vector.h"

#include "PRDCB/gl/glUtil.h"
#include "PRDCB/math/PRMath.h"
#include "PRDCB/geom/Spline.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class PRDCBApp : public AppNative {
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

    std::vector<ci::Vec3f> mPointRes;
    pr::geom::Spline       mSpline;
    pr::geom::TNBFrame     mSplineFrame;
    

};

void PRDCBApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void PRDCBApp::setup(){
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
    
    size_t num = 10;
    int i = -1;
    float n;
    float maxAngle = P_2PI ;
    while (++i < num) {
        n = (float)i / (float)(num - 1) ;
        mPointRes.push_back(ci::Vec3f(cosf(n * maxAngle) * 0.25f,sinf(n * maxAngle)*0.25f,0.0f));
        //mPointRes.push_back(ci::Vec3f(0,n,0));
    }

    /*
    mPointRes.push_back(ci::Vec3f(0,0,0));
    mPointRes.push_back(ci::Vec3f(0,0.125f,0));
    mPointRes.push_back(ci::Vec3f(0,0.25f,0));
    mPointRes.push_back(ci::Vec3f(0,0.25f,0.125f));
    mPointRes.push_back(ci::Vec3f(0,0.25f,0.25f));
    */
    /*
    mPointRes.push_back(ci::Vec3f(0.25f,0.5f,0));
    mPointRes.push_back(ci::Vec3f(0.5f,0.5f,0));
    */
    
    mSpline.setDetail(2);
    mSpline.setPoints(mPointRes);
    mSpline.update();
}



void PRDCBApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;

    //mSpline.setBias(sinf(mTime));
    //mSpline.setTension(cosf(mTime)*2);
    mSpline.update();
    mSpline.recalcFrames();
}

void PRDCBApp::draw(){
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
   //r pr::gl::drawAxes();
    
    pr::gl::color1f(0.125f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mSpline.getPoints()[0].x);
    glDrawArrays(GL_LINE_STRIP, 0, mSpline.getNumPoints());
    
    pr::gl::color1f(1.0f);
    glVertexPointer(3, GL_FLOAT, 0, &mSpline.getVertices()[0].x);
    glDrawArrays(GL_LINE_STRIP, 0, mSpline.getNumVertices());
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, mSpline.getNumVertices());
    
    glDisableClientState(GL_VERTEX_ARRAY);
    
    pr::gl::drawTNBFrames(mSpline.getVertices(), mSpline.getFrames(),0.025f);
    pr::geom::TNBFrame frame;
    ci::Vec3f point;
    float step = abs(sinf(mTime));//(1.0f + pr::math::sawf(mTime * 0.25f)) * 0.5f;
    mSpline.getFrameOnSpline(step, &frame);
    mSpline.getPointOnSpline(step, &point);
    
    pr::gl::drawTNBFrame(point, frame,0.1f);
    gl::drawSphere(point, 0.005f);
    glPopMatrix();
    
    mTimeLast = mTime;
}

void PRDCBApp::drawAxes(){
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

void PRDCBApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void PRDCBApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void PRDCBApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void PRDCBApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void PRDCBApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( PRDCBApp, RendererGl )
