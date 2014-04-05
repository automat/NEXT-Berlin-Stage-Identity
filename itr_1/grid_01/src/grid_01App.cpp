#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Config.h"
#include "world/World.h"

#include "Controller.h"

/*--------------------------------------------------------------------------------------------*/


using namespace ci;
using namespace ci::app;
using namespace std;

/*--------------------------------------------------------------------------------------------*/

class grid_01App : public AppNative {
public:
    void prepareSettings(Settings* settings);
	void setup();
	void keyDown( KeyEvent event);
    void update();
	void draw();
    
    WorldRef mWorld;
    
    Controller* mController;
    
    
};

/*--------------------------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------------------------*/

void grid_01App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_01App::setup(){
    mWorld = World::create();
    mController = new Controller(mWorld);
}


/*--------------------------------------------------------------------------------------------*/
// Input
/*--------------------------------------------------------------------------------------------*/

void grid_01App::keyDown( KeyEvent event ){
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        default:
            break;
    }
    
    mController->keyDown(event);
}

/*--------------------------------------------------------------------------------------------*/
// Update / Draw
/*--------------------------------------------------------------------------------------------*/

void grid_01App::update(){
    mWorld->update();
}

void grid_01App::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mWorld->draw();
}

CINDER_APP_NATIVE( grid_01App, RendererGl )
