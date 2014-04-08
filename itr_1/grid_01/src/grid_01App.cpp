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
private:
    bool mValid;
public:
    void prepareSettings(Settings* settings);
	void setup();
	void keyDown( KeyEvent event);
    void update();
	void draw();
    
    WorldRef    mWorld;
    Controller* mController;
};

/*--------------------------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------------------------*/

void grid_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void grid_00App::setup(){
    mValid = true;
   
    string msg;
    string configFilePath;
    
#ifdef CONFIG_USE_BAKED
    configFilePath = app::loadResource(CONFIG_FILE_PATH_BAKED)->getFilePath().string();
#else
    configFilePath = app::getAppPath() + "/config.json";
#endif

    //
    //  Load config & quote json
    //
    
    if(!Config::LoadJson(configFilePath, &msg)){
        mValid = false;
        throw msg; // error panel will open here
        return;
    }
    
    vector<QuoteJson> quoteData;
    if(!QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/grid_01/resources/test.json", &quoteData, &msg)){
        mValid = false;
        throw msg;  // error panel will open here
        return;
    }
    
    mWorld       = World::create(quoteData);
    mController = new Controller(mWorld);
}

/*--------------------------------------------------------------------------------------------*/
// Input
/*--------------------------------------------------------------------------------------------*/

void grid_00App::keyDown( KeyEvent event ){
    if(!mValid){
        return;
    }
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
    if(!mValid){
        return;
    }
    mWorld->update();
}

void grid_00App::draw(){
    if(!mValid){
        return;
    }
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mWorld->draw();
}

CINDER_APP_NATIVE( grid_00App, RendererGl )
