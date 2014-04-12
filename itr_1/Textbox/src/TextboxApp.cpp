#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/params/Params.h"

#include "Textbox.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float TEXT_BOX_FONT_SIZE(60.0f);
float TEXT_BOX_TEXTURE_FONT_SIZE_SCALE(2.0f);
float TEXT_BOX_FONT_LINE_HEIGHT(1.2f);
float TEXT_BOX_WIDTH(400.0f);

ColorAf TEXT_BOX_COLOR_FONT(1,1,1,1);
ColorAf TEXT_BOX_COLOR_UNDERLINE(1,0,1,1);
ColorAf TEXT_BOX_COLOR_DROP_SHADOW(0,0,0,1);

bool    TEXT_BOX_USE_DROP_SHADOW(true);
bool    TEXT_BOX_USE_UNDERLINE(false);
Vec2f   TEXT_BOX_DROP_SHADOW_OFFSET(1,1);
float   TEXT_BOX_DROP_SHADOW_SCALE(1.0f);

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
    mTextBox->setColorFont(TEXT_BOX_COLOR_FONT);
    mTextBox->setColorUnderline(TEXT_BOX_COLOR_UNDERLINE);
    mTextBox->setColorDropShadow(TEXT_BOX_COLOR_DROP_SHADOW);
    mTextBox->setDropShadowOffset(TEXT_BOX_DROP_SHADOW_OFFSET);
    mTextBox->dropShadow(TEXT_BOX_USE_DROP_SHADOW);
    mTextBox->setDropShadowScale(TEXT_BOX_DROP_SHADOW_SCALE);
    mTextBox->underline(TEXT_BOX_USE_UNDERLINE);
    mTextBox->setWidth(TEXT_BOX_WIDTH);
    mTextBox->setString(mString);

    mParams = params::InterfaceGl::create("Controls", Vec2i(200,400));
    mParams->addSeparator();
    mParams->addParam("Texture Font Scale", &TEXT_BOX_TEXTURE_FONT_SIZE_SCALE, "min=1.0 max=16.0 step=1.0");
    mParams->addParam("Font Size", &TEXT_BOX_FONT_SIZE, "min=0.0 max=200.0 step=0.125");
    mParams->addParam("Font Line Height", &TEXT_BOX_FONT_LINE_HEIGHT, "min=0.0 max=2.0 step=0.0125");
    mParams->addParam("Width", &TEXT_BOX_WIDTH);
    mParams->addSeparator();
    mParams->addParam("Color Font", &TEXT_BOX_COLOR_FONT);
    mParams->addParam("Color Underline", &TEXT_BOX_COLOR_UNDERLINE);
    mParams->addParam("Color DropShadow", &TEXT_BOX_COLOR_DROP_SHADOW);
    mParams->addSeparator();
    mParams->addParam("Use DropShadow", &TEXT_BOX_USE_DROP_SHADOW);
    mParams->addParam("DropShadow Offset X", &TEXT_BOX_DROP_SHADOW_OFFSET[0]);
    mParams->addParam("DropShadow Offset Y", &TEXT_BOX_DROP_SHADOW_OFFSET[1]);
    mParams->addParam("DropShadow Scale", &TEXT_BOX_DROP_SHADOW_SCALE,"min=1.0 max=4.0 step=0.0125");
    
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
    
    static ColorAf textBoxColorFontPrev = TEXT_BOX_COLOR_FONT;
    static ColorAf textBoxColorUnderlinePrev = TEXT_BOX_COLOR_UNDERLINE;
    static ColorAf textBoxColorDropShadowPrev = TEXT_BOX_COLOR_DROP_SHADOW;
    
    static bool textBoxUseDropShadowPrev = TEXT_BOX_USE_DROP_SHADOW;
    static bool textBoxUseUnderlinePrev = TEXT_BOX_USE_UNDERLINE;
    
    static Vec2f textBoxDropShadowOffsetPrev = TEXT_BOX_DROP_SHADOW_OFFSET;
    static bool textBoxDropShadowScale = TEXT_BOX_DROP_SHADOW_SCALE;
    
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
    
    if(TEXT_BOX_DROP_SHADOW_OFFSET != textBoxDropShadowOffsetPrev){
        mTextBox->setDropShadowOffset(TEXT_BOX_DROP_SHADOW_OFFSET);
        mTextBox->setString(mString);
        textBoxDropShadowOffsetPrev = TEXT_BOX_DROP_SHADOW_OFFSET;
    }
    
    if(TEXT_BOX_DROP_SHADOW_SCALE != textBoxDropShadowScale){
        mTextBox->setDropShadowScale(TEXT_BOX_DROP_SHADOW_SCALE);
        mTextBox->setString(mString);
        textBoxDropShadowScale = TEXT_BOX_DROP_SHADOW_SCALE;
    }
    
    if(TEXT_BOX_USE_DROP_SHADOW != textBoxUseDropShadowPrev){
        mTextBox->dropShadow(TEXT_BOX_USE_DROP_SHADOW);
        mTextBox->setString(mString);
        textBoxUseDropShadowPrev = TEXT_BOX_USE_DROP_SHADOW;
    }
    
    if(TEXT_BOX_USE_UNDERLINE != textBoxUseUnderlinePrev){
        mTextBox->underline(TEXT_BOX_USE_UNDERLINE);
        mTextBox->setString(mString);
        textBoxUseUnderlinePrev = TEXT_BOX_USE_UNDERLINE;
    }
    
    if(TEXT_BOX_COLOR_FONT != textBoxColorFontPrev){
        mTextBox->setColorFont(TEXT_BOX_COLOR_FONT);
        mTextBox->setString(mString);
        textBoxColorFontPrev = TEXT_BOX_COLOR_FONT;
    }
    
    if(TEXT_BOX_COLOR_UNDERLINE != textBoxColorUnderlinePrev){
        mTextBox->setColorUnderline(TEXT_BOX_COLOR_UNDERLINE);
        mTextBox->setString(mString);
        textBoxColorUnderlinePrev = TEXT_BOX_COLOR_UNDERLINE;
    }
    
    if(TEXT_BOX_COLOR_DROP_SHADOW != textBoxColorDropShadowPrev){
        mTextBox->setColorDropShadow(TEXT_BOX_COLOR_DROP_SHADOW);
        mTextBox->setString(mString);
        textBoxColorDropShadowPrev = TEXT_BOX_COLOR_DROP_SHADOW;
    }
}

void TextboxApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.15f, 0.15f, 0.45f ) );
    gl::setMatricesWindow(app::getWindowSize());
  
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    glPushMatrix();
    glTranslatef(250,50,0);
    glColor3f(1,1,1);
    gl::draw(mTextBox->getTexture());
    mTextBox->debugDraw();
    glPopMatrix();
    
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5); // reset what seems to be cinders default
    
    mParams->draw();
}

CINDER_APP_NATIVE( TextboxApp, RendererGl )
