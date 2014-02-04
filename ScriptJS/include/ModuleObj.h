//
//  WrappedObj.h
//  ScriptJS
//
//  Created by Henryk Wollik on 03/02/14.
//
//

#ifndef ScriptJS_WrappedObj_h
#define ScriptJS_WrappedObj_h

#include "ScriptJS.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"

//a sample wrapped object
using namespace v8;
class ModuleObj : public scriptjs::Module {
    
    
    
public:
    void Initialize(Handle<Object> target){
        SET_METHOD(target, "GetAverageFps", GetAverageFps);
        
    }
    
    Handle<Value> GetAverageFps(const FunctionCallbackInfo<Value>& args){
        return Number::New(ci::app::App::get()->getAverageFps());
    }
    
    
    
    void method(){
        
    }
    
};



#endif
