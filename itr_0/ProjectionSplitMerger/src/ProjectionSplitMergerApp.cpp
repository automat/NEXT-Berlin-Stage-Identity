#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "config.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//
//  WARNING: On Mavericks unique spaces for seperate monitors have to be deactivated!
//
//  Test for 2 multiprojector edge blending setup.
//  A scene is rendered into an fbo spanning 2 views, which is displayed in 2 seperate
//  viewports. The projector overlapping region to the right of the 1th viewport
//  is duplicated on the left of the 2th viewport.
//
//  The app is configurable via a config.json00
//


class ProjectionSplitMergerApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();

    void drawScene();
	void draw();
    
    CameraOrtho mCamera;
    CameraPersp mCameraPersp;
    gl::Fbo     mFbo;
};

void ProjectionSplitMergerApp::prepareSettings(Settings* settings){
    // Load config
    config::Load(getResourcePath().string() + "/config.json");
    // Set presets
    const std::vector<DisplayRef>& displays = Display::getDisplays();
    if (displays.size() == 1) {
        cout << "Warning: Single monitor setup detected." << endl;
    }
    
    settings->setBorderless(WINDOW_BORDERLESS);
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setResizable( WINDOW_REZISABLE);
    if (WINDOW_FIXED_POSITION) {
        settings->setWindowPos(WINDOW_POSITION_X, WINDOW_POSITION_Y);
    }
    settings->setAlwaysOnTop(true);
    
    // Set settings
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void ProjectionSplitMergerApp::setup(){
    float aspectRatio = float(APP_VIEWPORT_WIDTH) / float(APP_VIEWPORT_HEIGHT);//app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10.0f, 1);
    mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());

    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(8);

    mFbo = gl::Fbo(APP_VIEWPORT_WIDTH,APP_VIEWPORT_HEIGHT,fboFormat);
    
    glEnable(GL_SCISSOR_TEST);
}

void ProjectionSplitMergerApp::drawScene(){
    mFbo.bindFramebuffer();
    glPushAttrib(GL_VIEWPORT_BIT),
    glEnable(GL_DEPTH_TEST);
    gl::setViewport(mFbo.getBounds());
    gl::clear( Color(0.15f, 0.15f, 0.25f) );
    gl::setMatrices(mCamera);
    
    
    static const Vec3f zero;
    static const Vec3f unit(1,1,1);
    
    glPushMatrix();
    glScalef(3,3,3);
    glRotatef(app::getElapsedSeconds()*20, 0, 1, 0);
    gl::drawColorCube(zero, unit);
    glPopMatrix();
    
    
    glDisable(GL_DEPTH_TEST);
    glPopAttrib();
    mFbo.unbindFramebuffer();
}

void ProjectionSplitMergerApp::keyDown(KeyEvent event){
    switch (event.getCode()) {
        case KeyEvent::KEY_ESCAPE:
            this->quit();
            break;
    }
}

void ProjectionSplitMergerApp::update(){
    
}


void ProjectionSplitMergerApp::draw(){
    drawScene();
    
    gl::clear(Color(0,0,0));
    gl::setMatricesWindow(app::getWindowSize(),false);
    
    const static Area  fboArea0(Vec2f(0,0),Vec2f(APP_VIEWPORT_WIDTH / 2, APP_VIEWPORT_HEIGHT));
    const static Area  fboArea1(Vec2f(APP_VIEWPORT_WIDTH / 2 - PROJECTION_EDGE_WIDTH,0),Vec2f(APP_VIEWPORT_WIDTH / 2 - PROJECTION_EDGE_WIDTH + APP_VIEWPORT_WIDTH / 2, APP_VIEWPORT_HEIGHT));
    
    const static Rectf display0(0,0,DISPLAY_WIDTH, WINDOW_HEIGHT);
    const static Rectf display1(DISPLAY_WIDTH,0,DISPLAY_WIDTH + DISPLAY_WIDTH, WINDOW_HEIGHT);
    
    //draw first screen
    glColor3f(1,1,1);
    gl::draw(mFbo.getTexture(),fboArea0,display0);
    gl::draw(mFbo.getTexture(),fboArea1,display1);
    
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(20, 20, 0);
    glScalef(-1, 0, 0);
    gl::drawString("DISPLAY 0",Vec2f::zero());
    glPopMatrix();
    glColor3f(0, 1, 0);
    gl::drawStrokedRect(display0); // draw area display0
    gl::drawStrokedRect(display1); // draw area display1
    glEnable(GL_ALPHA_TEST);
    gl::enableAlphaBlending();
    glColor3f(1,0,0);
    gl::drawStrokedRect(Rectf(DISPLAY_WIDTH,0,
                              DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH, APP_VIEWPORT_HEIGHT));
    glColor4f(0, 0, 0, 0.75f);
    gl::drawSolidRect(Rectf(DISPLAY_WIDTH, 0,
                            DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH, APP_VIEWPORT_HEIGHT));
    
    gl::disableAlphaBlending();
    glDisable(GL_ALPHA_TEST);
    //draw second screen
    
    
}

CINDER_APP_NATIVE( ProjectionSplitMergerApp, RendererGl )
