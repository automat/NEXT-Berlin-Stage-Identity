

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include <iostream>
#include <fstream>

#include "ScriptJS.h"
#include "ModuleClassCpp.h"

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

    mScriptContext.addModule(new ModuleClassCpp());
    mScriptContext.loadScript(source);
    

    
    //new ModuleObj();
    ENTER_CONTEXT(mScriptContext);
    //mContextJS.Reset(isolate, mScriptContext.newInstance("ContextJS"));
    mScriptContext.call("Init");
    EXIT_CONTEXT;
}

void ScriptJSApp::mouseDown( MouseEvent event ){
    ENTER_CONTEXT(mScriptContext);
    mScriptContext.call("MouseDown");
    EXIT_CONTEXT;
}

void ScriptJSApp::update(){
    
    ENTER_CONTEXT(mScriptContext);
    Handle<Value> args[2];
        args[0] = scriptjs::ToV8Num(app::getElapsedSeconds());
        args[1] = scriptjs::ToV8Int(app::getElapsedFrames());
    mScriptContext.call("Update",2,args);
    EXIT_CONTEXT;
    
}

void ScriptJSApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
   // mScriptContext.call("draw");
}

CINDER_APP_NATIVE( ScriptJSApp, RendererGl )
