#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/TextureFont.h"

#include "Textbox.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TextboxApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void keyDown(KeyEvent event);
	void update();
	void draw();
    
    float mFontSize;
    
    
    utils::TextBox* mTextBox;
    
    Vec2f mReferenceFontOffset;
    gl::TextureFontRef mReferenceFont;
    
};

void TextboxApp::prepareSettings(Settings* settings){
    settings->setWindowSize(800, 600);
    settings->setFrameRate(36.0f);
}

void TextboxApp::setup(){
    mFontSize = 40.0f;
    
    mTextBox = new utils::TextBox();
    mTextBox->setFont(Font(app::loadResource(RES_TRANSCRIPT_BOLD),80.0f));
    mTextBox->setFontSize(mFontSize);
    mTextBox->setWidth(400);
    mTextBox->setString("Lorem\n ipsum dolor sit amet, consetetur\n sadipscing elitr, sed diam nonumy eirmod.");
    
    
    mReferenceFont = gl::TextureFont::create(Font(loadResource(RES_TRANSCRIPT_BOLD),mFontSize));
    mReferenceFontOffset.y = mReferenceFont->getAscent() - mReferenceFont->getDescent();
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
}

void TextboxApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::setMatricesWindow(app::getWindowSize());
    
    glColor3f(1, 0, 1);
    gl::drawLine(Vec2f::zero(), app::getWindowSize());
    
    
    
    glColor3f(1, 0, 1);
    gl::drawSolidRect(Rectf(20,20,20 + 5,20 + 20));
    
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    glPushMatrix();
    glTranslatef(20,20,0);
    glColor3f(1,1,1);
    mTextBox->debugDraw();
    glTranslatef(400,0,0);
    mReferenceFont->drawString(mTextBox->getString(), mReferenceFontOffset);
    glPopMatrix();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
}

CINDER_APP_NATIVE( TextboxApp, RendererGl )
