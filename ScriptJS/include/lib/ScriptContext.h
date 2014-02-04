//
//  ScriptContext.h
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#ifndef __ScriptJS__ScriptContext__
#define __ScriptJS__ScriptContext__

#include <iostream>
#include "ScriptJS_Prefix.h"
#include <vector>

#define ENTER_CONTEXT(scriptContext) \
    v8::Isolate* isolate = v8::Isolate::GetCurrent(); \
        v8::HandleScope handleScope(isolate); \
        v8::Handle<Context> context = scriptContext.getContext(); \
            v8::Context::Scope contextScope(context); \
        try{

#define EXIT_CONTEXT \
    } catch(std::exception& exc) { \
        std::cout << exc.what() << std::endl; \
        return; \
    }

using namespace std;
namespace scriptjs {
    
    class Module;
    
    
    class ScriptContext{
        Persistent<Context> mContext; //shared persistent execution context
        vector<Module*>     mModules;

        Handle<ObjectTemplate> getGlobalTemplate();
        
    public:
        ScriptContext();
        ~ScriptContext();
        
        // get Context
        Handle<Context> getContext();
        
        // add Module to global scope
        void addModule(Module* module);
        
        // load and execute Script
        bool loadScript(const std::string& sourceJsOrFile);
        
        // call function in global scope
        Handle<Value> call(const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        // call function in scope of object
        Handle<Value> call(Persistent<Object>& obj, const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        // get new instance of js object
        Handle<Object> newInstance(const std::string& name, int argc = 0, Handle<Value>* argv = NULL);
     };
}

#endif /* defined(__ScriptJS__ScriptContext__) */
