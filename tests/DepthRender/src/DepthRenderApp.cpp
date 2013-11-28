#include "Resources.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/GlslProg.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class DepthRenderApp : public AppNative {
public:
	void setup();
    void prepareSettings(cinder::app::App::Settings* settings);
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
    
    float mFarClip;
    /*----------------------------------------------------------------------------------*/
    
    gl::GlslProgRef mShader;
};

void DepthRenderApp::prepareSettings(cinder::app::App::Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void DepthRenderApp::setup(){
    mFarClip = 10.0f;
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, mFarClip);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
    
    try {
		mShader = gl::GlslProg::create( loadResource( VERT_GLSL ), loadResource( FRAG_GLSL ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}

}



void DepthRenderApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
}

void DepthRenderApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    
    
    
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta*1.5f, mCameraEyeTarget);
    
    
    mCamera.setEyePoint(mCameraEye);
    mCamera.setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::enableDepthRead();
    gl::setMatrices(mCamera);
    
    gl::clear( Color( 0.085f, 0, 0.085f ) );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    mShader->bind();
	mShader->uniform( "uFarClip", mCamera.getFarClip() );
    
    size_t num  = 10;
    float scale = (mCamera.getFarClip() * mCamera.getFarClip() ) / (float)num;
    float cubeScale = scale * 0.05f;
    int i,j,k;
    float ni,nj,nk;
    i = -1;
    while (++i < num) {
        ni = (-0.5f + (float)i / float(num-1)) * scale;
        j = -1;
        while (++j < num) {
            nj = (-0.5f + (float)j / float(num-1)) * scale;
            k = -1;
            while (++k < num) {
                nk = (-0.5f + (float)k / float(num-1)) * scale;
                gl::drawColorCube(ci::Vec3f(ni,nj,nk), ci::Vec3f(cubeScale,cubeScale,cubeScale));

            }
        }
    }
    

    mShader->unbind();
    glPopMatrix();
    
    mTimeLast = mTime;
}

void DepthRenderApp::drawAxes(){
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

void DepthRenderApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void DepthRenderApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void DepthRenderApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void DepthRenderApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void DepthRenderApp::resize(){
    mCamera.setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, mFarClip);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( DepthRenderApp, RendererGl )
