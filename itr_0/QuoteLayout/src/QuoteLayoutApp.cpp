#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "Config.h"
#include <vector>
#include "Cell.h"
#include "QuoteTypesetter.h"

#include <boost/assign/std/vector.hpp>


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace boost::assign;

class QuoteLayoutApp : public AppNative {
  public:
    void prepareSettings(Settings* settins);
	void setup();
    void keyDown(KeyEvent event);
	void mouseDown( MouseEvent event );
    void updateView();
	void update();
	void draw();
    
    vector<Cell*>    mCells;
    QuoteTypesetter* mTypesetter;
    
    
    CameraOrtho mCamera;
    float       mCameraZoom;
    
};

void QuoteLayoutApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void QuoteLayoutApp::setup(){
    mCameraZoom = 4;
    mCamera.lookAt(Vec3f(0,1,0), Vec3f::zero());
    updateView();
    
    int size_2 = GRID_NUM_XY / 2;
    int i,j;
    i = -1;
    while (++i < GRID_NUM_XY) {
        j = -1;
        while(++j < GRID_NUM_XY){
            int id[] = {j,i};
            mCells += new Cell(id,Vec3f(-size_2 + j, 0, -size_2 + i));
        }
    }
    
    //define area for layout
    float rectWidth = 5, rectHeight = 1;
    Vec3f tl(-rectWidth * 0.5f,0,-rectHeight * 0.5f),
          tr( rectWidth * 0.5f,0,-rectHeight * 0.5f),
          bl(-rectWidth * 0.5f,0, rectHeight * 0.5f),
          br( rectWidth * 0.5f,0, rectHeight * 0.5f);
    vector<Vec3f> rect;
    
    Matrix44f mat;
    mat *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI / 4);
    
    QuoteLayoutArea area(mat.transformPointAffine(tl),
                         mat.transformPointAffine(tr),
                         mat.transformPointAffine(bl),
                         mat.transformPointAffine(br));
    
    
    
    mTypesetter = new QuoteTypesetter(&mCells, area);
    
    
    
}

void QuoteLayoutApp::keyDown(KeyEvent event){
    switch (event.getCode()) {
        case KeyEvent::KEY_1:
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_2:
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_UP:
            mCameraZoom = MAX(2,MIN(mCameraZoom+1, 5));
            updateView();
            break;
        case KeyEvent::KEY_DOWN:
            mCameraZoom = MAX(2,MIN(mCameraZoom-1, 5));
            updateView();
            break;
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
            
        default:
            break;
    }
}

void QuoteLayoutApp::mouseDown( MouseEvent event )
{
}

void QuoteLayoutApp::update(){
    
}

void QuoteLayoutApp::updateView(){
    float aspectRatio = app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio * mCameraZoom, aspectRatio * mCameraZoom, -mCameraZoom, mCameraZoom, -10.0f, 10.f);
}

void QuoteLayoutApp::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    gl::drawCoordinateFrame();
    
    mTypesetter->debugDrawArea();
    
    for (auto* cell : mCells) {
        cell->debugDrawArea(mCamera);
    }



}

CINDER_APP_NATIVE( QuoteLayoutApp, RendererGl )
