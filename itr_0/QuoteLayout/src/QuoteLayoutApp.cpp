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
static float  TYPE_FONT_SCALE_CURR(TYPE_FONT_SCALE);
static float  TYPE_FONT_SCALE_LAST(TYPE_FONT_SCALE);
static int    TYPE_CELL_PADDING_T_CURR(TYPE_PADDING_T);
static int    TYPE_CELL_PADDING_T_LAST(TYPE_PADDING_T);
static int    TYPE_CELL_PADDING_R_CURR(TYPE_PADDING_R);
static int    TYPE_CELL_PADDING_R_LAST(TYPE_PADDING_R);
static int    TYPE_CELL_PADDING_B_CURR(TYPE_PADDING_B);
static int    TYPE_CELL_PADDING_B_LAST(TYPE_PADDING_B);
static int    TYPE_CELL_PADDING_L_CURR(TYPE_PADDING_L);
static int    TYPE_CELL_PADDING_L_LAST(TYPE_PADDING_L);
static string TYPE_STRING_LAST("");
static bool   TYPE_STRING_CONSTRAIN_CURR(TYPE_CONSTRAIN);
static bool   TYPE_STRING_CONSTRAIN_LAST(TYPE_CONSTRAIN);
static bool   TYPE_MANUAL_BREAK_CURR(TYPE_MANUAL_BREAK);
static bool   TYPE_MANUAL_BREAK_LAST(TYPE_MANUAL_BREAK);
static int    TYPE_ALIGN_CURR(QuoteTypesetter::Align::CENTER);
static int    TYPE_ALIGN_LAST(TYPE_ALIGN_CURR);
static bool   TYPE_SHOW_TEXTURE(true);
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
    mTypesetter->setAlign(static_cast<QuoteTypesetter::Align>(TYPE_ALIGN_CURR));
    mTypesetter->setPadding(0, 0, 0, 1);
    mTypesetter->constrain(false);
    mTypesetter->manualLineBreak(TYPE_MANUAL_BREAK_CURR);
    mTypesetter->debugTexture();
    
    //updateLayout("\n\n\nSmall\n\n\n\nstring\n\n \n\n,and\n\n");
    updateLayout("Small\nstring");
    
#ifdef USE_PARAMS
    vector<string> alignEnumNames = {"left","center","right"};
    
    mParams = params::InterfaceGl::create("controls", Vec2f(200,200));
    mParams->addParam("Font scale", &TYPE_FONT_SCALE_CURR, "min=0 max=1 step=0.001");
    mParams->addParam("Padding T",  &TYPE_CELL_PADDING_T_CURR, "min=0 max=5 step=1");
    mParams->addParam("Padding R",  &TYPE_CELL_PADDING_R_CURR, "min=0 max=5 step=1");
    mParams->addParam("Padding B",  &TYPE_CELL_PADDING_B_CURR, "min=0 max=5 step=1");
    mParams->addParam("Padding L",  &TYPE_CELL_PADDING_L_CURR, "min=0 max=5 step=1");
    mParams->addParam("Align", alignEnumNames, &TYPE_ALIGN_CURR);
    mParams->addParam("Constrain",  &TYPE_STRING_CONSTRAIN_CURR);
    mParams->addParam("Manual Break", &TYPE_MANUAL_BREAK_CURR);
    mParams->addParam("Show Texture", &TYPE_SHOW_TEXTURE);
    
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
    if(TYPE_FONT_SCALE_CURR != TYPE_FONT_SCALE_LAST){
        mTypesetter->setFontScale(TYPE_FONT_SCALE_CURR);
        updateLayout(TYPE_STRING_LAST);
        
        TYPE_FONT_SCALE_LAST = TYPE_FONT_SCALE_CURR;
    }
    
    if(TYPE_STRING_CONSTRAIN_CURR != TYPE_STRING_CONSTRAIN_LAST){
        mTypesetter->constrain(TYPE_STRING_CONSTRAIN_CURR);
        updateLayout(TYPE_STRING_LAST);
        TYPE_STRING_CONSTRAIN_LAST = TYPE_STRING_CONSTRAIN_CURR;
    }
    
    if(TYPE_MANUAL_BREAK_CURR != TYPE_MANUAL_BREAK_LAST){
        //mTypesetter->manualLineBreak(TYPE_MANUAL_BREAK_CURR);
        updateLayout(TYPE_STRING_LAST);
        TYPE_MANUAL_BREAK_LAST = TYPE_MANUAL_BREAK_CURR;
    }
    
    if(TYPE_ALIGN_CURR != TYPE_ALIGN_LAST){
        mTypesetter->setAlign(static_cast<QuoteTypesetter::Align>(TYPE_ALIGN_CURR));
        updateLayout(TYPE_STRING_LAST);
        TYPE_ALIGN_LAST = TYPE_ALIGN_CURR;
    }
    
    if(TYPE_CELL_PADDING_T_CURR != TYPE_CELL_PADDING_T_LAST ||
       TYPE_CELL_PADDING_R_CURR != TYPE_CELL_PADDING_R_LAST ||
       TYPE_CELL_PADDING_B_CURR != TYPE_CELL_PADDING_B_LAST ||
       TYPE_CELL_PADDING_L_CURR != TYPE_CELL_PADDING_L_LAST){
        
        mTypesetter->setPadding(TYPE_CELL_PADDING_T_CURR,
                                TYPE_CELL_PADDING_R_CURR,
                                TYPE_CELL_PADDING_B_CURR,
                                TYPE_CELL_PADDING_L_CURR);
        updateLayout(TYPE_STRING_LAST);
        TYPE_CELL_PADDING_T_LAST = TYPE_CELL_PADDING_T_CURR;
        TYPE_CELL_PADDING_R_LAST = TYPE_CELL_PADDING_R_CURR;
        TYPE_CELL_PADDING_B_LAST = TYPE_CELL_PADDING_B_CURR;
        TYPE_CELL_PADDING_L_LAST = TYPE_CELL_PADDING_L_CURR;
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

    static bool typeShowTexturePrev = !TYPE_SHOW_TEXTURE;
    if(TYPE_SHOW_TEXTURE != typeShowTexturePrev){
        if(TYPE_SHOW_TEXTURE){
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
        typeShowTexturePrev = TYPE_SHOW_TEXTURE;
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
    
	
    if(TYPE_SHOW_TEXTURE){
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
