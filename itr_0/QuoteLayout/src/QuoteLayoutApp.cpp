#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "Config.h"
#include <vector>
#include "Cell.h"
#include "LayoutArea.h"
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
    
    //vector<Cell*>    mCells;
    Grid*            mGrid;
    
    vector<string>   mStrings;
    int              mStringsIndex;
    string           mString;
    QuoteTypesetter* mTypesetter;
    
    CameraOrtho mCamera;
    float       mCameraZoom;
    
    void updateLayout();
    
};

void QuoteLayoutApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void QuoteLayoutApp::setup(){
    mCameraZoom = 1;
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    updateView();

    mGrid = new Grid(GRID_NUM_XY,GRID_NUM_XY);
    
    mStringsIndex = 0;
    mStrings += "Alghoritms are the new Art Direction.",
                "Paying by bits but with coins",
                "Knowing what your customers want before they do.",
                "You will be disrupted soon.",
                "You can't choose not to support one device.",
                "Modular content is the new normal.",
                "ABC\n DEF\n GHI"/*,
                "This\n\n\nis\n\nthe\nNew\n\nNormal."*/;
    
    //define area for layout
    LayoutArea area(10,5,true);
    area *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI / 4);
    
    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->enableManualLineBreak(true);
    mTypesetter->setFont(FONT_NAME,200,0.7f);
    mTypesetter->setAlign(QuoteTypesetter::Align::CENTER);
    mTypesetter->setPadding(0, 0, 0, 1);
    mTypesetter->setString(mStrings[mStringsIndex]);
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
            mCameraZoom = MAX(1,MIN(mCameraZoom+1, 8));
            updateView();
            break;
        case KeyEvent::KEY_DOWN:
            mCameraZoom = MAX(1,MIN(mCameraZoom-1, 8));
            updateView();
            break;
        case KeyEvent::KEY_LEFT:
            mStringsIndex = (mStringsIndex - 1) % mStrings.size();
            mTypesetter->setString(mStrings[mStringsIndex]);
            break;
        case KeyEvent::KEY_RIGHT:
            mStringsIndex = (mStringsIndex + 1) % mStrings.size();
            mTypesetter->setString(mStrings[mStringsIndex]);
            break;
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        case KeyEvent::KEY_BACKSPACE:
            if(mString.size() > 0){
                mString.pop_back();
            }
            updateLayout();
            break;
        case KeyEvent::KEY_RETURN:
            mString+= "\n ";
            updateLayout();
            break;
        
        case KeyEvent::KEY_RSHIFT:
            break;
            
        default:
            mString+= event.getChar();
            updateLayout();
            break;
    }
    
  
   
    
}

void QuoteLayoutApp::updateLayout(){
    mTypesetter->setString(mString);
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
    
    //gl::drawCoordinateFrame();
    glPushMatrix();
    glScalef(0.65f, 0.65f, 0.65f);

    glDisable(GL_DEPTH_TEST);
    mTypesetter->debugDrawArea();
 
    const vector<Cell*>& cells = mGrid->getCells();
    
    glColor3f(0.5f,0,0);
    for (auto* cell : cells) {
        cell->debugDrawArea();
    }
    mGrid->debugDrawIndices(mCamera);
    glEnable(GL_DEPTH_TEST);
    
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    gl::enableAdditiveBlending();
    glColor3f(1,1,1);
    mTypesetter->debugDrawString();
    gl::enableAdditiveBlending();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    glPopMatrix();


}

CINDER_APP_NATIVE( QuoteLayoutApp, RendererGl )
