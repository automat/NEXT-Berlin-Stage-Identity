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
#include "Event.h"
#include "EventView.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class EventsDeckApp : public AppNative {
public:
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();
	void draw();
    
    CameraOrtho  mCamera;
    
    // dummy data / view
    vector<gl::Texture>   mDataImages;
    vector<next::Speaker> mDataSpeakers;
    next::Event           mEvent;
    
    
    next::EventView*      mEventView;
    //SpeakerStackView*   mSpeakerStackView;
};

void EventsDeckApp::prepareSettings(Settings *settings) {
    settings->setWindowSize(1024, 768 );
}

void EventsDeckApp::setup(){
    float aspectRatio = getWindowAspectRatio();
    float zoom = 2;
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/6893.png")));
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26262.png")));
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26092.png")));
    mDataImages.push_back(gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/27263.png")));
   
    
    using namespace next;
    
    for(auto& image : mDataImages){
        mDataSpeakers.push_back(Speaker::Create(image.weakClone()));
    }
    mEvent     = next::Event::Create(&mDataSpeakers);
    mEventView = new EventView(&mEvent);
    //mEventView->nextSpeaker(); // first test trigger
    
    gl::enableDepthRead();
}

void EventsDeckApp::keyDown(KeyEvent event) {
    switch(event.getCode()){
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        case KeyEvent::KEY_RIGHT:
            mEventView->nextSpeaker();
            break;
        default:
            break;
    }
}

void EventsDeckApp::update(){
    mEventView->update();
}

void EventsDeckApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    
    gl::drawCoordinateFrame(2);
    mEventView->draw();
}

CINDER_APP_NATIVE( EventsDeckApp, RendererGl )
