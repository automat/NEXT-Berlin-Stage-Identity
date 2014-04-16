#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/qtime/MovieWriter.h"
#include "cinder/ip/Fill.h"

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

class grid_01_render_qtApp : public AppNative {
public:
    void prepareSettings(Settings* settings);
	void setup();
	void keyDown( KeyEvent event);
    void update();
	void draw();
    
    bool          mInitialConfigIsValid;
    string        mInitialConfigExcMsg;
    bool          mRecord;
    
    ExcInfoPanel* mExcPanel;
    StageRef      mStage;
    Controller*   mController;
    
    qtime::MovieWriterRef	mMovieWriter;
};

/*--------------------------------------------------------------------------------------------*/
// Setup
/*--------------------------------------------------------------------------------------------*/

void grid_01_render_qtApp::prepareSettings(Settings* settings){
#ifdef CONFIG_USE_BAKED
    mInitialConfigIsValid = Config::LoadJson(CONFIG_FILE_PATH_BAKED, &mInitialConfigExcMsg);
#else
    mInitialConfigIsValid = Config::LoadJson(app::getAppPath() + "/config.json", &mInitialConfigExcMsg);
#endif
    settings->setWindowSize(APP_WIDTH,APP_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void grid_01_render_qtApp::setup(){
    mExcPanel = new ExcInfoPanel();
    
    if(!mInitialConfigIsValid){
        mExcPanel->setString(mInitialConfigExcMsg);
        return;
    }
    
    vector<QuoteJson> quoteData;
    if(!QuoteParser::LoadJson("/Users/automat/Projects/next/itr_1/grid_01_render/resources/test.json", &quoteData, &excCatch)){
        mExcPanel->setString(excCatch);
        return;
    }
    
    mStage      = Stage::create(quoteData);
    mController = new Controller(mStage);
    mRecord = false;
    
    fs::path path = getSaveFilePath();
	if( path.empty() )
    return; // user cancelled save
    
	qtime::MovieWriter::Format format;
	if( qtime::MovieWriter::getUserCompressionSettings( &format ) ) {
		mMovieWriter = qtime::MovieWriter::create( path, getWindowWidth(), getWindowHeight(), format );
	}
}

/*--------------------------------------------------------------------------------------------*/
// Input
/*--------------------------------------------------------------------------------------------*/

void grid_01_render_qtApp::keyDown( KeyEvent event ){
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
        quit();
        break;
        case KeyEvent::KEY_s:
        mRecord = true;
        break;
        case KeyEvent::KEY_d:
        mStage->releaseTheDingo();
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

void grid_01_render_qtApp::update(){
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

void grid_01_render_qtApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    if(!Config::IsValid()){
        mExcPanel->draw();
        return;
    }
    
    mStage->draw();
    
    
	if( mMovieWriter && mRecord)
    mMovieWriter->addFrame( copyWindowSurface() );
}

CINDER_APP_NATIVE( grid_01_render_qtApp, RendererGl )
