#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Matrix44.h"
#include "SpeakerView.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"
#include "Speaker.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class SpeakerImageFilterApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();
	void draw();

    gl::Texture mTestTexture;

    CameraOrtho  mCamera;
    Speaker*     mSpeaker;
    SpeakerView* mSpeakerView;
};

void SpeakerImageFilterApp::prepareSettings(Settings *settings) {
    settings->setWindowSize(1280, 1050 );
}

void SpeakerImageFilterApp::setup(){
    float aspectRatio = getWindowAspectRatio();
    float zoom = 2;
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());

    mTestTexture = gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SpeakerImageFilter/resources/26262.png"));
    mSpeaker = Speaker::Create(mTestTexture.weakClone());
    mSpeakerView = new SpeakerView(mSpeaker);
    
    gl::enableDepthRead();
}

void SpeakerImageFilterApp::keyDown(KeyEvent event) {
    switch(event.getCode()){
        case KeyEvent::KEY_ESCAPE:
            this->quit();
        default:
            break;
    }
}

void SpeakerImageFilterApp::update(){
    mSpeakerView->update();
}

void SpeakerImageFilterApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);

    gl::drawCoordinateFrame(2);
    float t = static_cast<float>(app::getElapsedSeconds());

    glPushMatrix();
    glTranslatef(0,sin(t * 0.125f) * 0.5f,0);
    mSpeakerView->draw();
    glPopMatrix();
}

CINDER_APP_NATIVE( SpeakerImageFilterApp, RendererGl )
