//
//  ScriptJS_Prefix.h
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#ifndef ScriptJS_ScriptJS_Prefix_h
#define ScriptJS_ScriptJS_Prefix_h

#include <v8.h>

using namespace v8;

namespace scriptjs {
    //
    // Macros
    //
#define SET_PROPERTY(obj, name, value) \
    obj->Set(v8::String::New(name), value);
    
#define SET_PROPERTY_READONLY(obj,name,value) \
    obj->Set(v8::String::New(name), value, v8::PropertyAttribute::ReadOnly);
    
#define SET_METHOD(obj, name, callback) \
    obj->Set(v8::String::New(name), v8::FunctionTemplate::New(callback));

#define SET_INSTANCE_PROPERTY(templ, name, value) \
    SET_PROPERTY(templ->InstanceTemplate(), name, value);
    
#define SET_PROTOTYPE_METHOD(templ, name, callback) \
    SET_METHOD(templ->PrototypeTemplate(), name, callback);
    
#define SET_CLASS_CONSTUCT_HANDLER(templ, callback) \
    templ->SetCallHandler(callback);
    
#define SET_CLASS_NAME(templ, name)\
    templ->SetClassName(v8::String::New(name));
    
    
    // convert number to v8 val
    Local<Value> ToV8Num(double x);
    
    // convert string to v8 val
    Local<Value> ToV8String(const char* x);
    
    // convert bool to v8 val
    Local<Value> ToV8Bool(bool x);
    
    Local<Value> ToV8Int(int x);
    
    // convert v8 string to c string
    const char* ToCString(const String::Utf8Value& string);
    
    // convert v8 string to c string
    const char* ToCString(const Local<String> string);
    
    
}



#endif
