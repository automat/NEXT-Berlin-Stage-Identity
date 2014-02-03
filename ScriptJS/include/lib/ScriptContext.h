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
        
        bool execute(const std::string& sourceJsOrFile);
        
        // creates a new javascript object instance
        Handle<Object> newInstance(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);
        
        // calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        // calls the javascript function within the given context
        Handle<Value> call(Handle<Object> localContext, Handle<String> name, int argc = 0, Handle<Value>* argv = NULL);
    };
}

#endif /* defined(__ScriptJS__ScriptContext__) */
