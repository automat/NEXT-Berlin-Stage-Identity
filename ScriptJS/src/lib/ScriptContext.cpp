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

//  Get v8 instance
//      Enter isolate scope
//          Enter handle scope

#define CONTEXT_ENTER_HANDLE_SCOPE \
Isolate* isolate = Isolate::GetCurrent(); \
    Isolate::Scope isolateScope(isolate); \
        HandleScope handleScope(isolate);

#define CONTEXT_ENTER_CONTEXT_SCOPE \
    CONTEXT_ENTER_HANDLE_SCOPE; \
        Handle<Context> context = v8::Handle<Context>::New(isolate, mContext); \
        Context::Scope contextScope(context);

namespace scriptjs {
    ScriptContext::ScriptContext(){}
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    ScriptContext::~ScriptContext(){
        mContext.Dispose();
        while (!mModules.empty()) delete mModules.back(), mModules.pop_back();
    }
    
    void ScriptContext::addModule(scriptjs::Module *module){
        mModules.push_back(module);
    }
    
    Handle<Context> ScriptContext::getContext(){
        return Local<Context>::New(Isolate::GetCurrent(), mContext);
    }
    
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    Handle<ObjectTemplate> ScriptContext::getGlobalTemplate(){
        // create console object template
        Local<FunctionTemplate> consoleTemplate = FunctionTemplate::New(); // object
        SET_METHOD(consoleTemplate, "log", scriptjs::log);
        
        // create global object template
        Local<ObjectTemplate> globalTemplate = ObjectTemplate::New();
        SET_PROPERTY(globalTemplate, "console", consoleTemplate);
        
        return globalTemplate;
    };
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    bool ScriptContext::loadScript(const std::string &sourceJsOrFile){
        CONTEXT_ENTER_HANDLE_SCOPE;
            Handle<Context> context = Context::New(isolate,NULL,this->getGlobalTemplate()); // create new context
                mContext.Reset(isolate, context); // update shared persistent context with newly created one
                TryCatch tryCatch;
        
                Context::Scope contexScope(context); // enter context scope
                    Handle<Object> global = context->Global();
        
                    // add global available modules
                    for (vector<Module*>::iterator itr = mModules.begin(); itr != mModules.end(); ++itr) {
                        (*itr)->Initialize(global);
                    }
        
                    Handle<Script> script = Script::Compile(String::New(sourceJsOrFile.c_str()));
                    if(script.IsEmpty()){
                        ReportException(&tryCatch);
                        return false;
                    } else {
                        Handle<Value> result = script->Run();
                        if (result.IsEmpty()) {
                            ReportException(&tryCatch);
                            return false;
                        } else {
                            if (result->IsUndefined()) {
                                // no object instance returned, global func scope assumed
                            }
                            return true;
                        }
                    }
                // ~ exit context scope
            // ~ exit handle scope
        // ~ exit isolate scope
    };
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    Handle<Value> ScriptContext::call(const char *name, int argc, Handle<Value>* argv){
        CONTEXT_ENTER_HANDLE_SCOPE;
            Handle<Context> context = v8::Handle<Context>::New(isolate, mContext);
                TryCatch tryCatch;
                Context::Scope contextScope(context);
        
                    Handle<Object> global = context->Global();
                    Handle<Value> value  = global->Get(String::New(name));
        
                    if(!value->IsUndefined()) {
                            Handle<Function> func = Handle<Function>::Cast(value);
                            Handle<Value> result = func->Call(global,argc,argv);
            
                        if(result.IsEmpty()){
                            ReportException(&tryCatch);
                        }
                        
                        return handleScope.Close(result);
                    }
                // ~ exit context scope
            // ~ exit handle scope
        // ~ exit isolate scope
        return Undefined();
    };
    
    Handle<Value> ScriptContext::call(Persistent<v8::Object>& obj, const char *name, int argc, Handle<Value>* argv){
        CONTEXT_ENTER_CONTEXT_SCOPE;
        TryCatch tryCatch;
        Handle<Object> object = Handle<Object>::New(isolate, obj);
        Handle<Value> value = object->Get(String::New(name));
        
        // check if function exists within given context
        if(!value->IsUndefined()) {
            Handle<Function> func = Handle<Function>::Cast(value);
            Handle<Value> result = func->Call(object, argc, argv);
            
            if(result.IsEmpty()){
                ReportException(&tryCatch);
            }
            
            return handleScope.Close(result);
        }
        
        return Undefined();
    };
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    Handle<Object> ScriptContext::newInstance(const std::string &name, int argc, Handle<Value>* argv){
        CONTEXT_ENTER_HANDLE_SCOPE;
            Handle<Context> context = v8::Handle<Context>::New(isolate,mContext); // recreate context from persistent context
                TryCatch tryCatch;
        
                    Context::Scope contexScope(context); // enter context
        
                        Handle<Value>    value    = context->Global()->Get(ToV8String(name.c_str()));
                        Handle<Function> function = Handle<Function>::Cast(value);
                        Handle<Value>    result   = function->NewInstance(argc,argv);
        
                        if (result.IsEmpty()) {
                            ReportException(&tryCatch);
                        }
                    // ~ exit context scope
        return handleScope.Close(Handle<Object>::Cast(result)); // ~ exit handle scope with object
        // ~ exit isolate scope
    };
    
}



