#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Matrix44.h"
#include "SpeakerView.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"
#include "Speaker.h"
#include "SpeakerStackView.h"
#include <vector>

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
    
    CameraOrtho  mCamera;
    
    // dummy data / view
    vector<gl::Texture> mDataImages;
    vector<Speaker*>    mDataSpeakers;
    SpeakerStackView*   mSpeakerStackView;
};

void SpeakerImageFilterApp::prepareSettings(Settings *settings) {
    settings->setWindowSize(1280, 1050 );
}

void SpeakerImageFilterApp::setup(){
    float aspectRatio = getWindowAspectRatio();
    float zoom = 2;
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());

    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SpeakerImageFilter/resources/6893.png")));
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SpeakerImageFilter/resources/26262.png")));
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SpeakerImageFilter/resources/26092.png")));
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SpeakerImageFilter/resources/27263.png")));
    
    
    
    for(auto& image : mDataImages){
        mDataSpeakers.push_back(Speaker::Create(image.weakClone()));
    }
  
    mSpeakerStackView = new SpeakerStackView(mDataSpeakers);
    
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
    mSpeakerStackView->update();
}

void SpeakerImageFilterApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);

    gl::drawCoordinateFrame(2);
    gl::enableAlphaTest();
    gl::enableAlphaBlending();
    mSpeakerStackView->draw();
    gl::disableAlphaBlending();
    gl::disableAlphaTest();
}

CINDER_APP_NATIVE( SpeakerImageFilterApp, RendererGl )
