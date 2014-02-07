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

#include <iostream>
#include <fstream>
#include <vector>

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
    using namespace std;
    /*------------------------------------------------------------------------------------------------------------*/
    
    ScriptContext::~ScriptContext(){
        this->dispose();
    }
    
    // TODO: This should be mapped to a created ScriptContext
    static string scriptDir = "";
    static map<string,Persistent<Context>> requiredModules;
    
    
    void ScriptContext::dispose(){
        scriptDir = "";
        requiredModules.clear();
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
    
    Handle<ObjectTemplate> ScriptContext::GetGlobalTemplate(){
        // create console object template
        Local<ObjectTemplate> consoleTemplate = ObjectTemplate::New();
        SET_METHOD(consoleTemplate, "log", scriptjs::log);
        
        // create global object template
        Local<ObjectTemplate> globalTemplate = ObjectTemplate::New();
        SET_PROPERTY(globalTemplate, "console", consoleTemplate);
        
        // fake node functionality
        Local<ObjectTemplate> moduleTemplate = ObjectTemplate::New();
        //SET_PROPERTY(moduleTemplate, "exports", Handle<Value>());
        SET_PROPERTY(globalTemplate, "module", moduleTemplate);
        
        // pseudo require
        SET_METHOD(globalTemplate, "require", ScriptContext::Require);
        
        return globalTemplate;
    };
    
    void ScriptContext::Require(const FunctionCallbackInfo<Value>& args){
        CONTEXT_ENTER_HANDLE_SCOPE;
        
        if(args.Length() != 1){ // wrong arg length
            args.GetReturnValue().Set(Undefined());
            return;
        }
        
        Handle<Value> path(args[0]);
        if(!path->IsString()){ // no string, no use
            args.GetReturnValue().Set(Undefined());
            return;
        }
        
        string pathStr(ToCString(path->ToString()));
        size_t ext(pathStr.find_last_of("."));
        if(ext == string::npos){ // no extension
            pathStr += ".js";
        } else {
            if( pathStr.substr(ext + 1) != "js"){ // wrong extension
                args.GetReturnValue().Set(Undefined());
                return;
            }
        }
        
        // create full absolute path
        pathStr = scriptDir + pathStr;
        
        // load file
        ifstream in(pathStr.c_str());
        if(!in){
            args.GetReturnValue().Set(Undefined());
            return;
        }
        
        // get module name
        string moduleName = pathStr.substr(pathStr.find_last_of("/") + 1);
               moduleName = moduleName.substr(0,moduleName.find_last_of("."));
        
        Handle<Context> context;
        
        // check if module has already been required
        if(requiredModules.find(moduleName) != requiredModules.end()){
            context = Handle<Context>::New(isolate,requiredModules[moduleName]);
            Context::Scope contextScope(context);
            Handle<Object> contextGlobal = context->Global();
            Handle<Object> exports = Handle<Object>::Cast(Handle<Object>::Cast(contextGlobal->Get(ToV8String("module")))->Get(ToV8String("exports")));
            args.GetReturnValue().Set(exports);
            return;
        } // otherwise create new
        
        string source((istreambuf_iterator<char>(in)),istreambuf_iterator<char>());
        
        // create module context with all necessary functionality
        context = Context::New(isolate,NULL,ScriptContext::GetGlobalTemplate());
        Context::Scope contextScope(context);
        
            Handle<Script> script = Script::Compile(String::New(source.c_str()));
        
            if (script.IsEmpty()) {
                args.GetReturnValue().Set(Undefined());
                return;
            } else {
                Handle<Value> result = script->Run();
                if(result.IsEmpty()){
                    args.GetReturnValue().Set(Undefined());
                    return;
                }
                
                // check if exports is defined
                Handle<Value> valueModule = context->Global()->Get(ToV8String("module"));
                if(valueModule.IsEmpty()){
                    args.GetReturnValue().Set(Undefined());
                    return;
                }
                
                Handle<Object> objectModule = Handle<Object>::Cast(valueModule);
                Handle<Value>  valueExports = objectModule->Get(ToV8String("exports"));
                
                if(valueExports.IsEmpty()){
                    args.GetReturnValue().Set(Undefined());
                    return;
                }
                
                //push context to requiredModules map
                requiredModules[moduleName].Reset(isolate, context);
                
                //return the export
                args.GetReturnValue().Set(valueExports);
            }
    }
    
    /*------------------------------------------------------------------------------------------------------------*/
    
    bool ScriptContext::loadScript(const std::string& filepath){
        // is path valid?
        ifstream in(filepath.c_str());
        if(!in){
            throw ScriptContexExcNoFile(filepath);
        }
        // javascript ?
        if(filepath.substr(filepath.find_last_of(".") + 1) != "js"){
            throw ScriptContexExcNoJSFile(filepath);
        }
        // get script directory for imports
        scriptDir = filepath.substr(0,filepath.find_last_of("/") + 1);
   
        string source((istreambuf_iterator<char>(in)),istreambuf_iterator<char>());
        
        CONTEXT_ENTER_HANDLE_SCOPE;
            Handle<Context> context = Context::New(isolate,NULL,GetGlobalTemplate()); // create new context
                mContext.Reset(isolate, context); // update shared persistent context with newly created one
                TryCatch tryCatch;
        
                Context::Scope contexScope(context); // enter context scope
                    Handle<Object> global = context->Global();
        
                    // add global available modules
                    for (vector<Module*>::iterator itr = mModules.begin(); itr != mModules.end(); ++itr) {
                        (*itr)->Initialize(global);
                    }
        
                    Handle<Script> script = Script::Compile(String::New(source.c_str()));
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



