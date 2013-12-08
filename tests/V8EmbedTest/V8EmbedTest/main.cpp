//
//  main.cpp
//  V8EmbedTest
//
//  Created by Henryk Wollik on 08/12/13.
//  Copyright (c) 2013 atm. All rights reserved.
//

#include <iostream>
#include <v8.h>




int main(int argc, const char * argv[])
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    
    v8::HandleScope handle_scope(isolate);
    
    v8::Handle<v8::Context> context = v8::Context::New(isolate);
    
    v8::Persistent<v8::Context> persistent_context(isolate,context);
    
    v8::Context::Scope contex_scope(context);
    
    v8::Handle<v8::String> source = v8::String::New("'Hello ' + 'Darth Vader'");
    
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    
    v8::Handle<v8::Value> result = script->Run();
    
    persistent_context.Dispose();
    
    v8::String::AsciiValue ascii(result);
    
    printf("%s\n", *ascii);
    
     return 0;
}

