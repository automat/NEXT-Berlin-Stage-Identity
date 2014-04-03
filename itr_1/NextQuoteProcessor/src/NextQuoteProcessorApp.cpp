#include <vector>
#include <boost/assign/std/vector.hpp>
#include "Config.h"
#include "Controller.h"

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "LayoutArea.h"
#include "Cell.h"
#include "QuoteTypesetter.h"
#include "Quote.h"


#import "cinder/cocoa/CinderCocoa.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace boost::assign;

/*--------------------------------------------------------------------------------------------*/

class NextQuoteProcessorApp : public AppNative {
public:
    void prepareSettings(Settings* settins);
	void setup();
    void resize();
    
    void keyDown(KeyEvent event);
    
    void update();
	void draw();
    
    ControllerRef    mController;
    Grid*            mGrid;
    
    QuoteTypesetter* mTypesetter;
    
};

/*--------------------------------------------------------------------------------------------*/
//  Setup
/*--------------------------------------------------------------------------------------------*/

void NextQuoteProcessorApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void NextQuoteProcessorApp::resize(){
    mController->setCanvasSize(app::getWindowWidth(), app::getWindowHeight());
}

void NextQuoteProcessorApp::setup(){
    mGrid = new Grid(GRID_NUM_XY,GRID_NUM_XY);
    
    //define area for layout
    LayoutArea area(10,5,true);
    area *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI / 4);
    
    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),400.0f),0.7f);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(true);
    mTypesetter->setAlign(QuoteTypesetter::Align::CENTER);
    mTypesetter->debugTexture();
    
    // Init interface
    mController = Controller::create();
    mController->connect(mGrid, mTypesetter);
}

/*--------------------------------------------------------------------------------------------*/
//  Input
/*--------------------------------------------------------------------------------------------*/

void NextQuoteProcessorApp::keyDown(KeyEvent event){
    /*
     if(event.getNativeKeyCode() == 55){ // cmd key
     return;
     }*/
    
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        case KeyEvent::KEY_LEFT:
            mController->prev();
            break;
        case KeyEvent::KEY_RIGHT:
            mController->next();
            break;
        default:
            break;
    }
}

/*--------------------------------------------------------------------------------------------*/
//  Update / Draw
/*--------------------------------------------------------------------------------------------*/

void NextQuoteProcessorApp::update(){
    mController->update();
}

void NextQuoteProcessorApp::draw(){
    gl::clear(Color(0,0,0));
    gl::setMatricesWindow(getWindowSize());
    mController->draw();
}




CINDER_APP_NATIVE( NextQuoteProcessorApp, RendererGl )
