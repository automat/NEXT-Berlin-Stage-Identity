#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/cairo/Cairo.h"

#include <vector>
#include <string>



using namespace ci;
using namespace ci::app;
using namespace std;

static const int   APP_WIDTH(800),APP_HEIGHT(600);
static const float APP_FRAME_RATE(30);


class StringDeconstructionApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void update();
	void draw();
    
    void setupCairo();
    void renderCairo();
    
    /*-----------------------------------------------------------------------------------------------------------------------*/
    
    ci::gl::Texture      mTexture;
    cairo::SurfaceImage* mSurface;
    cairo::Context*      mContext;
    cairo::Matrix        mStackMatrix;
    
    void pushMatrix();
    void popMatrix();
};

void StringDeconstructionApp::prepareSettings(Settings* settings){
    settings->setWindowSize(APP_WIDTH, APP_HEIGHT);
    settings->setFrameRate(APP_FRAME_RATE);
}

void StringDeconstructionApp::setup(){
    this->setupCairo();
    this->renderCairo();
}

/*-----------------------------------------------------------------------------------------------------------------------*/

void StringDeconstructionApp::setupCairo(){
    mSurface = new cairo::SurfaceImage(app::getWindowWidth(),app::getWindowHeight());
    mContext = new cairo::Context(*mSurface);
    mTexture = ci::gl::Texture(mSurface->getSurface());
}


void StringDeconstructionApp::update(){
}

void StringDeconstructionApp::pushMatrix(){
    mContext->getMatrix(&mStackMatrix);
}

void StringDeconstructionApp::popMatrix(){
    mContext->setMatrix(mStackMatrix);
}

/*-----------------------------------------------------------------------------------------------------------------------*/

void StringDeconstructionApp::renderCairo(){
    cairo::Context* ctx = mContext;
    
    float width    = app::getWindowWidth();
    float height   = app::getWindowHeight();
    float width_2  = width * 0.5f;
    float height_2 = height* 0.5f;
    
    
    ctx->save();
    
    ctx->setSourceRgb(1, 1, 1);
    ctx->paint();
    
    pushMatrix();
   // ctx->translate(width_2, height_2);
    ctx->setSourceRgb(1, 0, 0);
    ctx->rectangle(0,0,100,100);
    popMatrix();
    
    
    ctx->restore();
    mTexture.update(mSurface->getSurface());
}


void StringDeconstructionApp::draw(){
	gl::clear( Color( 0, 0, 0 ) );
    glClear(GL_COLOR_BUFFER_BIT);
    gl::setMatricesWindow(app::getWindowSize());
    gl::enableAlphaBlending(false);
    
    ci::gl::color(ci::Color::white());
    ci::gl::draw(mTexture, ci::Vec2f(0,0));
}

/*-----------------------------------------------------------------------------------------------------------------------*/


CINDER_APP_NATIVE( StringDeconstructionApp, RendererGl )
