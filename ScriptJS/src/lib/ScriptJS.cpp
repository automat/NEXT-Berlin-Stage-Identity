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
    
    v8::Local<v8::Value> ToV8Int(int x){
        return v8::Integer::New(x);
    }
    
    v8::Local<v8::Value> ToV8String(const char* x){
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x);
    }
    
    v8::Local<v8::Value> ToV8Bool(bool x){
        return v8::Boolean::New(x);
    }
    
    const char* ToCString(const v8::String::Utf8Value& string){
        return *string;
    }
    
    const char* ToCString(const v8::Local<v8::String> string){
        return *v8::String::Utf8Value(string);
    }
    
    void ReportException(TryCatch* tryCatch) {
        String::Utf8Value exception(tryCatch->Exception());
        const char* exceptionString = ToCString(exception);
        Handle<Message> message = tryCatch->Message();
        
        if (message.IsEmpty()) {
            // V8 didn't provide any extra information about this error; just
            // print the exception.
            printf("%s\n", exceptionString);
        } else {
            // Print (filename):(line number): (message).
            String::Utf8Value filename(message->GetScriptResourceName());
            const char* filenameString = ToCString(filename);
            int linenum = message->GetLineNumber();
            printf("%s:%i: %s\n", filenameString, linenum, exceptionString);
            
            // Print line of source code.
            String::Utf8Value sourceline(message->GetSourceLine());
            const char* sourcelineString = ToCString(sourceline);
            printf("%s\n", sourcelineString);
            
            // Print wavy underline (GetUnderline is deprecated).
            int start = message->GetStartColumn();
            for (int i = 0; i < start; i++) {
                printf(" ");
            }
            
            int end = message->GetEndColumn();
            for (int i = start; i < end; i++) {
                printf("^");
            }
            printf("\n");
        }
    }
    
    Handle<Value> ThrowError(const char* msg){
        return ThrowException(Exception::Error(String::New(msg)));
    }
    
    Handle<Value> ThrowTypeError(const char* msg){
        return ThrowException(Exception::TypeError(String::New(msg)));
    }
    
}