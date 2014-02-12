//
//  Light.h
//  ScriptJS
//
//  Created by Henryk Wollik on 11/02/14.
//
//

#include "Light.h"
#include "ScriptJS.h"
#include "cinder/gl/Light.h"


using namespace ci;

class LightWrap : public scriptjs::ObjectWrap {
    static Persistent<Function> constructor;
    gl::Light* mImpl;
    
    LightWrap(int aType, int aID){
        mImpl = new gl::Light(aType,aID);
    }
    
    static gl::Light* Impl(const FunctionCallbackInfo<Value>& args){
        LightWrap* wrap = ObjectWrap::Unwrap<LightWrap>(args.This());
        return wrap->mImpl;
    }
    
public:
    // create js constructor
    static void Init(Handle<Object> exports){
        Isolate* isolate = Isolate::GetCurrent();
        
        const char* className = "Light";
        
        //Prepare constructor template
        Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
        tpl->SetClassName(String::NewFromUtf8(isolate, className));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        
        SET_PROPERTY_READONLY(tpl, "POINT",scriptjs::ToV8Int(gl::Light::POINT));
        SET_PROPERTY_READONLY(tpl, "DIRECTIONAL", scriptjs::ToV8Int(gl::Light::DIRECTIONAL));
        SET_PROPERTY_READONLY(tpl, "SPOTLIGHT", scriptjs::ToV8Int(gl::Light::SPOTLIGHT));
        
        
        
        
        LightWrap::constructor.Reset(isolate,tpl->GetFunction());
        exports->Set(String::NewFromUtf8(isolate, className), tpl->GetFunction());
    }
    
    // create js constructor
    static void New(const FunctionCallbackInfo<Value>& args){
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);
        
        if (args.IsConstructCall()) {
            // Invoked as constructor: `new MyObject(...)`
            LightWrap* obj;
            int argc = args.Length();
            switch (argc) {
                case 0:
                    obj = new LightWrap(gl::Light::POINT,0);
                break;
                case 1:
                    obj = new LightWrap(args[0]->Int32Value(),0);
                break;
                case 2:
                    obj = new LightWrap(args[0]->Int32Value(), args[1]->Int32Value());
                break;
            }
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            // Invoked as plain function `MyObject(...)`, turn into construct call.
            Local<Function> cons = Local<Function>::New(isolate, LightWrap::constructor);
            args.GetReturnValue().Set(cons->NewInstance());
        }
    }
    
};

//init static
Persistent<Function> LightWrap::constructor;

void Light::Initialize(v8::Handle<v8::Object> exports){
    LightWrap::Init(exports);
}