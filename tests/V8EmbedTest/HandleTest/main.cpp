//
//  main.cpp
//  HandleTest
//
//  Created by Henryk Wollik on 12/12/13.
//  Copyright (c) 2013 atm. All rights reserved.
//

#include <iostream>
#include <v8.h>


v8::Handle<v8::Array> NewPointArray(int x, int y, int z){
    // get current v8 instance
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    // create handle scope
    v8::HandleScope handle_scope(isolate);
    
    // create handle to v8 array
    v8::Handle<v8::Array> array = v8::Array::New(3);
    
    //if array handle cant be created return
    if (array.IsEmpty()) {
        return v8::Handle<v8::Array>();
    }
    
    array->Set(0, v8::Integer::New(x));
    array->Set(1, v8::Integer::New(y));
    array->Set(2, v8::Integer::New(z));
    
    return handle_scope.Close(array);
}

const char* toString(const v8::String::Utf8Value& value){
    return *value;
}

const char* toString(const v8::Value& value){
    return *v8::String::Utf8Value(value.ToString());
}

int main(int argc, const char * argv[])
{
    //Create initial v8 isolated instance
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    
    v8::Handle<v8::Context> context = v8::Context::New(isolate);
    v8::Persistent<v8::Context> persistentContext(isolate,context);
    
    v8::Context::Scope contextScope(context);
    
    v8::Handle<v8::Array> pointArr = NewPointArray(0, 1, 10);
    
    std::cout << pointArr->Length() << std::endl;
    
    pointArr->Set(0, v8::Integer::New(123));
    
    std::cout << toString(v8::String::Utf8Value(pointArr)) << std::endl;
    //std::cout << toString(v8::Value::Cast(v8::Number::New(10)) << std::endl;
    
    persistentContext.Dispose();
    
    //v8::String::AsciiValue ascii(result);
   // printf("%s\n", *ascii);
    
    return 0;
}



