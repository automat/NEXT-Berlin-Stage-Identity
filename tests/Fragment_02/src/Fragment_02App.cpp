#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Fragment_02App : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void Fragment_02App::setup()
{
}

void Fragment_02App::mouseDown( MouseEvent event )
{
}

void Fragment_02App::update()
{
}

void Fragment_02App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( Fragment_02App, RendererGl )
