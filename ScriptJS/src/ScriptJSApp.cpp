

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
    mScriptContext.addModule(new ModuleClassCpp());
    mScriptContext.loadScript("/Users/automat/Projects/next/ScriptJS/resources/script.js");
    
    ENTER_CONTEXT(mScriptContext);
    const int argc = 2;
    Handle<Value> argv[argc] = {scriptjs::ToV8Int(app::getWindowWidth()),
                                scriptjs::ToV8Int(app::getWindowHeight())};
    mContextJS.Reset(isolate, mScriptContext.newInstance("ContextJS",argc,argv));
    EXIT_CONTEXT;
    
}

void ScriptJSApp::mouseDown( MouseEvent event ){
    /*
    ENTER_CONTEXT(mScriptContext);
    mScriptContext.call("MouseDown");
    EXIT_CONTEXT;
     */
}

void ScriptJSApp::update(){
    ENTER_CONTEXT(mScriptContext);
    static const int argc = 2;
    Handle<Value> argv[argc];
        argv[0] = scriptjs::ToV8Num(app::getElapsedSeconds());
        argv[1] = scriptjs::ToV8Int(app::getElapsedFrames());
    mScriptContext.call(mContextJS,"update",argc,argv);
    EXIT_CONTEXT;
}

void ScriptJSApp::draw(){
	gl::clear( Color( 0, 0, 0 ) );
    ENTER_CONTEXT(mScriptContext);
    mScriptContext.call(mContextJS,"draw");
    EXIT_CONTEXT;
}

CINDER_APP_NATIVE( ScriptJSApp, RendererGl )
