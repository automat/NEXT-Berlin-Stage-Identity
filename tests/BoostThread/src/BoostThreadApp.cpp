#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <thread>

using namespace ci;
using namespace ci::app;
using namespace std;

class BoostThreadApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void BoostThreadApp::setup()
{
}

void BoostThreadApp::mouseDown( MouseEvent event )
{
}

void BoostThreadApp::update()
{
}

void BoostThreadApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( BoostThreadApp, RendererGl )
