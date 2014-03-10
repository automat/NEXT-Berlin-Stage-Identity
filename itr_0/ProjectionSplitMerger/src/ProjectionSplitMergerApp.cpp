#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "config.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class ProjectionSplitMergerApp : public AppNative {
  public:
	void setup();
    void prepareSettings(Settings* settings);
	void update();

    void drawScene();
	void draw();
    
    CameraOrtho mCamera;
};

void ProjectionSplitMergerApp::prepareSettings(Settings* settings){
    settings->setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    settings->setFrameRate(APP_FPS);
}

void ProjectionSplitMergerApp::setup(){
    float aspectRatio = float(VIEWPORT_WIDTH) / float(VIEWPORT_HEIGHT);//app::getWindowAspectRatio();
    mCamera.setOrtho(-aspectRatio, aspectRatio, -1, 1, -10.0f, 1);
    mCamera.lookAt(Vec3f(-1,1,-1), Vec3f::zero());
    glEnable(GL_SCISSOR_TEST);
}

void ProjectionSplitMergerApp::drawScene(){
    glPushAttrib(GL_VIEWPORT_BIT);
    glScissor(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    gl::clear( Color( 0.15f, 0.15f, 0.25f ) );
    gl::setMatrices(mCamera);
    
    static const Vec3f zero;
    static const Vec3f unit(1,1,1);
    glPushMatrix();
    glScalef(3,3,3);
    gl::drawColorCube(zero, unit);
    glPopMatrix();
    glPopAttrib();
}

void ProjectionSplitMergerApp::update(){
}

void ProjectionSplitMergerApp::draw()
{
    /*
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glPopAttrib();
    */
    glScissor(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    gl::clear(Color(0,0,0));
    drawScene();
}

CINDER_APP_NATIVE( ProjectionSplitMergerApp, RendererGl )
