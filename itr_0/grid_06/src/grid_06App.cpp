#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class grid_06App : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void grid_06App::setup()
{
}

void grid_06App::mouseDown( MouseEvent event )
{
}

void grid_06App::update()
{
}

void grid_06App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( grid_06App, RendererGl )
