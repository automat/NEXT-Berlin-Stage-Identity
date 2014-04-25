#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Resources.h"
#include "Config.h"

#include "util/ExcInfoPanel.h"
#include "quote/json/QuoteJson.h"
#include "quote/json/QuoteParser.h"

#include "stage/Stage.h"

using namespace ci;
using namespace ci::app;
using namespace std;

string excCatch;

/*--------------------------------------------------------------------------------------------*/

class StageApp : public AppNative {
public:
    void prepareSettings(Settings* settings);
	void setup();
	void keyDown( KeyEvent event);
    void update();
	void draw();
    
    bool    mInitialConfigIsValid;
    string  mInitialConfigExcMsg;
    
    next::util::ExcInfoPanel* mExcPanel;
    next::StageRef            mStage;
};

/*--------------------------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------------------------*/

void StageApp::prepareSettings(Settings* settings){
#ifdef CONFIG_USE_BAKED
    mInitialConfigIsValid = next::Config::LoadJson(CONFIG_FILE_PATH_BAKED, &mInitialConfigExcMsg);
#else
    mInitialConfigIsValid = next::Config::LoadJson(app::getAppPath() + "/config.json", &mInitialConfigExcMsg);
#endif
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void StageApp::setup(){
    mExcPanel = new next::util::ExcInfoPanel();
    
    if(!mInitialConfigIsValid){
        mExcPanel->setString(mInitialConfigExcMsg);
        return;
    }
    
    vector<next::QuoteJson> quoteData;
    if(!next::QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/Stage/resources/test.json", &quoteData, &excCatch)){
        mExcPanel->setString(excCatch);
        return;
    }
    
    mStage = next::Stage::create(quoteData);
}

/*--------------------------------------------------------------------------------------------*/
// Input
/*--------------------------------------------------------------------------------------------*/

void StageApp::keyDown( KeyEvent event ){
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        default:
            break;
    }

    if(!next::Config::IsValid()){
        return;
    }
    switch (event.getCode()) {
        case KeyEvent::KEY_UP:
            mStage->zoomModelIn();
            break;
        case KeyEvent::KEY_DOWN:
            mStage->zoomModelOut();
            break;
        case KeyEvent::KEY_LEFT:
            mStage->viewOrtho();
            break;
        case KeyEvent::KEY_RIGHT:
            mStage->viewTop();
        default:
            break;
    }
}

/*--------------------------------------------------------------------------------------------*/
// Update / Draw
/*--------------------------------------------------------------------------------------------*/

void StageApp::update(){
    if(next::Config::DidChange()){
        next::Config::Reload(&excCatch);
        if(next::Config::IsValid()){
            mStage->onConfigDidChange();
            mExcPanel->clear();
        } else {
            mExcPanel->setString(excCatch);
        }
    }
    if(!next::Config::IsValid()){
        return;
    }
    
    mStage->update();
}

void StageApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    if(!next::Config::IsValid()){
        mExcPanel->draw();
        return;
    }
    
    mStage->draw();
}

CINDER_APP_NATIVE( StageApp, RendererGl )
