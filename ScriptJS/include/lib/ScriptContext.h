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
#include <vector>
#include "ScriptJS_Prefix.h"

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
    //! Exception to be thrown on no file
    class ScriptContexExcNoFile : public std::exception{
        std::string mMessage;
    public:
        ScriptContexExcNoFile(const std::string& msg) : mMessage("File does not exist: " + msg){};
        inline virtual const char* what() const throw(){ return mMessage.c_str();}
    };
    //! Exception to be thrown on wrong extension
    class ScriptContexExcNoJSFile : public std::exception{
        string mMessage;
    public:
        ScriptContexExcNoJSFile(const std::string& msg) : mMessage("Wrong file extension: " + msg){};
        inline virtual const char* what() const throw(){ return mMessage.c_str();}
    };
    
    class Module;
    
    
    class ScriptContext{
        Persistent<Context> mContext; //shared persistent execution context
        vector<Module*>     mModules;
        
        static Handle<ObjectTemplate> GetGlobalTemplate();
        static void Require(const FunctionCallbackInfo<Value>& args);
        
    public:
        ~ScriptContext();
        
        //! get Handle to persistent context
        Handle<Context> getContext();
        
        //! add Module to global scope
        void addModule(Module* module);
        
        //! load and execute Script
        bool loadScript(const std::string& filepath);
        
        //! call function in global scope
        Handle<Value> call(const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        //! call function in scope of object
        Handle<Value> call(Persistent<Object>& obj, const char* name, int argc = 0, Handle<Value>* argv = NULL);
        
        //! get new instance of js object
        Handle<Object> newInstance(const std::string& name, int argc = 0, Handle<Value>* argv = NULL);
        
        //! dipose context and clear modules
        void dispose();
     };
}

#endif /* defined(__ScriptJS__ScriptContext__) */
