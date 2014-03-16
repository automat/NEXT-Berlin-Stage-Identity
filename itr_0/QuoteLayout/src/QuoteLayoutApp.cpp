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
    
    vector<Cell*>    mCells;
    string           mString;
    QuoteTypesetter* mTypesetter;
    
    CameraOrtho mCamera;
    float       mCameraZoom;
    
};

void QuoteLayoutApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void QuoteLayoutApp::setup(){
    mCameraZoom = 1;
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
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
    
    vector<string> strings;
    strings += "Alghoritms are the new Art Direction.",
               "Paying by bits but with coins",
               "Knowing what your customers want before they do.",
               "You will be disrupted soon.",
               "You can't choose not to support one device.",
               "Modular content is the new normal.",
               "ABC\n DEF\n GHI",
               "This\n\n\nis\n\nthe\nNew\n\nNormal.";
    
    //define area for layout
    LayoutArea area(10,5,true);
    area *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI / 4);
    
    mTypesetter = new QuoteTypesetter(&mCells, area);
    mTypesetter->enableManualLineBreak(true);
    mTypesetter->setFont(Font(FONT_NAME,200),0.7f);
    mTypesetter->setAlign(QuoteTypesetter::Align::CENTER);
    mTypesetter->setPadding(0, 0, 0, 0);
    
    if(!mTypesetter->setString(strings[6])){
        //cout << "Can´t set string: " << strings[3] << endl;
    }
    
    
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
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        case KeyEvent::KEY_BACKSPACE:
            if(mString.size() > 0){
                mString.pop_back();
            }
            break;
        case KeyEvent::KEY_RETURN:
            mString+= "\n ";
            break;
        
        case KeyEvent::KEY_RSHIFT:
            break;
            
        default:
            mString+= event.getChar();
            break;
    }
    
  
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
 
    for (auto* cell : mCells) {
        cell->debugDrawArea(mCamera);
    }
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
