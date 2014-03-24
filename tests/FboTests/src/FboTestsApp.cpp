#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"


using namespace ci;
using namespace ci::app;
using namespace std;



class FboTestsApp : public AppNative {
  public:
    void prepareSettings(Settings* settings);
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void drawScene();
    
    CameraOrtho mCamera;
    gl::Fbo     mFbo;
    
    
};

void FboTestsApp::prepareSettings(Settings* settings){
    settings->setWindowSize(800, 600);
}

void FboTestsApp::setup(){
    gl::Fbo::Format format;
    format.setSamples(4);
    mFbo = gl::Fbo(1024,1024,format);
    
    
    float zoom = 5;
    float aspectRatio = mFbo.getAspectRatio();
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f::yAxis(), Vec3f::zero());
}

void FboTestsApp::drawScene(){
    gl::enableDepthRead();

    float time  = getElapsedSeconds() * 180.0f / float(M_PI);
    float scale = 1.0f + (0.5f + sinf(time * 0.0125f) * 0.5f);

    float k = (0.5f + sinf(time * 0.125f) * 0.5f);
    
    gl::clear(Color(k,k*0.5f,k*0.25f));
    gl::setMatrices(mCamera);
    glPushMatrix();
    glRotatef(time, 1, 0, 1);
    gl::drawColorCube(Vec3f::zero(), Vec3f::one() * scale);
    glPopMatrix();
    gl::disableDepthRead();
}



void FboTestsApp::mouseDown( MouseEvent event )
{
}

void FboTestsApp::update()
{
}

void FboTestsApp::draw(){
	// clear out the window with black
    
    
	gl::clear( Color( 0, 0, 0 ) );
    mFbo.bindFramebuffer();
    gl::setViewport(mFbo.getBounds());
    drawScene();
    mFbo.unbindFramebuffer();
    
    float windowWidth  = getWindowWidth();
    float windowHeight = getWindowHeight();
    float fboWidth     = mFbo.getWidth();
    float ratio        = fboWidth / windowWidth;
    float width        = fboWidth * ratio;
    float height       = windowHeight;
    Rectf rect(0,0,width,height);
    
    
    gl::setMatricesWindow(getWindowSize(),true);
    gl::draw(mFbo.getTexture(),rect);
}

CINDER_APP_NATIVE( FboTestsApp, RendererGl )
