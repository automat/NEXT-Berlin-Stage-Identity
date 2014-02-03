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



#define CONTEXTJS_ENTER_SCOPE(instance) \
v8::Isolate* isolate = v8::Isolate::GetCurrent(); \
    v8::Isolate::Scope isolateScope(isolate); \
        v8::HandleScope handleScope(isolate); \
            v8::Handle<v8::Context> context = v8::Handle<v8::Context>::New(isolate,instance.getContext()); \
            try {

#define CONTEXTJS_EXIT_SCOPE \
} catch(std::exception& e) { \
std::cout << e.what() << std::endl; \
return; \
}


using namespace std;
namespace scriptjs {
    
    class Module;
    
    
    class ScriptContext{
        Persistent<Context> mContext; //shared persistent execution context
        vector<Module*>     mModules;
        
        bool executeString(Handle<String> str);
        Handle<ObjectTemplate> getGlobalTemplate();
        
    public:
        ScriptContext();
        ~ScriptContext();
        
        
        void addModule(Module* module);
        
        bool loadScript(const std::string& sourceJsOrFile);
        
        
        Handle<Object> getNewInstance(const std::string& name);
        
        // creates a new javascript object instance
        Handle<Object> newInstance(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);
        
        // calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        // calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);
    };
}

#endif /* defined(__ScriptJS__ScriptContext__) */
