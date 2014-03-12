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
//  The app is configurable via a config.json.
//  Assumed display resolution: 1920 x 1200
//


class ProjectionSplitMergerApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
    void keyDown(KeyEvent event);
	void update();

    void drawScene();
	void draw();
#ifdef APP_DEBUG_DRAW_EDGE_BLENDING
    void drawStringWithBg(gl::TextureFontRef font, const string& str, const Vec2f& center);
    gl::TextureFontRef mDebugTextFont40;
#endif
    CameraOrtho mCamera;
    CameraPersp mCameraPersp;
    gl::Fbo     mFbo;
};

void ProjectionSplitMergerApp::prepareSettings(Settings* settings){
    // Load config
    config::Load(getResourcePath().string() + "/config.json");
    
    // Set Config
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

    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void ProjectionSplitMergerApp::setup(){
#ifdef APP_DEBUG_DRAW_EDGE_BLENDING
    mDebugTextFont40 = gl::TextureFont::create(Font("Apercu Mono", 20));
#endif
    float aspectRatio = float(APP_VIEWPORT_WIDTH) / float(APP_VIEWPORT_HEIGHT);
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10.0f, 10.0f);
    mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());

    gl::Fbo::Format fboFormat;
    fboFormat.setSamples(8);

    mFbo = gl::Fbo(APP_VIEWPORT_WIDTH,APP_VIEWPORT_HEIGHT,fboFormat);
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
    glPushMatrix();
    glRotatef(app::getElapsedSeconds() * 100, 0, 1, 0);
    gl::drawColorCube(zero, unit );
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
#ifdef APP_DEBUG_DRAW_EDGE_BLENDING
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
#endif

void ProjectionSplitMergerApp::draw(){
    drawScene(); // render scene to fbo
    
    gl::clear(Color(0,0,0));
    gl::setMatricesWindow(app::getWindowSize(),false);
    
    glColor3f(1,1,1);
    gl::draw(mFbo.getTexture(),FBO_DISPLAY0_AREA,DISPLAY0_RECT); // draw display 0
    gl::draw(mFbo.getTexture(),FBO_DISPLAY1_AREA,DISPLAY1_RECT); // draw display 1
    
#ifdef APP_DEBUG_DRAW_EDGE_BLENDING
    glLineWidth(10);
    glColor3f(0, 1, 0);
    gl::drawStrokedRect(DISPLAY0_RECT);
    glLineWidth(1);
    
    glEnable(GL_ALPHA_TEST);
    gl::enableAlphaBlending();
    
        glColor3f(1,0,0);
        gl::drawStrokedRect(Rectf(DISPLAY_WIDTH,0,
                                  DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH, APP_VIEWPORT_HEIGHT));
        glColor4f(0, 0, 0, 0.25f);
        gl::drawSolidRect(Rectf(DISPLAY_WIDTH, 0,
                                DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH, APP_VIEWPORT_HEIGHT));
        glLineWidth(10);
        glColor3f(0, 1, 0);
        gl::drawStrokedRect(DISPLAY1_RECT);
        glLineWidth(1);
        drawStringWithBg(mDebugTextFont40, "Edge: " + toString(PROJECTION_EDGE_WIDTH),
                         Vec2f(DISPLAY_WIDTH + PROJECTION_EDGE_WIDTH * 0.5f,APP_VIEWPORT_HEIGHT * 0.5f));
    
    static const string display0String     = "Display 0: " + toString(DISPLAY_WIDTH) + " x " + toString(WINDOW_HEIGHT) ;
    static const string display1String     = "Display 1: " + toString(DISPLAY_WIDTH) + " x " + toString(WINDOW_HEIGHT) ;
    static const string viewport0String    = "Viewport 0: " + toString(DISPLAY_WIDTH) + " x " + toString(APP_VIEWPORT_HEIGHT);
    static const string viewport1String    = "Viewpprt 1: " + toString(DISPLAY_WIDTH - PROJECTION_EDGE_WIDTH) + " x " + toString(APP_VIEWPORT_HEIGHT);
    static const Vec2f  display0Center     = Vec2f(DISPLAY_WIDTH * 0.5f, APP_VIEWPORT_HEIGHT * 0.5f);
    static const Vec2f  display1Center     = Vec2f(DISPLAY_WIDTH + DISPLAY_WIDTH * 0.5f, display0Center.y);
    static const Vec2f  viewport0Center    = Vec2f(display0Center.x,display0Center.y - 40);
    static const Vec2f  viewport1Center    = Vec2f(display1Center.x,display1Center.y - 40);
    
    drawStringWithBg(mDebugTextFont40, display0String,  display0Center);
    drawStringWithBg(mDebugTextFont40, viewport0String, viewport0Center);
    drawStringWithBg(mDebugTextFont40, display1String,  display1Center);
    drawStringWithBg(mDebugTextFont40, viewport1String, viewport1Center);
    
    gl::disableAlphaBlending();
    glDisable(GL_ALPHA_TEST);
#endif
}

CINDER_APP_NATIVE( ProjectionSplitMergerApp, RendererGl )
