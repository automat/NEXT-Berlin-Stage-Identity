#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Resources.h"
#include "Config.h"

#include "util/ExcInfoPanel.h"
#include "data/quote/json/QuoteJson.h"
#include "data/quote/json/QuoteParser.h"

#include "data/session/Speaker.h"
#include "data/session/Event.h"
#include "data/session/Session.h"
#include "data/session/Mapping.h"

#include "stage/Stage.h"
#include <stdlib.h>

using namespace ci;
using namespace ci::app;
using namespace std;

string   excCatch;
uint32_t session_id;
string   configPath;
string   dataPath;


/*--------------------------------------------------------------------------------------------*/

class StageApp : public AppNative {
public:
    ~StageApp();
    
    void prepareSettings(Settings* settings);
	void setup();
	void keyDown( KeyEvent event);
    void update();
	void draw();
    
    bool    mInitialConfigIsValid;
    string  mInitialConfigExcMsg;
    
    map<uint32_t, gl::Texture>*   mImagesClocks;
    map<uint32_t, gl::Texture>*   mImagesSpeakers;
    map<uint32_t, next::Speaker>* mDataSpeakers;
    map<uint32_t, next::Event>*   mDataEvents;
    next::Session*                mDataSession;
    
    vector<next::QuoteJson>* mDataQuotes;
    
    next::util::ExcInfoPanel* mExcPanel;
    next::Stage*              mStage;
};

StageApp::~StageApp(){
    delete mStage;
    delete mDataSession;
    delete mDataEvents;
    delete mDataSpeakers;
    delete mImagesSpeakers;
    delete mImagesClocks;
}

/*--------------------------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------------------------*/

void StageApp::prepareSettings(Settings* settings){
#ifdef CONFIG_USE_BAKED
    mInitialConfigIsValid = next::Config::LoadJson(CONFIG_FILE_PATH_BAKED, &mInitialConfigExcMsg);
#else
    mInitialConfigIsValid = next::Config::LoadJson(app::getAppPath() + "/config.json", &mInitialConfigExcMsg);
#endif
    settings->setWindowPos(0, 0);
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
    //settings->setBorderless(true);
}

void StageApp::setup(){
    
    mExcPanel = new next::util::ExcInfoPanel();
    
    if(!mInitialConfigIsValid){
        mExcPanel->setString(mInitialConfigExcMsg);
        return;
    }
    
    //
    //  Load data - json quotes
    //
    mDataQuotes = new vector<next::QuoteJson>();
    if(!next::QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/Stage/resources/test.json", mDataQuotes, &excCatch)){
        mExcPanel->setString(excCatch);
        return;
    }
    
    //
    //  Load data - json session
    //
    mImagesClocks   = nullptr;
    mImagesSpeakers = nullptr;
    mDataSpeakers   = nullptr;
    mDataEvents     = nullptr;
    mDataSession    = nullptr;
    //3562 //3559
    next::Mapping::Get(3558/*atoi(getArgs()[1].c_str())*/ , mImagesClocks, mImagesSpeakers,
                       mDataSpeakers, mDataEvents, mDataSession);
    
    //
    //  Init
    //
    mStage = new next::Stage(mDataQuotes, mDataSession, mDataSpeakers);
    
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
}

/*--------------------------------------------------------------------------------------------*/
// Update / Draw
/*--------------------------------------------------------------------------------------------*/

void StageApp::update(){
#ifdef APP_HIDE_CURSOR
    hideCursor();
#endif
    
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

//CINDER_APP_NATIVE( StageApp, RendererGl )
// Override Cinders default main
//
int main( int argc, char * const argv[] ) {
    int opt;
    while((opt = getopt(argc, argv, "dec") != -1)){
        switch (opt) {
            case 'e':   // data path
                cout << optarg << endl;
                break;
                
            default:
                break;
        }
    }
    
    cinder::app::AppBasic::prepareLaunch();
    cinder::app::AppBasic *app = new StageApp();
    cinder::app::RendererRef ren( new RendererGl() );
    cinder::app::AppBasic::executeLaunch( app, ren, "StageApp", argc, argv );
    cinder::app::AppBasic::cleanupLaunch();
    return 0;
}

