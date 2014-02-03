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
        SET_METHOD(consoleTemplate, "log", scriptjs::log);
        
        // create global object template
        Local<ObjectTemplate> globalTemplate = ObjectTemplate::New();
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
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    bool ScriptContext::loadScript(const std::string &sourceJsOrFile){
        Isolate* isolate = Isolate::GetCurrent(); // get default Instance of v8
        
        Isolate::Scope isolateScope(isolate); // enter isolate scope
        HandleScope    handleScope(isolate);  // enter handle scope
        
            Handle<Context> context = Context::New(isolate,NULL,this->getGlobalTemplate()); // create new context
                mContext.Reset(isolate, context); // update shared persistent context with newly created one
                TryCatch tryCatch;
        
                Context::Scope contexScope(context); // enter context scope
        
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
                                
                            }
                            return true;
                        }
                    }
                // ~ exit context scope
            // ~ exit handle scope
        // ~ exit isolate scope
    };
    
    

    void ScriptContext::addModule(scriptjs::Module *module){
        mModules.push_back(module);
    }
    
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    
    Handle<Object> ScriptContext::getNewInstance(const std::string &name){
        Isolate* isolate = Isolate::GetCurrent(); //get instance of v8
        
        Isolate::Scope isolateScope(isolate); // enter isolate scope
            HandleScope  handleScope(isolate); // enter handle scope
        
            Handle<Context> context = v8::Handle<Context>::New(isolate,mContext); // recreate context from persistent context
                TryCatch tryCatch;
        
                    Context::Scope contexScope(context); // enter context
        
                        Handle<Value>    value    = context->Global()->Get(ToV8String(name.c_str()));
                        Handle<Function> function = Handle<Function>::Cast(value);
                        Handle<Value>    result   = function->NewInstance();
        
                        if (result.IsEmpty()) {
                            ReportException(&tryCatch);
                        }
                    // ~ exit context scope
        return handleScope.Close(Handle<Object>::Cast(result)); // ~ exit handle scope with object
        // ~ exit isolate scope
    };
    
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



