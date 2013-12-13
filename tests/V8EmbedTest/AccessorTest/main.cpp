//
//  main.cpp
//  AccessorTest
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

void helloCpp(const v8::FunctionCallbackInfo<v8::Value>& info){
    if (info.Length() == 0)return;
    
    int i = -1;
    while (++i < info.Length()) {
        const v8::Local<v8::Value>& val = info[i];
        
        if(val->IsNumber()){
            std::cout << "Object at " << i << " is Number." << std::endl;
        }
        
        if(val->IsBoolean()) {
            std::cout << "Object at " << i << " is Boolean." << std::endl;
        }
        
        if(val->IsString()) {
            std::cout << "Object at " << i << " is String." << std::endl;
        }
    }
    
    info.GetReturnValue().SetUndefined();
}



int main(int argc, const char * argv[])
{
    Point p(10,20);
    std::cout << p << std::endl;
    
    //Create initial v8 isolated instance
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    
    //Create global function accesor
    v8::Handle<v8::FunctionTemplate> funcTemp   = v8::FunctionTemplate::New(helloCpp);
    v8::Handle<v8::ObjectTemplate>   globalTemp = v8::ObjectTemplate::New();
    globalTemp->Set(v8::String::New("helloCpp"), funcTemp);
   
    v8::Handle<v8::Context> context = v8::Context::New(isolate,nullptr,globalTemp);
    v8::Persistent<v8::Context> persistentContext(isolate,context);
    
    v8::Context::Scope contextScope(context);
    v8::Handle<v8::Script> script = v8::Script::Compile(v8::String::New("helloCpp(1,'alloa',true)"));
    script->Run();
    
    persistentContext.Dispose();

    return 0;
}

