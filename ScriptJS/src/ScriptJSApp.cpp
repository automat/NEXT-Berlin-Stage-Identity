

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include <iostream>
#include <fstream>

#include "ScriptJS.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ScriptJSApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    scriptjs::ScriptContext mScriptContext;
    
    
};

void ScriptJSApp::setup(){
    std::ifstream in("/Users/automat/Projects/next/ScriptJS/resources/script.js");
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    mScriptContext.execute(source);
}

void ScriptJSApp::mouseDown( MouseEvent event )
{
}

void ScriptJSApp::update()
{
}

void ScriptJSApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( ScriptJSApp, RendererGl )
