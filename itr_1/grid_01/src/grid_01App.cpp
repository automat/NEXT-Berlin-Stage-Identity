#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Resources.h"
#include "Config.h"
#include "Controller.h"
#include "layout/quote/json/QuoteJson.h"
#include "layout/quote/json/QuoteParser.h"
#include "util/ExcInfoPanel.h"

#include "world/World.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace utils;

string excCatch;

/*--------------------------------------------------------------------------------------------*/

class grid_00App : public AppNative {
public:
    void prepareSettings(Settings* settings);
	void setup();
	void keyDown( KeyEvent event);
    void update();
	void draw();

    ExcInfoPanel* mExcPanel;
    WorldRef      mWorld;
    Controller*   mController;
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
    mExcPanel = new ExcInfoPanel();
    string configFilePath;
    
#ifdef CONFIG_USE_BAKED
    configFilePath = CONFIG_FILE_PATH_BAKED;
#else
    configFilePath = app::getAppPath() + "/config.json";
#endif

    if(!Config::LoadJson(configFilePath, &excCatch)){
        mExcPanel->setString(excCatch);
        return;
    }
    
    vector<QuoteJson> quoteData;
    if(!QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/grid_01/resources/test.json", &quoteData, &excCatch)){
        mExcPanel->setString(excCatch);
        return;
    }
    
    mWorld      = World::create(quoteData);
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
    if(Config::DidChange(&excCatch)){
        cout << Config::IsValid() << endl;
        if(!Config::IsValid()){
            mExcPanel->setString(excCatch);
        } else {
            mExcPanel->clear();
        }
    }
    
   /*
    string str;
    if(Config::DidChange(&str)){
        // error panel will open here after checking if change was valid
        mWorld->onConfigDidChange();
    }
    
    if(Config::IsValid()){
        cout << "hell" << endl;
    */
     //   mWorld->update();
    /*
    } else {
        mExcPanel->setString(str);
    }*/
}

void grid_00App::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    /*
    if(Config::IsValid()){*/
     //   mWorld->draw();
    /*} else {
        mExcPanel->draw();
    }*/
    
    mExcPanel->draw();
}

CINDER_APP_NATIVE( grid_00App, RendererGl )
