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
    using namespace std;
    void log_internal(const Local<Value>& val, int depth){
        static const int intend_space = 4;
        std::string intend(intend_space * (depth + 1),' ');
        int j;
        
        if (val->IsObject() && Local<Object>::Cast(val)->GetPropertyNames()->Length() != 0) {
            const Local<Object>& obj = Local<Object>::Cast(val);
            cout << ToCString(obj->GetConstructorName()) << endl;
        
            // check if object is bound to native object
            int internalFieldCount = obj->InternalFieldCount();
            if(internalFieldCount != 0){
                j = -1;
                while (++j < internalFieldCount) {
                    cout << intend << "internal field " << j << " : " <<
                    ToCString(obj->GetInternalField(j)->ToString()) << endl;
                }
            }
            
            // iterate over properties
            const Handle<Array>& propertyNames = obj->GetPropertyNames();
            if (!propertyNames.IsEmpty()){
                j = -1;
                while (++j < propertyNames->Length()) {
                    cout << intend << ToCString(propertyNames->Get(j)->ToString()) << ": ";
                    log_internal(obj->Get(propertyNames->Get(j)), depth+1);
                }
            }
        }
        
        if (val->IsUndefined()) {
            cout << "Undefined";
        }
        
        if (val->IsNull()) {
            cout << "Null";
        }
        
        if (val->IsNumberObject() ||
            val->IsNumber() ||
            val->IsFunction() ||
            val->IsString() ||
            val->IsBoolean() ||
            val->IsBooleanObject()) {
            cout << ToCString(val->ToString());
        }
        
        cout << std::endl;
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