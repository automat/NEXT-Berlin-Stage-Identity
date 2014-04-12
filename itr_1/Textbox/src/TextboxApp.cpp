#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/params/Params.h"

#include "Textbox.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float TEXT_BOX_FONT_SIZE(40.0f);
float TEXT_BOX_TEXTURE_FONT_SIZE_SCALE(2.0f);
float TEXT_BOX_FONT_LINE_HEIGHT(1.0f);
float TEXT_BOX_WIDTH(400.0f);

class TextboxApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void keyDown(KeyEvent event);
	void update();
	void draw();
    
    string mString;
    utils::TextBox* mTextBox;
    params::InterfaceGlRef mParams;
    
    
   
};

void TextboxApp::prepareSettings(Settings* settings){
    settings->setWindowSize(800, 600);
    settings->setFrameRate(36.0f);
}

void TextboxApp::setup(){
    mString = "Joining the physical with the digital:\n the future of creating, selling and owning things";
    
    mTextBox = new utils::TextBox();
    
    mTextBox->setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),TEXT_BOX_FONT_SIZE * TEXT_BOX_TEXTURE_FONT_SIZE_SCALE));
    mTextBox->setFontSize(TEXT_BOX_FONT_SIZE);
    mTextBox->setLineHeight(TEXT_BOX_FONT_LINE_HEIGHT);
    mTextBox->setWidth(TEXT_BOX_WIDTH);
    mTextBox->setString(mString);

    mParams = params::InterfaceGl::create("Controls", Vec2i(200,100));
    mParams->addParam("Texture Font Scale", &TEXT_BOX_TEXTURE_FONT_SIZE_SCALE, "min=1.0 max=16.0 step=1.0");
    mParams->addParam("Font Size", &TEXT_BOX_FONT_SIZE, "min=0.0 max=80.0 step=0.125");
    mParams->addParam("Font Line Height", &TEXT_BOX_FONT_LINE_HEIGHT, "min=0.0 max=2.0 step=0.0125");
    mParams->addParam("Width", &TEXT_BOX_WIDTH);
}

void TextboxApp::keyDown( KeyEvent event ){
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            quit();
            break;
        default:
            break;
    }
}

void TextboxApp::update(){
    static float textBoxFontSizePrev = TEXT_BOX_FONT_SIZE;
    static float textBoxTextureFontSizeScalePrev = TEXT_BOX_TEXTURE_FONT_SIZE_SCALE;
    static float textBoxFontLineHeightPrev = TEXT_BOX_FONT_LINE_HEIGHT;
    static float textBoxWidthPrev = TEXT_BOX_WIDTH;
    
    if(TEXT_BOX_TEXTURE_FONT_SIZE_SCALE != textBoxTextureFontSizeScalePrev){
        mTextBox->setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),TEXT_BOX_FONT_SIZE * TEXT_BOX_TEXTURE_FONT_SIZE_SCALE));
        mTextBox->setFontSize(TEXT_BOX_FONT_SIZE);
        mTextBox->setString(mString);
        
        textBoxTextureFontSizeScalePrev = TEXT_BOX_TEXTURE_FONT_SIZE_SCALE;
    }
    
    if(TEXT_BOX_FONT_SIZE != textBoxFontSizePrev ||
       TEXT_BOX_FONT_LINE_HEIGHT != textBoxFontLineHeightPrev ||
       TEXT_BOX_WIDTH != textBoxWidthPrev){
        mTextBox->setFontSize(TEXT_BOX_FONT_SIZE);
        mTextBox->setLineHeight(TEXT_BOX_FONT_LINE_HEIGHT);
        mTextBox->setWidth(TEXT_BOX_WIDTH);
        mTextBox->setString(mString);
        
        textBoxFontSizePrev = TEXT_BOX_FONT_SIZE;
        textBoxFontLineHeightPrev = TEXT_BOX_FONT_LINE_HEIGHT;
        textBoxWidthPrev = TEXT_BOX_WIDTH;
        
    }
}

void TextboxApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.15f, 0.15f, 0.15f ) );
    gl::setMatricesWindow(app::getWindowSize());
  
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    glPushMatrix();
    glTranslatef(200,20,0);
    glColor3f(1,1,1);
    mTextBox->debugDraw();
    glPopMatrix();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
    
    mParams->draw();
}

CINDER_APP_NATIVE( TextboxApp, RendererGl )
