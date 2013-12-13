//
//  main.cpp
//  ObjBinding
//
//  Created by Henryk Wollik on 13/12/13.
//  Copyright (c) 2013 atm. All rights reserved.
//

#include <iostream>
#include <v8.h>
#include "v8Utils.h"

class CppClass {
    int member_a_;
    int member_b_;
public:
   
    CppClass() : member_a_(0), member_b_(1){};
    
    void set_member_a(int a){member_a_ = a;}
    void set_member_b(int b){member_b_ = b;}
    
    
};

int global_cpp_x = 256;
int global_cpp_y = 16;

void get_global_x(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info){
    info.GetReturnValue().Set(v8Utils::v8_num(global_cpp_x));
}

void set_global_x(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info){
    global_cpp_x = value->Int32Value();
}

void get_global_y(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info){
    info.GetReturnValue().Set(v8Utils::v8_num(global_cpp_y));
}

void set_global_y(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info){
    global_cpp_y = value->Int32Value();
}

void logGlobalCppX(const v8::FunctionCallbackInfo<v8::Value>& info){
    std::cout << global_cpp_x << std::endl;
    info.GetReturnValue().SetUndefined();
}

void logGlobalCppY(const v8::FunctionCallbackInfo<v8::Value>& info){
    std::cout << global_cpp_y << std::endl;
    info.GetReturnValue().SetUndefined();
}

/*-------------------------------------------------------------------------------------*/

void globalCppMethod(const v8::FunctionCallbackInfo<v8::Value>& info){
    std::cout << "YAAARGH! CPP!" << std::endl;
    info.GetReturnValue().SetUndefined();
}

/*-------------------------------------------------------------------------------------*/

v8::Handle<v8::ObjectTemplate> getGlobalTemplate(){
    v8::Local<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New();
    
    v8::Local<v8::FunctionTemplate> console_template = v8::FunctionTemplate::New();
    console_template->Set("log", v8::FunctionTemplate::New(v8Utils::log));
    
    v8::Local<v8::ObjectTemplate> g_cpp_template = v8::ObjectTemplate::New();
    g_cpp_template->Set("method", v8::FunctionTemplate::New(globalCppMethod));
    g_cpp_template->Set("logX",   v8::FunctionTemplate::New(logGlobalCppX));
    g_cpp_template->Set("logY",   v8::FunctionTemplate::New(logGlobalCppY));
    g_cpp_template->SetAccessor(v8Utils::v8_str("x"),get_global_x, set_global_x);
    g_cpp_template->SetAccessor(v8Utils::v8_str("y"),get_global_y, set_global_y);
    
    global_template->Set("console",    console_template);
    global_template->Set("global_cpp", g_cpp_template);
    
    return global_template;
}


/*-------------------------------------------------------------------------------------*/

const char* kFilePath = "/Users/automat/Projects/next/tests/V8EmbedTest/ObjBinding/source.js";

int main(int argc, const char * argv[])
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    
    v8::Handle<v8::Context> context = v8::Context::New(isolate,nullptr,getGlobalTemplate());
    v8::Persistent<v8::Context> persistentContext(isolate,context);
    
    v8::Context::Scope contextScope(context);
    std::string fileStr = v8Utils::readFile(kFilePath);
    v8Utils::runScript(fileStr.c_str());
    
    persistentContext.Dispose();
}

