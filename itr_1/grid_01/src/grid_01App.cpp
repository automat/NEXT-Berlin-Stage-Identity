#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Resources.h"
#include "Config.h"
#include "Controller.h"
#include "layout/quote/json/QuoteJson.h"
#include "layout/quote/json/QuoteParser.h"

#include "world/World.h"


/*--------------------------------------------------------------------------------------------*/


using namespace ci;
using namespace ci::app;
using namespace std;

/*--------------------------------------------------------------------------------------------*/

class grid_00App : public AppNative {
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

void grid_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void grid_00App::setup(){
    vector<QuoteJson> quoteData;
    string msg;
    if(!QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/grid_01/resources/test.json", &quoteData, &msg)){
        throw msg;
    }
    
    mWorld = World::create(quoteData);
    mController = new Controller(mWorld);
}


/*--------------------------------------------------------------------------------------------*/
// Input
/*--------------------------------------------------------------------------------------------*/

void grid_00App::keyDown( KeyEvent event ){
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

void grid_00App::update(){
    mWorld->update();
}

void grid_00App::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mWorld->draw();
}

CINDER_APP_NATIVE( grid_00App, RendererGl )
