#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class QuoteLayoutApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void QuoteLayoutApp::setup()
{
}

void QuoteLayoutApp::mouseDown( MouseEvent event )
{
}

void QuoteLayoutApp::update()
{
}

void QuoteLayoutApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( QuoteLayoutApp, RendererGl )
