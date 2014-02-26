#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include <vector>
#include "cinder/Rand.h"


#include "Grid.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const int APP_WIDTH(1776), APP_HEIGHT(552);
static const float APP_FPS(30.0f);




class grid_00App : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
    void resize();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    CameraOrtho  mCamera;
    float        mCameraZoom;
    gl::Light*   mLight;
    
    gl::Material  mMaterial;
    vector<Grid*> mGrids0;
    vector<Grid*> mGrids1;

    //Grid         mGrid;
    

    

};

void grid_00App::setup(){
    //mCamera.setPerspective(45.0f, app::getWindowAspectRatio(), 0.0001f, 10.0f);
    mCameraZoom = 8.0f;
    //mCamera.setOrtho(-app::getWindowAspectRatio() * mCameraZoom,  mCameraZoom, -mCameraZoom, mCameraZoom, 0.01f, 30.0f);
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho( -aspectRatio, aspectRatio, -1, 1, 0.0001f, 30.0f);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    mLight = new gl::Light(gl::Light::POINT,0);
    mLight->lookAt( Vec3f( 5, 5, 1 ), Vec3f( 0, 0, 0 ) );
	mLight->setAmbient( Color( 0.3f, 0.3f, 0.3f ) );
	mLight->setDiffuse( Color( 0.5f, 0.5f, 0.5f ) );
	mLight->setSpecular( Color( 1.0f, 0.5f, 0.5f ) );
    mLight->enable();
    
    mMaterial.setDiffuse(Color(0.135f,0.135f,0.135f));
    mMaterial.setAmbient(Color(0.15f,0.15f,0.15f));
    mMaterial.setSpecular(Color(0.35f,0.35f,0.35f));
    mMaterial.setShininess( 10.0f );
    
    gl::enableDepthRead();
    
    
    int numGridsX = 15;
    int numGridsY = 15;
    
    int i,j;
    i = -1;
    while (++i < numGridsY) {
        j = -1;
        while (++j < numGridsX) {
            mGrids0.push_back(new Grid(Vec3f(-numGridsX /2 + j,0.0f,-numGridsY /2+ i),Rand::randInt(3,60),2));
            mGrids1.push_back(new Grid(Vec3f(-numGridsX /2 + j,0.0f,-numGridsY /2+ i),2,2));
        }
    }
    
    
    
    
    
}

void grid_00App::mouseDown( MouseEvent event )
{
}

void grid_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_00App::update(){
    float time = app::getElapsedSeconds();
    //mLight->lookAt( Vec3f( cosf(time), 5, sinf(time) ), Vec3f( 0, 0, 0 ) );
    
    for(vector<Grid*>::iterator itr = mGrids0.begin(); itr != mGrids0.end(); itr++){
        (*itr)->update();
    }
    for(vector<Grid*>::iterator itr = mGrids1.begin(); itr != mGrids1.end(); itr++){
        (*itr)->update();
    }
    //mGrid.update();
}

void grid_00App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0,0,0 ) );
    gl::setMatrices(mCamera);
    glMatrixMode(GL_MODELVIEW_MATRIX);
    //gl::drawCoordinateFrame(1.0f, 0.02f, 0.005f );
    
    glEnable(GL_LIGHTING);
    glEnable( GL_LIGHT0 );
    
    
    mLight->update(mCamera);
    glPushMatrix();
    glScalef(0.75f, 0.75f, 0.75f);
    mMaterial.apply();
    int i,j;
    i = -1;
    while (++i < 15) {
        j = -1;
        while (++j < 15) {
            gl::drawCube(Vec3f(-7.5f + j,0,-7.5f + i), Vec3f(0.98f,0.05f,0.98f));
        }
    }
    
   
    for(vector<Grid*>::iterator itr = mGrids0.begin(); itr != mGrids0.end(); itr++){
        (*itr)->draw();
    }
    glScalef(0.5f, 1, 0.5f);
    //glRotatef(90.0f, 0, 1, 0);
    for(vector<Grid*>::iterator itr = mGrids1.begin(); itr != mGrids1.end(); itr++){
        (*itr)->draw();
    }
    glPopMatrix();
    
        //mGrid.draw();
    gl::drawColorCube(Vec3f(0,-0.25f,0), Vec3f(0.25f,0.25f,0.25f));
    mLight->disable();
    
    glDisable(GL_LIGHTING);
    
    
}

void grid_00App::resize(){
    //mCamera.setOrtho(-app::getWindowAspectRatio(), app::getWindowAspectRatio(), -1, 1, 0.0001f, 60.0);
}

CINDER_APP_NATIVE( grid_00App, RendererGl )
