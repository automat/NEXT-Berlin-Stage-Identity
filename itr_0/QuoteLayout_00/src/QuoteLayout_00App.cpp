#include <vector>
#include <boost/assign/std/vector.hpp>
#include "Config.h"

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"


#include "LayoutArea.h"
#include "Cell.h"
#include "QuoteTypesetter.h"
#include "Quote.h"

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

class QuoteLayout_00App : public AppNative {
public:
    void prepareSettings(Settings* settins);
	void setup();
    void keyDown(KeyEvent event);
	void update();
	void draw();
    
    Grid*            mGrid;
    
    vector<string>   mStrings;
    int              mStringsIndex;
    string           mString;
    QuoteTypesetter* mTypesetter;
    QuoteRef         mQuote;
    gl::Texture      mTypeTexture;
    
    CameraOrtho mCamera;
    float       mCameraZoom;
    
    gl::Fbo mFbo;
    
    void updateLayout(const string& str);
    params::InterfaceGlRef mParams;
    void updateParams();
};

/*--------------------------------------------------------------------------------------------*/
//  Setup
/*--------------------------------------------------------------------------------------------*/

void QuoteLayout_00App::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void QuoteLayout_00App::setup(){
    //
    //  Setup props
    //
    
    mCameraZoom = 1;
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    mGrid = new Grid(GRID_NUM_XY,GRID_NUM_XY);
    
    mStringsIndex = 0;
    mStrings += "Alghoritms are the new Art Direction.",
                "Paying by bits but with coins",
                "Knowing what your customers want before they do.",
                "You will be disrupted soon.",
                "You can't choose not to support one device.",
                "Modular content is the new normal.",
                "ABC\n DEF\n GHI"/*,
                      "This\n\n\nis\n\nthe\nNew\n\nNormal."*/;
    
    //define area for layout
    LayoutArea area(10,5,true);
    area *= Matrix44f::createRotation(Vec3f::yAxis(), M_PI / 4);
    
    mTypesetter = new QuoteTypesetter(mGrid, area);
    mTypesetter->balanceBaseline();
    mTypesetter->setFont(TYPE_FONT_NAME,200,0.7f);
    mTypesetter->setAlign(static_cast<QuoteTypesetter::Align>(PARAM_TYPE_ALIGN));
    mTypesetter->setPadding(0, 0, 0, 1);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(PARAM_TYPE_MANUAL_BREAK);
    mTypesetter->debugTexture();

    
    mFbo = gl::Fbo(100,100);
    mFbo.bindFramebuffer();
    gl::setViewport(mFbo.getBounds());
    gl::clear(Color(1,0,1));
    mFbo.unbindFramebuffer();
    
    updateLayout("Small\nstring");

    //
    //  Setup params
    //
    
    vector<string> alignEnumNames = {"left","center","right"};
    
    mParams = params::InterfaceGl::create("controls", Vec2f(200,250));
    mParams->addParam("Font scale", &PARAM_TYPE_FONT_SCALE, "min=0 max=1 step=0.001");
    mParams->addParam("Padding T",  &PARAM_TYPE_PADDING_T, "min=0 max=5 step=1");
    mParams->addParam("Padding R",  &PARAM_TYPE_PADDING_R, "min=0 max=5 step=1");
    mParams->addParam("Padding B",  &PARAM_TYPE_PADDING_B, "min=0 max=5 step=1");
    mParams->addParam("Padding L",  &PARAM_TYPE_PADDING_L, "min=0 max=5 step=1");
    mParams->addParam("Align", alignEnumNames, &PARAM_TYPE_ALIGN);
    mParams->addParam("Constrain",        &PARAM_TYPE_STRING_CONSTRAIN);
    mParams->addParam("Manual Break",     &PARAM_TYPE_MANUAL_BREAK);
    mParams->addParam("Show Texture",     &PARAM_TYPE_SHOW_TEXTURE);
    mParams->addParam("Debug Texture",    &PARAM_TYPE_DEBUG_TEXTURE);
    mParams->addParam("Balance Baseline", &PARAM_TYPE_BALANCED_BASELINE);
    
    //glEnable(GL_SCISSOR_TEST);
}

/*--------------------------------------------------------------------------------------------*/
//  Input
/*--------------------------------------------------------------------------------------------*/

void QuoteLayout_00App::keyDown(KeyEvent event){
    /*
    if(event.getNativeKeyCode() == 55){ // cmd key
        return;
    }*/
    
    switch (event.getCode()) {
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
        case KeyEvent::KEY_LEFT:
            mStringsIndex = (mStringsIndex - 1) % mStrings.size();
            updateLayout(mStrings[mStringsIndex]);
            break;
        case KeyEvent::KEY_RIGHT:
            mStringsIndex = (mStringsIndex + 1) % mStrings.size();
            updateLayout(mStrings[mStringsIndex]);
            break;
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
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
        default:
            mString+= event.getChar();
            updateLayout(mString);
            break;
    }
}

/*--------------------------------------------------------------------------------------------*/
//  Update
/*--------------------------------------------------------------------------------------------*/

void QuoteLayout_00App::updateLayout(const string& str){
    if(mTypesetter->setString(str)){
        mQuote = mTypesetter->getQuote();
    }
    TYPE_STRING_LAST = str;
}

void QuoteLayout_00App::update(){
    updateParams();
}

/*--------------------------------------------------------------------------------------------*/
//  Draw
/*--------------------------------------------------------------------------------------------*/

void QuoteLayout_00App::draw(){
    static const float windowWidth  = getWindowWidth();
    static const float windowHeight = getWindowHeight();
    
    static float viewportWidth;
    static float viewportHeight;
    static float viewportPosX;
    static float viewportPosY;
    
    static bool paramTypeShowTexturePrev = !PARAM_TYPE_SHOW_TEXTURE;
    if(PARAM_TYPE_SHOW_TEXTURE != paramTypeShowTexturePrev){
        if(PARAM_TYPE_SHOW_TEXTURE){
            viewportWidth  = windowWidth - windowHeight;
            viewportHeight = windowHeight;//viewportWidth *  windowRatio;
            viewportPosX   = 0;
            viewportPosY   = (windowHeight - viewportHeight) * 0.5f;
        } else {
            viewportWidth  = windowWidth;
            viewportHeight = windowHeight;
            viewportPosX   = 0;
            viewportPosY   = 0;
        }
        paramTypeShowTexturePrev = PARAM_TYPE_SHOW_TEXTURE;
    }

    static float viewportWidthPrev = -1;
    
    if(viewportWidth != viewportWidthPrev){
        float viewportRatio = viewportWidth / viewportHeight;
        float cameraMult    = windowWidth / viewportWidth;
        float cameraZoom    = mCameraZoom * cameraMult;
        
        mCamera.setOrtho(-viewportRatio * cameraZoom, viewportRatio * cameraZoom, -cameraZoom, cameraZoom, -10.0f, 10.f);
        viewportWidthPrev = viewportWidth;
    }
    
    //
    //  View Debug
    //
    
    glPushAttrib(GL_VIEWPORT_BIT);
        gl::clear( Color( 0, 0, 0 ) );
    
        gl::setViewport(Area(Rectf(0,viewportPosY,viewportWidth,viewportPosY+viewportHeight)));
        gl::setMatrices(mCamera);
    
        // gl::drawCoordinateFrame(4);
    
        glPushMatrix();
            glScalef(0.65f, 0.65f, 0.65f);

            gl::disableDepthRead();
            mTypesetter->debugDrawArea();
            const vector<Cell*>& cells = mGrid->getCells();
    
            glColor3f(0.5f,0,0);
            for (auto* cell : cells) {
                cell->debugDrawArea();
            }
            mGrid->debugDrawIndices(mCamera);
    
            gl::enableDepthRead();
            gl::enableAlphaTest();
            gl::enableAlphaBlending();
            gl::enableAdditiveBlending();
            glColor3f(1,1,1);
            mTypesetter->debugDrawString();
            gl::disableAlphaBlending();
            gl::disableAlphaTest();
            gl::disableDepthRead();
    
        glPopMatrix();
    glPopAttrib();
  
    //
    //  View Texture
    //
    
    if(PARAM_TYPE_SHOW_TEXTURE && mQuote != nullptr){
        static const float padding = 50.0f;
        static const Vec2f trans   = Vec2f(windowWidth - windowHeight + padding, padding);
        static const Rectf rect    = Rectf(0,0,windowHeight - padding * 2, windowHeight - padding * 2);
        
        glPushAttrib(GL_VIEWPORT_BIT);
        gl::disableDepthRead();
            gl::setViewport(getWindowBounds());
            gl::setMatricesWindow(getWindowSize());
            glPushMatrix();
            glTranslatef(trans.x,trans.y,0);
                const gl::Texture& texture = mQuote->getTexture();
                glColor3f(0.5f, 0, 0.125f);
                gl::drawStrokedRect(rect);
                glEnable(GL_TEXTURE_2D);
                glColor3f(1, 1, 1);
                gl::draw(texture,rect);
                //texture.bind();
                //gl::drawSolidRect(rect);
                //texture.unbind();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        gl::enableDepthRead();
        glPopAttrib();
    }
    
    mParams->draw();
}


/*--------------------------------------------------------------------------------------------*/
//  Params
/*--------------------------------------------------------------------------------------------*/

void QuoteLayout_00App::updateParams(){
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



CINDER_APP_NATIVE( QuoteLayout_00App, RendererGl )
