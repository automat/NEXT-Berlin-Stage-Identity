#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include <vector>
#include "GridCube.h"
#include "GridEdge.h"

using namespace ci;
using namespace ci::app;
using namespace std;

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
    
    /*----------------------------------------------------------------------------------*/
    
    int mNumPointsX;
    int mNumPointsY;
    int mNumPointsZ;
    int mNumCubes;
    int mNumPoints;
    int mNumEdges;
    
    std::vector<ci::Vec3f> mGridPoints;
    std::vector<GridEdge>  mGridEdges;
    std::vector<GridCube>  mCubes;
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
    mCameraZoom = 1.0f;
    mCameraRotX = (float)M_PI * 0.5f;
    mCameraRotY = 0.0f;
    
    mMouseWheelOffset = 0.0f;
    
    setupGridCube();
}

void IsoGridMarcherApp::setupGridCube(){
    
    mNumPointsX = 3;
    mNumPointsY = 3;
    mNumPointsZ = 3;
    int numPointsXY = mNumPointsX * mNumPointsY;
    
    int numCubeX = mNumPointsX - 1;
    int numCubeY = mNumPointsY - 1;
    int numCubeZ = mNumPointsZ - 1;

    
    mNumCubes = numCubeX * numCubeY * numCubeZ;
    mNumPoints= mNumPointsX * mNumPointsY * mNumPointsZ;
    
    int i,j,k;
    float ni,nj,nk;
    
    ci::Vec3f size(1.0f/(float)numCubeX,1.0f/(float)numCubeY,1.0f/(float)numCubeZ);
    
    i = -1;
    while (++i < mNumPointsX) {
        ni = (float)i/(float)(mNumPointsX-1);
        j = -1;
        while (++j < mNumPointsY) {
            nj = (float)j/(float)(mNumPointsY-1);
            k = -1;
            while (++k < mNumPointsZ) {
                nk = (float)k/(float)(mNumPointsZ-1);
                
                mGridPoints.push_back(ci::Vec3f(-0.5 + ni, -0.5f + nj, -0.5f + nk));
  
                
                //mCubes.push_back(GridCube(ci::Vec3f(-0.5f + ni, -0.5f + nj, -0.5f + nk),size));
            }
        }
    }
    
    int numEdgesX  = mNumPointsX - 1;
    int numEdgesY  = mNumPointsY - 1;
    int numEdgesZ  = mNumPointsZ - 1;
    int numEdgesXY = numEdgesX * numEdgesY;
    

    int index;
    int indexX1;
    
    
    i = -1;
    while (++i < numEdgesX) {
        j = -1;
        while (++j < numEdgesY) {
            k = -1;
            while (++k < numEdgesZ) {
                index   = i * numPointsXY + j * mNumPointsZ + k;
                indexX1 = (i+1) * numPointsXY + j * mNumPointsZ + k;
                //std::cout << index << "  " << indexX1 << std::endl;
                mGridEdges.push_back(GridEdge(&mGridPoints[index],&mGridPoints[indexX1]));
                //mCubes.push_back(GridCube(ci::Vec3f(-0.5f + ni, -0.5f + nj, -0.5f + nk),size));
            }
        }

    }
    
    mNumEdges = mGridEdges.size();
    
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
    
    gl::clear( Color( 0.085f, 0, 0.085f ) );
    
    glMatrixMode(GL_MODELVIEW_MATRIX);
    glPushMatrix();
    drawAxes();
    
    
    glColor3f(1,1,1);
    int i = -1;
    
    while (++i < mCubes.size()) {
        ci::gl::drawStrokedCube(mCubes[i].getPosition(), mCubes[i].getSize());
    }
     
    
    i = -1;
    while (++i < mNumPoints) {
        ci::gl::drawSphere(mGridPoints[i],0.0015f);
    }
    
    i = -1;
    while (++i < mNumEdges) {
        mGridEdges[i].draw();
    }
     
    
    
    glPopMatrix();
    
    mTimeLast = mTime;
}

void IsoGridMarcherApp::drawAxes(){
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
