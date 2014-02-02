//
//  ScriptContext.h
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#ifndef __ScriptJS__ScriptContext__
#define __ScriptJS__ScriptContext__

#include <iostream>
#include "ScriptJS_Prefix.h"



namespace scriptjs {
    class ScriptContext{
        Persistent<Context> mContext; // persistent execution context
        
        Handle<ObjectTemplate> getGlobalTemplate();
        
    public:
        ScriptContext();
        ~ScriptContext();
        
        bool execute(const std::string& sourceJsOrFile);
        
        
       // Handle<Object> newInstance(Handle<Object> contextLocal, Handle<String> name)
        
        
        
    };
}

#endif /* defined(__ScriptJS__ScriptContext__) */
