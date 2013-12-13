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


v8::Handle<v8::ObjectTemplate> getGlobalTemplate(){
    v8::Local<v8::ObjectTemplate> global_template = v8::ObjectTemplate::New();
    
    v8::Local<v8::FunctionTemplate> console_template = v8::FunctionTemplate::New();
    console_template->Set("log", v8::FunctionTemplate::New(v8Utils::log));
    
    global_template->Set("console",console_template);
    
    
    
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

