//
//  v8Utlis.h
//  V8EmbedTest
//
//  Created by Henryk Wollik on 13/12/13.
//  Copyright (c) 2013 atm. All rights reserved.
//

#ifndef V8EmbedTest_v8Utlis_h
#define V8EmbedTest_v8Utlis_h

#include <v8.h>
#include <iostream>
#include <fstream>

namespace v8Utils {
    
    v8::Local<v8::Number> v8_num(double x){
        return v8::Number::New(x);
    }
    
    v8::Local<v8::String> v8_str(const char* x){
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x);
    }
    
    
    
    
    const char* toCString(const v8::String::Utf8Value& string){
        return *string;
    }
    
    const char* toCString(const v8::Local<v8::String> string){
        return *v8::String::Utf8Value(string);
    }
    
    void log_internal(const v8::Local<v8::Value>& val, int depth = 0){
        static const int intend_space = 4;
        std::string intend(intend_space * (depth + 1),' ');
        
        int j;
        
        if (val->IsObject() && v8::Local<v8::Object>::Cast(val)->GetPropertyNames()->Length() != 0) {
            const v8::Local<v8::Object>& obj = v8::Local<v8::Object>::Cast(val);
            std::cout << toCString(obj->GetConstructorName()) << std::endl;
            
            int internal_field_count = obj->InternalFieldCount();
            if(internal_field_count != 0){
                j = -1;
                while (++j < internal_field_count) {
                    std::cout << intend << "internal field " << j << " : " <<
                    toCString(obj->GetInternalField(j)->ToString()) << std::endl;
                }
            }
            
            const v8::Handle<v8::Array>& property_names = obj->GetPropertyNames();
            if (!property_names.IsEmpty()) {    // if properies, print
                j = -1; while (++j < property_names->Length()) {
                    std::cout << intend << toCString(property_names->Get(j)->ToString()) << ": ";
                    log_internal(obj->Get(property_names->Get(j)), depth+1);
                }
            }
        }
        
        if (val->IsNull()) {
            std::cout << "Null";
        }
        
        if (val->IsNumberObject() ||
            val->IsNumber() ||
            val->IsFunction() ||
            val->IsString()) {
            std::cout << toCString(val->ToString());
        }
        
        std::cout << std::endl;
    }
    
    void log(const v8::FunctionCallbackInfo<v8::Value>& info){
        if (info.Length() == 0) return;
        
        int i = -1;
        while (++i < info.Length()) {
            log_internal(info[i]);
        }
        
        info.GetReturnValue().SetUndefined();
    }
    
    
    //TODO add exception handling
    bool runScript(const char* source){
        v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
            v8::Handle<v8::Script> script = v8::Script::Compile(v8_str(source));
            if (script.IsEmpty()) return false;
        
            v8::Handle<v8::Value> result = script->Run();
            return result.IsEmpty() || result->IsUndefined() ? false : true;
    }
    
    
    std::string readFile(const char* filepath){
        FILE* file = fopen(filepath, "rb");
        if (file == NULL) {
            perror("Error");
        }
        
        fseek(file, 0, SEEK_END);
        int size = (int)ftell(file);
        rewind(file);
        
        char* chars = new char[size + 1];
        chars[size] = '\0';
        int i = -1;
        while (++i < size) {
            i += (int)fread(&chars[i], 1, size - i, file);;
        }
        fclose(file);
        
        std::string contents(chars, size);
        delete chars;
        
        return contents;
    }
    
    
    
    
}




#endif
