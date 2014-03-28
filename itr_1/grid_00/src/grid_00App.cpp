#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class grid_00App : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void grid_00App::setup()
{
}

void grid_00App::mouseDown( MouseEvent event )
{
}

void grid_00App::update()
{
}

void grid_00App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( grid_00App, RendererGl )
