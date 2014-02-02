//
//  ScriptContext.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "ScriptContext.h"
#include "Logger.h"

namespace scriptjs {
    ScriptContext::ScriptContext(){}
    
    ScriptContext::~ScriptContext(){
        mContext.Dispose();
    }
    
    Handle<ObjectTemplate> ScriptContext::getGlobalTemplate(){
        // create console object template
        Local<FunctionTemplate> consoleTemplate = FunctionTemplate::New(); // object
        consoleTemplate->Set("log",FunctionTemplate::New(scriptjs::log));  // object.method
        
        // create global object template
        Local<ObjectTemplate> globalTemplate = ObjectTemplate::New();
        globalTemplate->Set("console", consoleTemplate); // add console object
        
        return globalTemplate;
    };
    
    // this currently recreates a persistend context on execution
    bool ScriptContext::execute(const std::string &sourceJsOrFile){
        if(sourceJsOrFile.length() == 0){
            return false;
        }
        
        if (Context::InContext()) {
            //Context::Exit();
            //mContext.Exit();
            mContext.Dispose();
            
        }
        
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope handleScope(isolate);
        
        Handle<Context> context = Context::New(isolate,nullptr,this->getGlobalTemplate());
        Persistent<Context> contextPersistent(isolate,context);
        
        Context::Scope contextScope(context);
        Handle<Script> script = Script::Compile(String::New(sourceJsOrFile.c_str()));
        script->Run();
        
        contextPersistent.Dispose();
        return true;
    };
    
    
}


