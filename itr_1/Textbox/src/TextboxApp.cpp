#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/params/Params.h"

#include "Textbox.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const int WINDOW_WIDTH(1280), WINDOW_HEIGHT(1024);

const float PADDING(100.0f);

float TEXT_BOX_FONT_SIZE(100.0f);
float TEXT_BOX_TEXTURE_FONT_SIZE_SCALE(4.0f);
float TEXT_BOX_FONT_LINE_HEIGHT(1.1);
float TEXT_BOX_WIDTH(WINDOW_WIDTH - PADDING * 2);

ColorAf TEXT_BOX_COLOR_FONT(1,1,1,1);
ColorAf TEXT_BOX_COLOR_UNDERLINE(223.0f/255.0f,17.0f/255.0f,101.0f/255.0f,1);
ColorAf TEXT_BOX_COLOR_DROP_SHADOW(0,0,0,0.75f);

bool  TEXT_BOX_USE_DROP_SHADOW(true);
bool  TEXT_BOX_USE_UNDERLINE(true);
Vec2f TEXT_BOX_DROP_SHADOW_OFFSET(0,0);
float TEXT_BOX_DROP_SHADOW_SCALE(1.0f);

float TEXT_BOX_UNDERLINE_HEIGHT(32.0f);
float TEXT_BOX_UNDERLINE_BASELINE_OFFSET(0.0f);

bool TEXT_BOX_DEBUG_DRAW(true);

class TextboxApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void keyDown(KeyEvent event);
	void update();
	void draw();
    
    string mString;
    next::TextBox* mTextBox;
    params::InterfaceGlRef mParams;
};

void TextboxApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(36.0f);
}

void TextboxApp::setup(){
    mString = "At­ NEXT14, great speakers will draw a picture of what ’The New Normal’ will look like in the near future. Join NEXT14 on May 5-6, 2014!";
    
    mTextBox = new next::TextBox();
    
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
    mTextBox->setUnderlineHeight(TEXT_BOX_UNDERLINE_HEIGHT);
    mTextBox->setUnderlineBaselineOffset(TEXT_BOX_UNDERLINE_BASELINE_OFFSET);
    
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
    mParams->addParam("DropShadow Scale", &TEXT_BOX_DROP_SHADOW_SCALE,"min=0.0 max=4.0 step=0.0125");
    mParams->addSeparator();
    mParams->addParam("Underline Height", &TEXT_BOX_UNDERLINE_HEIGHT, "min=0.0 max=30.0 step=0.0125");
    mParams->addParam("Underline Offset", &TEXT_BOX_UNDERLINE_BASELINE_OFFSET, "min=-20.0 max=20.0 step=0.0125");
    mParams->addParam("Debug Draw", &TEXT_BOX_DEBUG_DRAW);
    
    
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
    
    static bool textBoxUnderlineHeightPrev = TEXT_BOX_UNDERLINE_HEIGHT;
    static bool textBoxUnderlineOffsetPrev = TEXT_BOX_UNDERLINE_BASELINE_OFFSET;
    
    
    
    if (TEXT_BOX_UNDERLINE_HEIGHT != textBoxUnderlineHeightPrev) {
        mTextBox->setUnderlineHeight(TEXT_BOX_UNDERLINE_HEIGHT);
        mTextBox->setString(mString);
        textBoxUnderlineHeightPrev = TEXT_BOX_UNDERLINE_HEIGHT;
    }
    
    if (TEXT_BOX_UNDERLINE_BASELINE_OFFSET != textBoxUnderlineOffsetPrev) {
        mTextBox->setUnderlineBaselineOffset(TEXT_BOX_UNDERLINE_BASELINE_OFFSET);
        mTextBox->setString(mString);
        textBoxUnderlineHeightPrev = TEXT_BOX_UNDERLINE_BASELINE_OFFSET;
    }
    
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
	gl::clear( Color( 0.15f, 0.15f, 0.15f ) );
    gl::setMatricesWindow(app::getWindowSize());
  
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    glPushMatrix();
    glTranslatef(PADDING + mTextBox->getTopLeft().x,PADDING + mTextBox->getTopLeft().y,0);
    glColor3f(1,1,1);
    gl::draw(mTextBox->getTexture());
    
    if(TEXT_BOX_DEBUG_DRAW){
        mTextBox->debugDraw();
    }
    glPopMatrix();
    
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5); // reset what seems to be cinders default
    
    mParams->draw();
}

CINDER_APP_NATIVE( TextboxApp, RendererGl )
