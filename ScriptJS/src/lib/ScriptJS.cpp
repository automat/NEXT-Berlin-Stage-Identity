//
//  ScriptJS.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "ScriptJS.h"
#include <v8.h>

namespace scriptjs {
    // definitions from ScriptJS_Prefix
    
    v8::Local<v8::Value> ToV8Num(double x){
        return v8::Number::New(x);
    }
    
    v8::Local<v8::Value> ToV8String(const char* x){
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x);
    }
    
    const char* ToCString(const v8::String::Utf8Value& string){
        return *string;
    }
    
    const char* ToCString(const v8::Local<v8::String> string){
        return *v8::String::Utf8Value(string);
    }
    
}