//
//  Logger.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "ScriptJS_Prefix.h"
#include "Logger.h"

namespace scriptjs {
    void log_internal(const Local<Value>& val, int depth){
        static const int intend_space = 4;
        std::string intend(intend_space * (depth + 1),' ');
        int j;
        
        if (val->IsObject() && v8::Local<v8::Object>::Cast(val)->GetPropertyNames()->Length() != 0) {
            const v8::Local<v8::Object>& obj = v8::Local<v8::Object>::Cast(val);
            std::cout << ToCString(obj->GetConstructorName()) << std::endl;
        
            // check if object is bound to native object
            int internal_field_count = obj->InternalFieldCount();
            if(internal_field_count != 0){
                j = -1;
                while (++j < internal_field_count) {
                    std::cout << intend << "internal field " << j << " : " <<
                    ToCString(obj->GetInternalField(j)->ToString()) << std::endl;
                }
            }
            
            // iterate over properties
            const v8::Handle<v8::Array>& property_names = obj->GetPropertyNames();
            if (!property_names.IsEmpty()) {    // if properies, print
                j = -1;
                while (++j < property_names->Length()) {
                    std::cout << intend << ToCString(property_names->Get(j)->ToString()) << ": ";
                    log_internal(obj->Get(property_names->Get(j)), depth+1);
                }
            }
        }
        
        if (val->IsUndefined()) {
            std::cout << "Undefined";
        }
        
        if (val->IsNull()) {
            std::cout << "Null";
        }
        
        if (val->IsNumberObject() ||
            val->IsNumber() ||
            val->IsFunction() ||
            val->IsString() ||
            val->IsBoolean() ||
            val->IsBooleanObject()) {
            std::cout << ToCString(val->ToString());
        }
        
        
        std::cout << std::endl;
    }
    
    void log(const FunctionCallbackInfo<Value>& info){
        if (info.Length() == 0) {
            return;
        }
        
        int i = -1;
        while (++i < info.Length()) {
            log_internal(info[i]);
        }
        
        info.GetReturnValue().SetUndefined();
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