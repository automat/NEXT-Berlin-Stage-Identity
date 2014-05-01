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
#include <getopt.h>

using namespace ci;
using namespace ci::app;
using namespace std;

string   excCatch;

//
//  args
//

string pathFileConfigJson;
string pathFolderImagesSpeaker;
string pathFolderImagesClocks;
string pathFileQuoteJson;
string pathFileDataJson;
int    session_id(-1);

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
    mInitialConfigIsValid = next::Config::LoadJson(pathFileConfigJson, &mInitialConfigExcMsg);

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
    if(!next::QuoteParser::LoadJson(pathFileQuoteJson, mDataQuotes, &excCatch)){
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
    next::Mapping::Get(pathFileDataJson, pathFolderImagesSpeaker, pathFolderImagesClocks,
                       session_id, mImagesClocks, mImagesSpeakers,
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
// Update
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

/*--------------------------------------------------------------------------------------------*/
// Draw
/*--------------------------------------------------------------------------------------------*/

void StageApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
 
    //
    //  Display err
    //
    if(!next::Config::IsValid()){
        mExcPanel->draw();
        return;
    }
    
    //
    //  Update fbo stage
    //
    mStage->draw();
    
    
    //
    //  Split stage according to projection blending edge
    //
    gl::setMatricesWindow(APP_SIZE,false);
    const gl::Texture& stageTexture = mStage->getTexture();
    glColor3f(1, 1, 1);
    gl::draw(stageTexture);

    
}

/*--------------------------------------------------------------------------------------------*/
// Init
/*--------------------------------------------------------------------------------------------*/

//
// Override Cinders default main
//

//CINDER_APP_NATIVE( StageApp, RendererGl )

using namespace boost;
int main( int argc, char * const argv[] ) {
    int argc_ = !strcmp(argv[argc-2],"-NSDocumentRevisionsDebugMode") ? argc - 2 :  argc;
    char *argv_[argc_];
    std::copy(argv, argv + argc_, argv_);
    
    //  -c  :   path file config.json
    //  -d  :   path file session data.json
    //  -q  :   path file quote.json
    //  -i  :   path folder speaker images
    //  -h  :   path folder clock images
    //  -s  :   session id

    char args[] = ":c:i:h:d:q:s:";
    
    int oc;
    while ((oc = getopt(argc_, argv_, args)) != -1) {
        if(optarg[0] == '-'){ // arg value not supplied
            break;
        }
        switch (oc) {
            case 'c':   //  path file config.json
                pathFileConfigJson = string(optarg);
                break;
            case 'i':
                pathFolderImagesSpeaker = string(optarg);
                break;
            case 'h':
                pathFolderImagesClocks = string(optarg);
                break;
            case 'q':   //  path file quote.json
                pathFileQuoteJson = string(optarg);
                break;
            case 'd':   //  path file session data.json
                pathFileDataJson = string(optarg);
                break;
            case 's':   // session id
                try {
                    session_id = boost::lexical_cast<int>(optarg);
                } catch (boost::bad_lexical_cast& ecx) {
                    session_id = -1;
                }
                session_id = session_id < -1 ? -1 : session_id;
                break;
            case '?':
                break;
            default:
                break;
        }
    }
    
    if(pathFileConfigJson.empty()){
        printf("config filepath missing\n");
        return 1;
    }
    
    if(pathFolderImagesSpeaker.empty()){
        printf("speaker images folder path missing\n");
        return 1;
    }
    
    if(pathFolderImagesClocks.empty()){
        printf("clock images folder path missing\n");
        return 1;
    }
    
    if(pathFileDataJson.empty()){
        printf("data filepath missing\n");
        return 1;
    }
    
    if(pathFileQuoteJson.empty()){
        printf("quote filepath missing\n");
        return 1;
    }
    
    if(session_id == -1){
        printf("session id missing\n");
        return 1;
    }
    
    cinder::app::AppBasic::prepareLaunch();
    cinder::app::AppBasic *app = new StageApp();
    cinder::app::RendererRef ren( new RendererGl() );
    cinder::app::AppBasic::executeLaunch( app, ren, "StageApp", argc, argv );
    cinder::app::AppBasic::cleanupLaunch();
    return 0;
}

