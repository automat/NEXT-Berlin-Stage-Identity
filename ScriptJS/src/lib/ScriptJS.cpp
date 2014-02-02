//
//  ScriptJS.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "ScriptJS.h"
#include "ScriptJS_Prefix.h"

namespace scriptjs {
    // definitions from ScriptJS_Prefix
    
    Local<Value> ToV8Num(double x){
        return Number::New(x);
    }
    
    Local<Value> ToV8String(const char* x){
        return String::NewFromUtf8(Isolate::GetCurrent(), x);
    }
    
    const char* ToCString(const String::Utf8Value& string){
        return *string;
    }
    
    const char* ToCString(const Local<String> string){
        return *String::Utf8Value(string);
    }
    
}