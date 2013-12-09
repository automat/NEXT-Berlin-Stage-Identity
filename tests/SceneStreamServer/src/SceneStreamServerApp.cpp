#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SceneStreamServerApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void SceneStreamServerApp::setup()
{
}

void SceneStreamServerApp::mouseDown( MouseEvent event )
{
}

void SceneStreamServerApp::update()
{
}

void SceneStreamServerApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( SceneStreamServerApp, RendererGl )
