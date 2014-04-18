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
#include "Session.h"
#include "SessionView.h"
#include "cinder/Rand.h"


#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace boost::assign;

class EventsDeckApp : public AppNative {
public:
    ~EventsDeckApp();
    
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();
	void draw();
    
    CameraOrtho  mCamera;
    
    // dummy data
    vector<gl::Texture>   mDataImages;
    vector<next::Speaker> mDataSpeakers;
    vector<next::Event>   mDataEvents;
    next::Session         mDataSession;
    
    next::SessionView*    mViewSession;
    
    next::Event           mEvent;
    next::EventView*      mEventView;
};

void EventsDeckApp::prepareSettings(Settings *settings) {
    //settings->setWindowSize(3552, 1105 );
    settings->setWindowSize(800, 600);
    settings->setWindowPos(50, 50);
}

void EventsDeckApp::setup(){
    Rand::randSeed(clock() & 65535);
    
    float aspectRatio = getWindowAspectRatio();
    float zoom = 3.65f;
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/6893.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26262.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26092.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/27263.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/6893.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26262.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26092.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/27263.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26262.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/26092.png"));
    mDataImages += gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/EventsDeck/resources/27263.png"));
    
    using namespace next;
    for(auto& image : mDataImages){
        mDataSpeakers += Speaker::Create(image.weakClone());
    }
    
    vector<Speaker*> tempSpeakers;
    
    int i,j,l;
    i = -1;
    while(++i < 10){ // create 6 dummy events
        l = Rand::randInt(1, 6);
        j = -1;
        tempSpeakers.clear();
        while (++j < l) {
            tempSpeakers += &mDataSpeakers[Rand::randInt(0, mDataSpeakers.size())];
        }
        mDataEvents += next::Event::Create(tempSpeakers);
    }


   
    mDataSession = Session::Create(0, "Session", 0, 0, &mDataEvents);
    mViewSession = new SessionView(&mDataSession);
    
    gl::enableDepthRead();
}

EventsDeckApp::~EventsDeckApp(){
    delete mViewSession;
}


void EventsDeckApp::keyDown(KeyEvent event) {
    switch(event.getCode()){
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        case KeyEvent::KEY_RIGHT:
            mViewSession->next();
            break;
        default:
            break;
    }
}

void EventsDeckApp::update(){
    mViewSession->update();
}

void EventsDeckApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatrices(mCamera);
    
    gl::drawCoordinateFrame(2);
    
    mViewSession->debugDraw();
   
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    mViewSession->draw();
    
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5); // reset what seems to be cinders default
}

CINDER_APP_NATIVE( EventsDeckApp, RendererGl )
