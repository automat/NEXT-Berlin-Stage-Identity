#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Light.h"

#include "PRDCB/gl/glUtil.h"
#include "PRDCB/math/PRMath.h"

#include <vector>
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "SegCube.h"
#include "SegLine.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class GeomQuadWall_2ElementsApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel(MouseEvent event );
    void resize();
    void updateCamRot(const ci::Vec2f& pos);
    void setupPaths();
    //   void setupShadowMap();
	void update();
    void drawGeometry();
	void draw();
    void drawAxes();
    
    /*----------------------------------------------------------------------------------*/
    
    //Camera
    
    ci::CameraPersp* mCamera;
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
    
    ci::gl::GlslProg mShader;
    ci::gl::Fbo      mDepthFbo;
    ci::gl::Light*   mLight0;
    ci::gl::Material mMaterial0;
    ci::gl::Material mMaterial1;
    
    /*----------------------------------------------------------------------------------*/
    
    std::vector<std::vector<ci::Vec3f>> mPaths;
    
    std::vector<SegCube> mSegsCube;
    std::vector<SegLine> mSegsLine;
    
};

static const Color BLUE( 0.25f, 0.25f, 1.0f ), RED( 1.0f, 0.55f, 0.55f );

void GeomQuadWall_2ElementsApp::prepareSettings(Settings* settings){
    settings->setWindowSize(1024, 768);
    settings->setFrameRate(30);
}

void GeomQuadWall_2ElementsApp::setup(){
    mCamera = new ci::CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f );
    mCamera->setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
    mTime = mTimeLast = mTimeDelta = 0.0f;
    mTick = 0;
    
    mCameraEye.set(1, 1, 1);
    
    mCamera->lookAt(mCameraEye, ci::Vec3f::zero());
    
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
    
    mLight0 = new gl::Light( gl::Light::DIRECTIONAL, 0 );
	mLight0->lookAt( Vec3f( 5,5,5 ), Vec3f( 0, 0, 0 ) );
	mLight0->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight0->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight0->setSpecular( Color( 1,1,1 ) );
	mLight0->update( *mCamera );
	mLight0->enable();
    
    mMaterial0.setAmbient( BLUE );
	mMaterial0.setDiffuse( RED );
    mMaterial0.setSpecular( Color(1,1,1));
	mMaterial0.setShininess( 125.0f );
    
    mMaterial1.setAmbient( Color(1,1,1) );
	mMaterial1.setDiffuse( Color(1,1,1) );
    mMaterial1.setSpecular( Color(1,1,1));
	mMaterial1.setShininess( 125.0f );
    
    
    
    setupPaths();
}

void GeomQuadWall_2ElementsApp::setupPaths(){
    int i,j;
    float ni;
    mPaths.resize(2000);
    int pathPointsNum = 30;
    i = -1;
    while (++i < mPaths.size()) {
        j = -1;
        while (++j < pathPointsNum) {
            ni = (float)j/(float)pathPointsNum;
            mPaths[i].push_back(ci::Vec3f(0,0.0025,(-0.5+ni)));
        }
        
    }
    
    i = -1;
    while (++i < mPaths.size()) {
        if (ci::randFloat() < 0.30f) {
            mSegsCube.push_back(SegCube(&mPaths[i]));
            SegCube& cube = mSegsCube.back();
            cube.setScale(ci::randFloat(),ci::randFloat()*0.145f,ci::randFloat()*0.015f);
            cube.setPathOffset(0.0f, ci::randFloat(-1,1));
            
        }else{
            mSegsLine.push_back(SegLine(&mPaths[i],30,10));
            SegLine& line = mSegsLine.back();
            line.setScale(ci::randFloat() * 0.0035f);
            line.setPathOffset(ci::randFloat(-0.0015f,0.0015f), ci::randFloat(-1,1));
            
        }
        
        
        // SegCube& cube = mSegsCube.back();
        // cube.setScale(ci::randFloat(),ci::randFloat()*0.0145f,ci::randFloat()*0.025f);
        //cube.setOffset(0.0f, ci::randFloat(-1,1));
    }
    
    
    
}


void GeomQuadWall_2ElementsApp::update(){
    mTime        = (float)app::getElapsedSeconds();
    mTimeDelta   = mTime - mTimeLast;
    mTick++;
    
    int i,j;
    float ni,nj;
    
    std::vector<std::vector<ci::Vec3f>>::iterator itrPaths = mPaths.begin();
    std::vector<ci::Vec3f>::iterator itrPath;
    
    float pathsSize = (float)mPaths.size();
    float pathSize;
    
    i = 0;
    while (itrPaths != mPaths.end()) {
        ni       = (float)i / pathsSize;
        itrPath  = itrPaths->begin();
        pathSize = (float)itrPaths->size();
        
        j = 0;
        while (itrPath != itrPaths->end()) {
            nj = (float)j / pathSize;
            
            itrPath->y = 0.0f;//sinf(nj * P_2PI + mTime * 0.5)*0.25f;
            itrPath->x = sinf(nj * P_PI * 2 + mTime)*0.055f + (-0.5 + ni)*2;
            
            ++j;
            ++itrPath;
        }
        ++i;
        ++itrPaths;
    }
    
    
    float n;
    i = 0;
    for (std::vector<SegCube>::iterator itr = mSegsCube.begin(); itr != mSegsCube.end(); itr++) {
        ni = (float)i/(float)(mSegsCube.size());
        n  = (1 +pr::math::sawf(ni * P_PI*16 + mTime*0.25f)) * 0.5f;
        itr->setPathRange(n,n+0.1f);
        itr->update();
        ++i;
    }
    
    i = 0;
    for (std::vector<SegLine>::iterator itr = mSegsLine.begin(); itr != mSegsLine.end(); itr++) {
        ni = (float)i/(float)(mSegsLine.size());
        n  = (1 +pr::math::sawf(ni * P_PI*2 + mTime*0.25f)) * 0.5f;
        itr->setPathRange(n,n+0.15f);
        itr->update();
        ++i;
    }
    
}

void GeomQuadWall_2ElementsApp::draw(){
	mCameraZoom += mMouseWheelOffset;
    //mCameraZoom = std::max(-5, std::min(mCameraZoom, 5));
    
    
    mCameraEyeTarget.set(cosf(mCameraRotX) * mCameraZoom,
                         sinf(mCameraRotY) * mCameraZoom,
                         sinf(mCameraRotX) * mCameraZoom);
    
    //mCameraEyeTarget.set(-mCameraZoom,mCameraZoom,mCameraZoom);
    
    mCameraEye = mCameraEye.lerp(mTimeDelta * 2.0f, mCameraEyeTarget);
    
    
    mCamera->setEyePoint(mCameraEye);
    mCamera->setCenterOfInterestPoint(ci::Vec3f::zero());
    
    gl::setMatrices(*mCamera);
    
    
    gl::enableDepthRead();
    gl::clear( Color( 0.75f,0.75f,0.80f ) );
    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    this->drawGeometry();
    
    mTimeLast = mTime;
}

void GeomQuadWall_2ElementsApp::drawGeometry(){
    glPushMatrix();
    pr::gl::color1f(0.825f);
    //  pr::gl::drawGridCube(16);
    //pr::gl::drawGrid(32,0.25);
    //pr::gl::drawAxes();
    glPopMatrix();
    
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    mLight0->update( *mCamera );
    mMaterial0.apply();
    
    for (std::vector<SegCube>::iterator itr = mSegsCube.begin(); itr != mSegsCube.end(); itr++) {
        itr->draw();
    }
    
    mMaterial1.apply();
    for (std::vector<SegLine>::iterator itr = mSegsLine.begin(); itr != mSegsLine.end(); itr++) {
        itr->draw();
    }
    
    glDisable(GL_LIGHTING);
    
    return;
    
    for (std::vector<SegCube>::iterator itr = mSegsCube.begin(); itr != mSegsCube.end(); itr++) {
        itr->drawPath();
        itr->drawDebug();
    }
    
    
    for (std::vector<SegLine>::iterator itr = mSegsLine.begin(); itr != mSegsLine.end(); itr++) {
        itr->drawPath();
        itr->drawDebug();
    }
}

void GeomQuadWall_2ElementsApp::drawAxes(){
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

void GeomQuadWall_2ElementsApp::mouseDown( MouseEvent event ){
    updateCamRot(event.getPos());
}

void GeomQuadWall_2ElementsApp::mouseDrag( MouseEvent event ){
    updateCamRot(event.getPos());
}

void GeomQuadWall_2ElementsApp::mouseWheel(MouseEvent event){
    mMouseWheelOffset = event.getWheelIncrement() * 0.01f;
}

void GeomQuadWall_2ElementsApp::updateCamRot(const ci::Vec2f &pos){
    mCameraRotX = (-1.0f + pos.x / (float)app::getWindowWidth()  * 2.0f) * (float)M_PI;
    mCameraRotY = (-1.0f + pos.y / (float)app::getWindowHeight() * 2.0f) * (float)M_PI * 0.5f;
}

void GeomQuadWall_2ElementsApp::resize(){
    mCamera->setPerspective(45.0f, app::getWindowAspectRatio(),  0.0001, 100.0);
}

/*----------------------------------------------------------------------------------*/

CINDER_APP_NATIVE( GeomQuadWall_2ElementsApp, RendererGl )
