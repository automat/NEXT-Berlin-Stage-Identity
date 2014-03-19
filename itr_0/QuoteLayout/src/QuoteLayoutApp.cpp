#include <vector>
#include <boost/assign/std/vector.hpp>
#include "Config.h"

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#ifdef USE_PARAMS
#include "cinder/params/Params.h"
#endif


#include "LayoutArea.h"
#include "Cell.h"
#include "QuoteTypesetter.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace boost::assign;

#ifdef USE_PARAMS
static float  PARAM_TYPE_FONT_SCALE(TYPE_FONT_SCALE);
static int    PARAM_TYPE_PADDING_T(TYPE_PADDING_T);
static int    PARAM_TYPE_PADDING_R(TYPE_PADDING_R);
static int    PARAM_TYPE_PADDING_B(TYPE_PADDING_B);
static int    PARAM_TYPE_PADDING_L(TYPE_PADDING_L);

static string TYPE_STRING_LAST("");
static bool   PARAM_TYPE_STRING_CONSTRAIN(TYPE_CONSTRAIN);
static bool   PARAM_TYPE_MANUAL_BREAK(TYPE_MANUAL_BREAK);
static int    PARAM_TYPE_ALIGN(QuoteTypesetter::Align::CENTER);
static bool   PARAM_TYPE_SHOW_TEXTURE(true);
#endif

class QuoteLayoutApp : public AppNative {
  public:
    void prepareSettings(Settings* settins);
	void setup();
    void keyDown(KeyEvent event);
	void mouseDown( MouseEvent event );
    void updateView();
	void update();
	void draw();

    Grid*            mGrid;
    
    vector<string>   mStrings;
    int              mStringsIndex;
    string           mString;
    QuoteTypesetter* mTypesetter;
    gl::Texture      mTypeTexture;
    
    CameraOrtho mCamera;
    float       mCameraZoom;
    
    void updateLayout(const string& str);
    void updateTexture();
#ifdef USE_PARAMS
    params::InterfaceGlRef mParams;
    void updateParams();
#endif
};

void QuoteLayoutApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
    settings->setResizable(false);
}

void QuoteLayoutApp::setup(){
    mCameraZoom = 1;
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    //mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
    updateView();

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
    mTypesetter->setFont(TYPE_FONT_NAME,200,0.7f);
    mTypesetter->setAlign(static_cast<QuoteTypesetter::Align>(PARAM_TYPE_ALIGN));
    mTypesetter->setPadding(0, 0, 0, 1);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(PARAM_TYPE_MANUAL_BREAK);
    mTypesetter->debugTexture();
    
    //updateLayout("\n\n\nSmall\n\n\n\nstring\n\n \n\n,and\n\n");
    updateLayout("Small\nstring");
    
#ifdef USE_PARAMS
    vector<string> alignEnumNames = {"left","center","right"};
    
    mParams = params::InterfaceGl::create("controls", Vec2f(200,200));
    mParams->addParam("Font scale", &PARAM_TYPE_FONT_SCALE, "min=0 max=1 step=0.001");
    mParams->addParam("Padding T",  &PARAM_TYPE_PADDING_T, "min=0 max=5 step=1");
    mParams->addParam("Padding R",  &PARAM_TYPE_PADDING_R, "min=0 max=5 step=1");
    mParams->addParam("Padding B",  &PARAM_TYPE_PADDING_B, "min=0 max=5 step=1");
    mParams->addParam("Padding L",  &PARAM_TYPE_PADDING_L, "min=0 max=5 step=1");
    mParams->addParam("Align", alignEnumNames, &PARAM_TYPE_ALIGN);
    mParams->addParam("Constrain",  &PARAM_TYPE_STRING_CONSTRAIN);
    mParams->addParam("Manual Break", &PARAM_TYPE_MANUAL_BREAK);
    mParams->addParam("Show Texture", &PARAM_TYPE_SHOW_TEXTURE);
    
    glEnable(GL_SCISSOR_TEST);
    
#endif
}

void QuoteLayoutApp::keyDown(KeyEvent event){
    switch (event.getCode()) {
        case KeyEvent::KEY_1:
            mCamera.lookAt(Vec3f(0,1,0),Vec3f::zero());
            break;
        case KeyEvent::KEY_2:
            mCamera.lookAt(Vec3f(1,1,1),Vec3f::zero());
            break;
        case KeyEvent::KEY_UP:
            mCameraZoom = MAX(1,MIN(mCameraZoom+1, 8));
            updateView();
            break;
        case KeyEvent::KEY_DOWN:
            mCameraZoom = MAX(1,MIN(mCameraZoom-1, 8));
            updateView();
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

void QuoteLayoutApp::updateLayout(const string& str){
    if(mTypesetter->setString(str)){
        updateTexture();
    }
#ifdef USE_PARAMS
    TYPE_STRING_LAST = str;
#endif
}


void QuoteLayoutApp::updateView(){
    //float aspectRatio = app::getWindowAspectRatio();
    //mCamera.setOrtho(-aspectRatio * mCameraZoom, aspectRatio * mCameraZoom, -mCameraZoom, mCameraZoom, -10.0f, 10.f);
}

void QuoteLayoutApp::updateTexture(){
    mTypeTexture = mTypesetter->getTexture();
}



void QuoteLayoutApp::mouseDown( MouseEvent event )
{
}

void QuoteLayoutApp::update(){
#ifdef USE_PARAMS
    updateParams();
#endif
}

#ifdef USE_PARAMS
void QuoteLayoutApp::updateParams(){
    static float paramTypeFontScalePrev       = PARAM_TYPE_FONT_SCALE;
    static bool  paramTypeStringConstrainPrev = PARAM_TYPE_STRING_CONSTRAIN;
    static int   paramTypeAlignPrev           = PARAM_TYPE_ALIGN;
    static bool  paramTypeManualBreak         = PARAM_TYPE_MANUAL_BREAK;
    static int   paramTypePaddingTPrev        = PARAM_TYPE_PADDING_T;
    static int   paramTypePaddingRPrev        = PARAM_TYPE_PADDING_R;
    static int   paramTypePaddingBPrev        = PARAM_TYPE_PADDING_B;
    static int   paramTypePaddingLPrev        = PARAM_TYPE_PADDING_L;
    
    
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
}
#endif
               



void QuoteLayoutApp::draw(){
#ifdef USE_PARAMS
    static const float windowWidth  = getWindowWidth();
    static const float windowHeight = getWindowHeight();

    static float viewportWidth;
    static float viewportHeight;
    static float viewportPosX;
    static float viewportPosY;
    
    glScissor(0, 0, windowWidth, windowHeight);
    gl::clear(Colorf(0.0125f,0.0125f,0.025f));

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
    
    glPushAttrib(GL_VIEWPORT_BIT);
    glScissor(viewportPosX, viewportPosY, viewportWidth, viewportHeight);
    // clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    //gl::drawSolidRect(Rectf(0,0,windowWidth,windowHeight));
    gl::setViewport(Area(Rectf(0,viewportPosY,viewportWidth,viewportPosY+viewportHeight)));
    gl::setMatrices(mCamera);
    
    gl::drawCoordinateFrame(4);
    glPushMatrix();
    glScalef(0.65f, 0.65f, 0.65f);
    
    glDisable(GL_DEPTH_TEST);
    mTypesetter->debugDrawArea();
    
    
    const vector<Cell*>& cells = mGrid->getCells();
    
    glColor3f(0.5f,0,0);
    for (auto* cell : cells) {
        cell->debugDrawArea();
    }
    mGrid->debugDrawIndices(mCamera);
    
    glEnable(GL_DEPTH_TEST);
    
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    gl::enableAdditiveBlending();
    glColor3f(1,1,1);
    mTypesetter->debugDrawString();
    gl::enableAdditiveBlending();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    glPopMatrix();

    glPopAttrib();
    
	
    if(PARAM_TYPE_SHOW_TEXTURE){
        float textureWidth  = windowHeight,
              textureHeight = windowHeight;
        float texturePosX   = windowWidth - textureWidth,
              texturePosY   = 0;
        
        glScissor(texturePosX, texturePosY, textureWidth, textureHeight);
        glPushAttrib(GL_VIEWPORT_BIT);
        gl::setMatricesWindow(getWindowSize());
        gl::draw(mTypeTexture,Rectf(windowWidth-textureWidth,0,windowWidth,textureHeight));
        glPopAttrib();

    }
    mParams->draw();
#else
    // clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    
    gl::drawCoordinateFrame(4);
    glPushMatrix();
    glScalef(0.65f, 0.65f, 0.65f);
    
    glDisable(GL_DEPTH_TEST);
    mTypesetter->debugDrawArea();
    
    const vector<Cell*>& cells = mGrid->getCells();
    
    glColor3f(0.5f,0,0);
    for (auto* cell : cells) {
        cell->debugDrawArea();
    }
    mGrid->debugDrawIndices(mCamera);
    
    glEnable(GL_DEPTH_TEST);
    
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    gl::enableAdditiveBlending();
    glColor3f(1,1,1);
    mTypesetter->debugDrawString();
    gl::enableAdditiveBlending();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    glPopMatrix();
#endif

}

CINDER_APP_NATIVE( QuoteLayoutApp, RendererGl )
