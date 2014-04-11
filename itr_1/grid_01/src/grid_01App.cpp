#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Resources.h"
#include "Config.h"
#include "Controller.h"
#include "layout/quote/json/QuoteJson.h"
#include "layout/quote/json/QuoteParser.h"
#include "util/ExcInfoPanel.h"

#include "stage/Stage.h"

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

    bool          mInitialConfigIsValid;
    string        mInitialConfigExcMsg;
    
    ExcInfoPanel* mExcPanel;
    StageRef      mStage;
    Controller*   mController;
};

/*--------------------------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------------------------*/

void grid_00App::prepareSettings(Settings* settings){
#ifdef CONFIG_USE_BAKED
    mInitialConfigIsValid = Config::LoadJson(CONFIG_FILE_PATH_BAKED, &mInitialConfigExcMsg);
#else
    mInitialConfigIsValid = Config::LoadJson(app::getAppPath() + "/config.json", &mInitialConfigExcMsg);
#endif
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void grid_00App::setup(){
    mExcPanel = new ExcInfoPanel();
    
    if(!mInitialConfigIsValid){
        mExcPanel->setString(mInitialConfigExcMsg);
        return;
    }
    
    vector<QuoteJson> quoteData;
    if(!QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/grid_01/resources/test.json", &quoteData, &excCatch)){
        mExcPanel->setString(excCatch);
        return;
    }
    
    mStage      = Stage::create(quoteData);
    mController = new Controller(mStage);
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
    if(!Config::IsValid()){
        return;
    }
    
    mController->keyDown(event);
}

/*--------------------------------------------------------------------------------------------*/
// Update / Draw
/*--------------------------------------------------------------------------------------------*/

void grid_00App::update(){
    if(Config::DidChange()){
        Config::Reload(&excCatch);
        if(Config::IsValid()){
            mStage->onConfigDidChange();
            mExcPanel->clear();
        } else {
            mExcPanel->setString(excCatch);
        }
    }
    if(!Config::IsValid()){
        return;
    }
    
    mStage->update();
}

void grid_00App::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    if(!Config::IsValid()){
        mExcPanel->draw();
        return;
    }
    
    mStage->draw();
}

CINDER_APP_NATIVE( grid_00App, RendererGl )
