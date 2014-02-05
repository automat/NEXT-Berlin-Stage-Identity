//
//  ModuleObj.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 05/02/14.
//
//

#ifndef ScriptJS_ModuleObj_cpp
#define ScriptJS_ModuleObj_cpp

#include "ScriptJS.h"
#include "ModuleClassCpp.h"

/*----------------------------------------------------------------------------------------------------*/
// The cpp object to be wrapped
/*----------------------------------------------------------------------------------------------------*/

class ModuleClassCppImpl{
protected:
    double mValue;
public:
    ModuleClassCppImpl(double value = 0) :
    mValue(value){};
    
    void add(const ModuleClassCppImpl& obj){
        mValue += obj.mValue;
    };
    
    void sub(const ModuleClassCppImpl& obj){
        mValue -= obj.mValue;
    }
    
    void incr(){
        mValue += 1.0;
    }
    
    void decr(){
        mValue -= 1.0;
    }
    
    void print(){
        std::cout << "ModuleObjImpl: " << mValue << std::endl;
    }
    
    float getValue(){
        return mValue;
    }
    
};

/*----------------------------------------------------------------------------------------------------*/
// The wrapper
/*----------------------------------------------------------------------------------------------------*/

class ModuleClassCppWrap : public scriptjs::ObjectWrap {
    static Persistent<Function> constructor;
    
    ModuleClassCppImpl mImpl; // ref impl
    
    ModuleClassCppWrap(double value = 0) : mImpl(value){}; // init impl
    ~ModuleClassCppWrap(){};
   
    /*
    static bool HasInstance(Handle<Value> value){
    
    }
     */
    
    /*----------------------------------------------------------------------------------------------------*/
    // converted object methods
    
    static void Add(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 1){
            args.GetReturnValue().Set(Undefined());
            return;
        }
        HandleScope handleScope(Isolate::GetCurrent());
        //Handle<Object> object = ::ModuleClassCppWrap::New::Object::New(args[0]);
        //ModuleClassCppWrap* wrap = //ObjectWrap::Unwrap<ModuleClassCppWrap>(args.);
        
        Handle<Value> value = args[0];
        if(value->IsUndefined()){
            
        }
        
        ModuleClassCppImpl& this_ = ObjectWrap::Unwrap<ModuleClassCppWrap>(args.This())->mImpl;
        ModuleClassCppImpl& that_ = ObjectWrap::Unwrap<ModuleClassCppWrap>(Handle<Object>::Cast(value))->mImpl;
        
        this_.add(that_);
        args.GetReturnValue().Set(Undefined());
    }
    
    static void Sub(const FunctionCallbackInfo<Value>& args){
        if(args.Length() != 1){
            args.GetReturnValue().Set(Undefined());
            return;
        }
        
        HandleScope handleScope(Isolate::GetCurrent());
        //Impl(args).sub(ObjectWrap::Unwrap<ModuleObjWrap>(args[0])->mImpl);
        args.GetReturnValue().Set(Undefined());
    }
    
    static void Incr(const FunctionCallbackInfo<Value>& args){
        HandleScope handleScope(Isolate::GetCurrent());
        ObjectWrap::Unwrap<ModuleClassCppWrap>(args.This())->mImpl.incr();
        args.GetReturnValue().Set(Undefined());
    }
    
    static void Decr(const FunctionCallbackInfo<Value>& args){
        HandleScope handleScope(Isolate::GetCurrent());
        ObjectWrap::Unwrap<ModuleClassCppWrap>(args.This())->mImpl.decr();
        args.GetReturnValue().Set(Undefined());
    }
    
    static void GetValue(const FunctionCallbackInfo<Value>& args){
        HandleScope handleScope(Isolate::GetCurrent());
        ModuleClassCppImpl& this_ = ObjectWrap::Unwrap<ModuleClassCppWrap>(args.This())->mImpl;
        args.GetReturnValue().Set(Number::New(this_.getValue()));
    }
    
    static void Print(const FunctionCallbackInfo<Value>& args){
        HandleScope handleScope(Isolate::GetCurrent());
        ObjectWrap::Unwrap<ModuleClassCppWrap>(args.This())->mImpl.print();
        args.GetReturnValue().Set(Undefined());
    }
    
    /*----------------------------------------------------------------------------------------------------*/
    
    
public:
    
    /*----------------------------------------------------------------------------------------------------*/
    
    // create js constructor
    static void Init(Handle<Object> exports){
        Isolate* isolate = Isolate::GetCurrent();
        
        const char* className = "ClassCpp";
        
        //Prepare constructor template
        Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
        tpl->SetClassName(String::NewFromUtf8(isolate, className));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);
        
        // add methods to protoype
        SET_PROTOTYPE_METHOD(tpl, "add", Add);
        SET_PROTOTYPE_METHOD(tpl, "sub", Sub);
        SET_PROTOTYPE_METHOD(tpl, "incr", Incr);
        SET_PROTOTYPE_METHOD(tpl, "decr", Decr);
        SET_PROTOTYPE_METHOD(tpl, "getValue", GetValue);
        SET_PROTOTYPE_METHOD(tpl, "print", Print);
        
        ModuleClassCppWrap::constructor.Reset(isolate,tpl->GetFunction());
        exports->Set(String::NewFromUtf8(isolate, className), tpl->GetFunction());
    }
    
    // create js constructor
    static void New(const FunctionCallbackInfo<Value>& args){
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);
        
        if (args.IsConstructCall()) {
            // Invoked as constructor: `new MyObject(...)`
            double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
            ModuleClassCppWrap* obj = new ModuleClassCppWrap(value);
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            // Invoked as plain function `MyObject(...)`, turn into construct call.
            const int argc = 1;
            Local<Value> argv[argc] = { args[0] };
            Local<Function> cons = Local<Function>::New(isolate, ModuleClassCppWrap::constructor);
            args.GetReturnValue().Set(cons->NewInstance(argc, argv));
        }
    }
    
    /*----------------------------------------------------------------------------------------------------*/
};

//init static
Persistent<Function> ModuleClassCppWrap::constructor;

void ModuleClassCpp::Initialize(v8::Handle<v8::Object> exports){
    ModuleClassCppWrap::Init(exports);
}



#endif
