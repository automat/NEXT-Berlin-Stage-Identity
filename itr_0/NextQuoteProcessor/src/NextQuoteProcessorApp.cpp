#include <vector>
#include <boost/assign/std/vector.hpp>
#include "Config.h"
#include "Controller.h"
#include "Properties.h"


#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Json.h"
#include "cinder/params/Params.h"


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
//  Params
/*--------------------------------------------------------------------------------------------*/

static float  PARAM_TYPE_FONT_SCALE(TYPE_FONT_SCALE);
static int    PARAM_TYPE_PADDING_T(TYPE_PADDING_T);
static int    PARAM_TYPE_PADDING_R(TYPE_PADDING_R);
static int    PARAM_TYPE_PADDING_B(TYPE_PADDING_B);
static int    PARAM_TYPE_PADDING_L(TYPE_PADDING_L);

static string TYPE_STRING_LAST("");
static bool   PARAM_TYPE_STRING_CONSTRAIN(TYPE_CONSTRAIN);
static bool   PARAM_TYPE_MANUAL_BREAK(TYPE_MANUAL_BREAK);
static int    PARAM_TYPE_ALIGN(QuoteTypesetter::Align::CENTER);
static bool   PARAM_TYPE_BALANCED_BASELINE(true);
static bool   PARAM_TYPE_DEBUG_TEXTURE(true);
static bool   PARAM_TYPE_SHOW_TEXTURE(true);

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

class NextQuoteProcessorApp : public AppNative {
public:
    void prepareSettings(Settings* settins);
	void setup();
    void resize();
    
    void keyDown(KeyEvent event);
	void fileDrop(FileDropEvent event);
    
    void update();
	void draw();
    
    
    
    
    ControllerRef    mController;
    Grid*            mGrid;
    
    vector<string>   mStrings;
    int              mStringsIndex;
    string           mString;
    
    QuoteTypesetter* mTypesetter;
    
    void updateLayout(const string& str);
    params::InterfaceGlRef mParams;
    void updateParams();
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
    //
    //  Setup props
    //
    
    
    mStringsIndex = 0;
    mStrings += "Alghoritms are the new Art Direction.",
    "Paying by bits but with coins",
    "Knowing what your customers want before they do.",
    "You will be disrupted soon.",
    "You can't choose not to support one device.",
    "Modular content is the new normal.",
    "ABC\n DEF\n GHI"/*,
                      "This\n\n\nis\n\nthe\nNew\n\nNormal."*/;
    
    mGrid = new Grid(GRID_NUM_XY,GRID_NUM_XY);

    //define area for layout
    LayoutArea area(10,5,true);
    area *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI / 4);
    
    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->balanceBaseline();
    mTypesetter->setFont(Font(app::loadResource(RES_FONT_TRANSCRIPT),200.0f),0.7f);
    mTypesetter->setAlign(static_cast<QuoteTypesetter::Align>(PARAM_TYPE_ALIGN));
    mTypesetter->setPadding(0, 0, 0, 1);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(PARAM_TYPE_MANUAL_BREAK);
    mTypesetter->debugTexture();
    
    updateLayout("You can't choose not to support one device.");
    

    // Init interface
    mController = Controller::create();
    mController->connect(mGrid, mTypesetter);
    mController->updateLayout();
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
            /*
        case KeyEvent::KEY_1:
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_2:
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_UP:
            mCameraZoom = MAX(1,MIN(mCameraZoom+1, 8));
            break;
        case KeyEvent::KEY_DOWN:
            mCameraZoom = MAX(1,MIN(mCameraZoom-1, 8));
            break;
             */
        case KeyEvent::KEY_LEFT:
            mStringsIndex = (mStringsIndex - 1) % mStrings.size();
            updateLayout(mStrings[mStringsIndex]);
            break;
        case KeyEvent::KEY_RIGHT:
            mStringsIndex = (mStringsIndex + 1) % mStrings.size();
            updateLayout(mStrings[mStringsIndex]);
            break;
        /*
        case KeyEvent::KEY_BACKSPACE:
            if(mString.size() > 0){
                mString.pop_back();
            }
            updateLayout(mString);
            break;
            
        case KeyEvent::KEY_RETURN:
            mString+= "\n ";
            updateLayout(mString);
            break;
        case KeyEvent::KEY_RSHIFT:
            break;
             */
        default:
            //mString+= event.getChar();
            //updateLayout(mString);
            break;
    }
}

/*--------------------------------------------------------------------------------------------*/
//  Update
/*--------------------------------------------------------------------------------------------*/

void NextQuoteProcessorApp::updateLayout(const string& str){
    if(mTypesetter->setString(str)){
        //mQuote = mTypesetter->getQuote();
        //mInterface->updateLayout();
    }
    TYPE_STRING_LAST = str;
}

void NextQuoteProcessorApp::update(){
   // updateParams();
}

/*--------------------------------------------------------------------------------------------*/
//  Draw
/*--------------------------------------------------------------------------------------------*/

void NextQuoteProcessorApp::draw(){
  
    gl::clear(Color(0,0,0));
    gl::setMatricesWindow(getWindowSize());
    mController->draw();
}


/*--------------------------------------------------------------------------------------------*/
//  Params
/*--------------------------------------------------------------------------------------------*/

void NextQuoteProcessorApp::updateParams(){
    static float paramTypeFontScalePrev       = PARAM_TYPE_FONT_SCALE;
    static bool  paramTypeStringConstrainPrev = PARAM_TYPE_STRING_CONSTRAIN;
    static int   paramTypeAlignPrev           = PARAM_TYPE_ALIGN;
    static bool  paramTypeManualBreak         = PARAM_TYPE_MANUAL_BREAK;
    static int   paramTypePaddingTPrev        = PARAM_TYPE_PADDING_T;
    static int   paramTypePaddingRPrev        = PARAM_TYPE_PADDING_R;
    static int   paramTypePaddingBPrev        = PARAM_TYPE_PADDING_B;
    static int   paramTypePaddingLPrev        = PARAM_TYPE_PADDING_L;
    static bool  paramTypeBalencedBaseline    = PARAM_TYPE_BALANCED_BASELINE;
    static bool  paramTypeDebugTexturePrev    = PARAM_TYPE_DEBUG_TEXTURE;
    
    if(PARAM_TYPE_FONT_SCALE != paramTypeFontScalePrev){
        mTypesetter->setFontScale(PARAM_TYPE_FONT_SCALE);
        updateLayout(TYPE_STRING_LAST);
        
        paramTypeFontScalePrev = PARAM_TYPE_FONT_SCALE;
    }
    
    if(PARAM_TYPE_STRING_CONSTRAIN != paramTypeStringConstrainPrev){
        mTypesetter->constrain(PARAM_TYPE_STRING_CONSTRAIN);
        updateLayout(TYPE_STRING_LAST);
        paramTypeStringConstrainPrev = PARAM_TYPE_STRING_CONSTRAIN;
    }
    
    if(PARAM_TYPE_MANUAL_BREAK != paramTypeManualBreak){
        mTypesetter->manualLineBreak(PARAM_TYPE_MANUAL_BREAK);
        updateLayout(TYPE_STRING_LAST);
        paramTypeManualBreak = PARAM_TYPE_MANUAL_BREAK;
    }
    
    if(PARAM_TYPE_ALIGN != paramTypeAlignPrev){
        mTypesetter->setAlign(static_cast<QuoteTypesetter::Align>(PARAM_TYPE_ALIGN));
        updateLayout(TYPE_STRING_LAST);
        paramTypeAlignPrev = PARAM_TYPE_ALIGN;
    }
    
    if(PARAM_TYPE_BALANCED_BASELINE != paramTypeBalencedBaseline){
        mTypesetter->balanceBaseline(PARAM_TYPE_BALANCED_BASELINE);
        updateLayout(TYPE_STRING_LAST);
        paramTypeBalencedBaseline = PARAM_TYPE_BALANCED_BASELINE;
    }
    
    if(PARAM_TYPE_PADDING_T != paramTypePaddingTPrev ||
       PARAM_TYPE_PADDING_R != paramTypePaddingRPrev ||
       PARAM_TYPE_PADDING_B != paramTypePaddingBPrev ||
       PARAM_TYPE_PADDING_L != paramTypePaddingLPrev){
        
        mTypesetter->setPadding(PARAM_TYPE_PADDING_T,
                                PARAM_TYPE_PADDING_R,
                                PARAM_TYPE_PADDING_B,
                                PARAM_TYPE_PADDING_L);
        updateLayout(TYPE_STRING_LAST);
        paramTypePaddingTPrev = PARAM_TYPE_PADDING_T;
        paramTypePaddingRPrev = PARAM_TYPE_PADDING_R;
        paramTypePaddingBPrev = PARAM_TYPE_PADDING_B;
        paramTypePaddingLPrev = PARAM_TYPE_PADDING_L;
    }
    
    if(PARAM_TYPE_DEBUG_TEXTURE != paramTypeDebugTexturePrev){
        mTypesetter->debugTexture(PARAM_TYPE_DEBUG_TEXTURE);
        updateLayout(TYPE_STRING_LAST);
        paramTypeDebugTexturePrev = PARAM_TYPE_DEBUG_TEXTURE;
    }
    
    
}

void NextQuoteProcessorApp::fileDrop(cinder::app::FileDropEvent event){
    mController->onAppFileDrop(event);
}



CINDER_APP_NATIVE( NextQuoteProcessorApp, RendererGl )
