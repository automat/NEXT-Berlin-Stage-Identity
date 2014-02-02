//
//  Logger.cpp
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#include "Logger.h"
#include "ScriptJS_Prefix.h"

namespace scriptjs {
    void log_internal(const Local<Value>& val, int depth){
        static const int intend_space = 4;
        std::string intend(intend_space * (depth + 1),' ');
        
        int j;
        
        if (val->IsObject() && v8::Local<v8::Object>::Cast(val)->GetPropertyNames()->Length() != 0) {
            const v8::Local<v8::Object>& obj = v8::Local<v8::Object>::Cast(val);
            std::cout << ToCString(obj->GetConstructorName()) << std::endl;
            
            int internal_field_count = obj->InternalFieldCount();
            if(internal_field_count != 0){
                j = -1;
                while (++j < internal_field_count) {
                    std::cout << intend << "internal field " << j << " : " <<
                    ToCString(obj->GetInternalField(j)->ToString()) << std::endl;
                }
            }
            
            const v8::Handle<v8::Array>& property_names = obj->GetPropertyNames();
            if (!property_names.IsEmpty()) {    // if properies, print
                j = -1; while (++j < property_names->Length()) {
                    std::cout << intend << ToCString(property_names->Get(j)->ToString()) << ": ";
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
}