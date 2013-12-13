//
//  main.cpp
//  CppObjWrapTest
//
//  Created by Henryk Wollik on 12/12/13.
//  Copyright (c) 2013 atm. All rights reserved.
//

#include <iostream>
#include <v8.h>

struct Point{
    float x,y;
    
    Point(int x, int y) : x(x),y(y){}
    
    friend std::ostream& operator<<( std::ostream& lhs, const Point& rhs )
    {
        lhs << "[" << rhs.x << "," << rhs.y << "]";
        return lhs;
    }
};

v8::Local<v8::Value> v8_num(double x){
    return v8::Number::New(x);
}

v8::Local<v8::Value> v8_str(const char* x){
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), x);
}


const char* toCString(const v8::String::Utf8Value& string){
    return *string;
}

const char* toCString(const v8::Local<v8::String> string){
    return *v8::String::Utf8Value(string);
}

/*-------------------------------------------------------------------------------------*/


void v8log_internal(const v8::Local<v8::Value>& val, int depth = 0){
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
                v8log_internal(obj->Get(property_names->Get(j)), depth+1);
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

void v8log(const v8::FunctionCallbackInfo<v8::Value>& info){
    if (info.Length() == 0) return;
    
    int i = -1;
    while (++i < info.Length()) {
        v8log_internal(info[i]);
    }
    
    info.GetReturnValue().SetUndefined();
}

/*-------------------------------------------------------------------------------------*/

void class_t_construct_handler(const v8::FunctionCallbackInfo<v8::Value>& info){
    const v8::Local<v8::Object>& objThis = info.This();
    
    switch (info.Length()) {
        case 1:
            objThis->Set(v8_str("member_a"), info[0]);
            objThis->Set(v8_str("member_b"), info[0]);
            break;
        case 2:
            objThis->Set(v8_str("member_a"), info[0]);
            objThis->Set(v8_str("member_b"), info[1]);
            break;
        default:
            break;
    }
    
    info.GetReturnValue().Set(info.IsConstructCall());
}

void class_t_proto_func_do(const v8::FunctionCallbackInfo<v8::Value>& info){
    const v8::Local<v8::Object>& objThis = info.This();
    v8log_internal(objThis);
    info.GetReturnValue().SetUndefined();
}

/*-------------------------------------------------------------------------------------*/

v8::Handle<v8::ObjectTemplate> getGlobalTemplate(){
    v8::Local<v8::FunctionTemplate> class_t_template = v8::FunctionTemplate::New();
    class_t_template->SetCallHandler(class_t_construct_handler);
    
    v8::Local<v8::ObjectTemplate>   class_t_instance = class_t_template->InstanceTemplate();
    class_t_instance->Set("member_a", v8::Null());
    class_t_instance->Set("member_b", v8::Null());
    class_t_instance->SetInternalFieldCount(1);
    
    v8::Local<v8::Template> class_t_prototype = class_t_template->PrototypeTemplate();
    class_t_prototype->Set("do", v8::FunctionTemplate::New(class_t_proto_func_do));
    
    v8::Local<v8::FunctionTemplate> console_template = v8::FunctionTemplate::New();
    console_template->Set("log", v8::FunctionTemplate::New(v8log));

    v8::Handle<v8::ObjectTemplate> global_templ = v8::ObjectTemplate::New();
    
    global_templ->Set("Class_T", class_t_template);
    global_templ->Set("console", console_template);

    return global_templ;
}

/*-------------------------------------------------------------------------------------*/

int main(int argc, const char * argv[]){
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    
    v8::Handle<v8::Context> context = v8::Context::New(isolate,nullptr,getGlobalTemplate());
    v8::Persistent<v8::Context> persistentContext(isolate,context);
    
    char source[] = "var t0 = new Class_T(0,100), t1 = new Class_T(t0,600); t0.do(); console.log(t0)";
    
    v8::Context::Scope contextScope(context);
    v8::Handle<v8::Script> script = v8::Script::Compile(v8::String::New(source));
    script->Run();
 
    persistentContext.Dispose();
}