#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "config.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Utilities.h"

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
    void drawStringWithBg(gl::TextureFontRef font, const string& str, const Vec2f& center);
    
    gl::TextureFontRef mDebugTexFont60;
    
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
    settings->setAlwaysOnTop(WINDOW_ALWAYS_ON_TOP);
    
    // Set settings
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void ProjectionSplitMergerApp::setup(){
    mDebugTexFont60 = gl::TextureFont::create(Font("Apercu Mono", 40));
    
    float aspectRatio = float(APP_VIEWPORT_WIDTH) / float(APP_VIEWPORT_HEIGHT);//app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10.0f, 10.0f);
    mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());

    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(8);

    mFbo = gl::Fbo(APP_VIEWPORT_WIDTH,APP_VIEWPORT_HEIGHT,fboFormat);
    
    glEnable(GL_SCISSOR_TEST);
}

void ProjectionSplitMergerApp::drawScene(){
    // Sample scene
    mFbo.bindFramebuffer();
    glPushAttrib(GL_VIEWPORT_BIT),
    glEnable(GL_DEPTH_TEST);
    gl::setViewport(mFbo.getBounds());
    gl::clear( Color(0.15f, 0.15f, 0.25f) );
    gl::setMatrices(mCamera);
    
    
    static const Vec3f zero;
    static const Vec3f unit(1,1,1);
    int num    = 10;
    float step = float(M_PI * 2) / float(num) ;
    glPushMatrix();
    glRotatef(app::getElapsedSeconds() * 100, 0, 1, 0);
    for(int i = 0; i < num; ++i){
        glPushMatrix();
        glTranslatef(cosf(step *  float(i) ), 0, sin(step * float(i) ));
        gl::drawColorCube(zero, unit * 0.125f);
        glPopMatrix();
        
    }
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

void ProjectionSplitMergerApp::drawStringWithBg(gl::TextureFontRef font, const string& str, const Vec2f& center){
    float fontDescent    = font->getDescent();
    Vec2f fontStringSize = font->measureString(str);
    Rectf stringRect     = Rectf(center.x - fontStringSize.x * 0.5f,
                                 center.y - fontStringSize.y * 0.5f,
                                 center.x + fontStringSize.x * 0.5f,
                                 center.y + fontStringSize.y * 0.5f);
    
    
    glPushMatrix();
    glColor3f(0, 0, 0);
    gl::drawSolidRect(stringRect);
    glTranslatef(stringRect.x1, stringRect.y1, 0);
    glScalef( 1, -1, 1);
    glColor3f(1, 1, 1);
    font->drawString(str, Vec2f(0,-fontDescent));
    glPopMatrix();
}

void ProjectionSplitMergerApp::draw(){


    drawScene();
    
    gl::clear(Color(0,0,0));
    gl::setMatricesWindow(app::getWindowSize(),false);
    
    
    //draw first screen
    glColor3f(1,1,1);
    gl::draw(mFbo.getTexture(),FBO_DISPLAY0_AREA,DISPLAY0_RECT);
    glPushMatrix();
    //glTranslatef(-(0.5f + sinf(app::getElapsedSeconds()) * 0.5f) * float(PROJECTION_EDGE_WIDTH), 0, 0);
    gl::draw(mFbo.getTexture(),FBO_DISPLAY1_AREA,DISPLAY1_RECT);
    glPopMatrix();
    
    // debug view
    
    //return;
    
    glLineWidth(10);
    glColor3f(0, 1, 0);
    gl::drawStrokedRect(DISPLAY0_RECT); // draw area display0
    glLineWidth(1);
    glEnable(GL_ALPHA_TEST);
    gl::enableAlphaBlending();
    
    glPushMatrix();
    //glTranslatef(-(0.5f + sinf(app::getElapsedSeconds()) * 0.5f) * float(PROJECTION_EDGE_WIDTH), 0, 0);
    glColor3f(1,0,0);
    gl::drawStrokedRect(Rectf(DISPLAY_WIDTH,0,
                              DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH, APP_VIEWPORT_HEIGHT));
    glColor4f(0, 0, 0, 0.75f);
    gl::drawSolidRect(Rectf(DISPLAY_WIDTH, 0,
                            DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH, APP_VIEWPORT_HEIGHT));
    drawStringWithBg(mDebugTexFont60, "Edge: " + toString(PROJECTION_EDGE_WIDTH), Vec2f(DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH * 0.5f,APP_VIEWPORT_HEIGHT * 0.5f));
    glLineWidth(10);
    glColor3f(0, 1, 0);
    gl::drawStrokedRect(DISPLAY1_RECT); // draw area display1
    glLineWidth(1);

    glPopMatrix();
    
    glColor3f(1, 1, 1);
    

    static const string display0String     = "Display 0: " + toString(DISPLAY_WIDTH) + " x " + toString(WINDOW_HEIGHT) ;
    static const string display1String     = "Display 1: " + toString(DISPLAY_WIDTH) + " x " + toString(WINDOW_HEIGHT) ;
    static const Vec2f  display0Center     = Vec2f(DISPLAY_WIDTH * 0.5f, APP_VIEWPORT_HEIGHT * 0.5f);
    static const Vec2f  display1Center     = Vec2f(DISPLAY_WIDTH + DISPLAY_WIDTH * 0.5f, display0Center.y);
    
    drawStringWithBg(mDebugTexFont60, display0String, display0Center);
    drawStringWithBg(mDebugTexFont60, "Viewport 0: " + toString(DISPLAY_WIDTH) + " x " + toString(APP_VIEWPORT_HEIGHT), Vec2f(display0Center.x,display0Center.y - 80));
    drawStringWithBg(mDebugTexFont60, display1String, display1Center);
    drawStringWithBg(mDebugTexFont60, "Viewport 1: " + toString(DISPLAY_WIDTH - PROJECTION_EDGE_WIDTH) + " x " + toString(APP_VIEWPORT_HEIGHT), Vec2f(display1Center.x,display1Center.y - 80));
    
    
    gl::disableAlphaBlending();
    glDisable(GL_ALPHA_TEST);
    //draw second screen
    

    
    
}

CINDER_APP_NATIVE( ProjectionSplitMergerApp, RendererGl )
