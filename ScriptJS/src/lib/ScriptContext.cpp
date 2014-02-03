//
//  ScriptContext.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "ScriptContext.h"
#include "Logger.h"
#include "Module.h"
#include <boost/foreach.hpp>

namespace scriptjs {
    ScriptContext::ScriptContext(){}
    
    ScriptContext::~ScriptContext(){
        mContext.Dispose();
        
        BOOST_FOREACH(Module* m, mModules){
            delete m;
        }
        
        mModules.clear();
    }
    
    Handle<ObjectTemplate> ScriptContext::getGlobalTemplate(){
        // create console object template
        Local<FunctionTemplate> consoleTemplate = FunctionTemplate::New(); // object
        //consoleTemplate->Set("log",FunctionTemplate::New(scriptjs::log));  // object.method
        SET_METHOD(consoleTemplate, "log", scriptjs::log);
        
        
        
        // create global object template
        Local<ObjectTemplate> globalTemplate = ObjectTemplate::New();
        //globalTemplate->Set("console", consoleTemplate); // add console object
        SET_PROPERTY(globalTemplate, "console", consoleTemplate);
        SET_PROPERTY_READONLY(globalTemplate, "const_number", ToV8Num(1000.0));;
        
        
        return globalTemplate;
    };
    
    // execute string within a given js context
    bool ScriptContext::executeString(Handle<String> str){
        Handle<Context> handleContext;
        TryCatch tryCatch;
        Handle<Script> script = Script::Compile(str);
        
        if(script.IsEmpty()){
            ReportException(&tryCatch);
            return false;
        } else {
            Handle<Value> result = script->Run();
            if (result.IsEmpty()) {
                ReportException(&tryCatch);
                return false;
            } else {
                if(!result->IsUndefined()){
                    
                }
                return true;
            }
        }
    };
    
    bool ScriptContext::execute(const std::string &sourceJsOrFile){
        if(sourceJsOrFile.length() == 0){
            return false;
        }

        //if context already existe, eg created dispose (not sure if also done in Context.Reset())
        if(Context::InContext()){
            mContext.Dispose();
        }
    
        // get current isolate / create handle scope
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope handleScope(isolate);
        Handle<Context> context = Context::New(isolate,nullptr,this->getGlobalTemplate());
        // dispose old context, recreate new if initialized already
        mContext.Reset(isolate, context);
        // enter new context
        Context::Scope contextScope(context);
        // compile string
        return this->executeString(String::New(sourceJsOrFile.c_str()));
    };

    void ScriptContext::addModule(scriptjs::Module *module){
        mModules.push_back(module);
    }
    
    Handle<Object> ScriptContext::newInstance(Handle<Object> localContext, Handle<String> name, int argc, Handle<Value>* argv){
        HandleScope handleScope(Isolate::GetCurrent());
        TryCatch tryCatch;
        
        Handle<Value> value = localContext->Get(name);
        Handle<Function> func = Handle<Function>::Cast(value);
        Handle<Value> result = func->NewInstance(argc, argv);
        
        if(result.IsEmpty())
            ReportException(&tryCatch);
        
        return handleScope.Close(Handle<Object>::Cast(result));
    }
    
    Handle<Value> ScriptContext::call(Handle<Object> localContext, const char* name, int argc, Handle<Value>* argv){
        return this->call(localContext, String::New(name), argc, argv);
    }
    
    
    Handle<Value> ScriptContext::call(Handle<Object> localContext, Handle<String> name, int argc, Handle<Value>* argv)
    {
        HandleScope handleScope(Isolate::GetCurrent());
        TryCatch tryCatch;
        
        Handle<Value> value = localContext->Get(name);
        
        // check if function exists within given context
        if(!value->IsUndefined()) {
            Handle<Function> func = Handle<Function>::Cast(value);
            Handle<Value> result = func->Call(localContext, argc, argv);
            
            if(result.IsEmpty())
                ReportException(&tryCatch);
            
            return handleScope.Close(result);
        }
        
        return Undefined();
    }
    
}



