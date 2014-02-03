

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
    
    scriptjs::ScriptContext    mScriptContext;
    v8::Persistent<v8::Object> mContextJS;

    
    
};

void ScriptJSApp::setup(){
    std::ifstream in("/Users/automat/Projects/next/ScriptJS/resources/script.js");
    std::string source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    mScriptContext.loadScript(source);
   
    /*
    CONTEXTJS_ENTER_SCOPE(mScriptContext);
    mScriptContext.getNewInstance("ContextJS");
    CONTEXTJS_EXIT_SCOPE;
    */
    v8::Handle<v8::Object> contextJS = mScriptContext.getNewInstance("ContextJS");
    mContextJS = v8::Persistent<v8::Object>::New(<#v8::Isolate *isolate#>, <#v8::Object *that#>)
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
