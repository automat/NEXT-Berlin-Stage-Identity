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
    // convert number to v8 val
    Local<Value> ToV8Num(double x);
    
    // convert string to v8 val
    Local<Value> ToV8String(const char* x);
    
    // convert v8 string to c string
    const char* ToCString(const String::Utf8Value& string);
    
    // convert v8 string to c string
    const char* ToCString(const Local<String> string);
    
    
}



#endif
