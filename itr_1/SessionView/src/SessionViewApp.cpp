#include "cinder/app/AppNative.h"

#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/assign.hpp>

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Matrix44.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"
#include "cinder/gl/Texture.h"

#include "stage/session/model/Speaker.h"
#include "stage/session/model/Event.h"
#include "stage/session/model/Session.h"
#include "stage/session/model/Mapping.h"

#include "stage/session/view/SessionView.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace boost::assign;

class SessionViewApp : public AppNative {
public:
    ~SessionViewApp();
    
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();
	void draw();
    
    CameraOrtho  mCamera;
    
    map<uint32_t, gl::Texture>*   mDataImages;
    map<uint32_t, next::Speaker>* mDataSpeakers;
    map<uint32_t, next::Event>*   mDataEvents;
    next::Session*                mDataSession;
    
    next::SessionView*    mViewSession;
    
    bool        mDrawMeasurements;
    gl::Texture mRefMeasurements;
};

void SessionViewApp::prepareSettings(Settings *settings) {
    settings->setWindowSize(3552, 1105 );
    settings->setWindowPos(0, 0);
}

void SessionViewApp::setup(){
    Rand::randSeed(clock() & 65535);
    
    float aspectRatio = getWindowAspectRatio();
    float zoom = 0.65f;
    mCamera.setOrtho(-aspectRatio * zoom, aspectRatio * zoom, -zoom, zoom, -1, 10);
    mCamera.lookAt(Vec3f(1,1,1), Vec3f::zero());
    
    string jsonFilepath  = "/Users/automat/Projects/next/itr_1/SessionView/resources/data.json";
    string imageFilepath = "/Users/automat/Projects/next/data/images_dome_stage";
    
    mDataImages   = nullptr;
    mDataSpeakers = nullptr;
    mDataEvents   = nullptr;
    mDataSession  = nullptr;

    next::Mapping::Get(jsonFilepath, imageFilepath,     3615,
                       mDataImages, mDataSpeakers, mDataEvents, mDataSession);
    mViewSession = new next::SessionView(mDataSession);
    
    mDrawMeasurements = false;
    mRefMeasurements = gl::Texture(loadImage("/Users/automat/Projects/next/itr_1/SessionView/resources/measurements.png"));
    
    gl::enableDepthRead();
}

SessionViewApp::~SessionViewApp(){
    delete mViewSession;
    delete mDataSession;
    delete mDataEvents;
    delete mDataSpeakers;
    delete mDataImages;
}

void SessionViewApp::keyDown(KeyEvent event) {
    switch(event.getCode()){
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
        case KeyEvent::KEY_RIGHT:
            //mViewSession->next();
            break;
        case KeyEvent::KEY_LEFT:
            mViewSession->start();
            //mViewSession->prev();
            break;
        case KeyEvent::KEY_SPACE:
            mDrawMeasurements = !mDrawMeasurements;
            break;
        default:
            break;
    }
}

void SessionViewApp::update(){
    mViewSession->update();
}

void SessionViewApp::draw(){
    gl::clear( Color( 0, 0, 0 ) );
    gl::setMatricesWindow(app::getWindowSize());
    
    if(mDrawMeasurements){
        glColor3f(1,1,1);
        gl::draw(mRefMeasurements);
    }
    
    gl::setMatrices(mCamera);
    
#ifdef SESSION_DEBUG_DRAW
    gl::drawCoordinateFrame(2);
    mViewSession->debugDraw();
#endif
    
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_ALPHA_TEST);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    mViewSession->draw();
    
    gl::enableAlphaBlending();
    
    gl::disableDepthRead();
    gl::pushMatrices();
    gl::setMatricesWindow(app::getWindowSize());
    mViewSession->drawLabels();
    gl::popMatrices();
    gl::enableDepthRead();
    
    gl::disableAlphaBlending();
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5); // clearStates what seems to be cinders default
    
    
    
    
    
    
}

CINDER_APP_NATIVE( SessionViewApp, RendererGl )
