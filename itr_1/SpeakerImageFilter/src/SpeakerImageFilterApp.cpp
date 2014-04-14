#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpeakerImageFilterApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void SpeakerImageFilterApp::setup()
{
}

void SpeakerImageFilterApp::mouseDown( MouseEvent event )
{
}

void SpeakerImageFilterApp::update()
{
}

void SpeakerImageFilterApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( SpeakerImageFilterApp, RendererGl )
