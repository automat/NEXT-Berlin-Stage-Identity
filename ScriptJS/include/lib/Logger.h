//
//  Logger.h
//  ScriptJS
//
//  Created by Henryk Wollik on 02/02/14.
//
//

#ifndef __ScriptJS__Logger__
#define __ScriptJS__Logger__

#include <iostream>
#include "ScriptJS_Prefix.h"

namespace scriptjs {
    void log_internal(const Local<Value>& val, int depth = 0);
    void log(const FunctionCallbackInfo<Value>& info);
    
    //from plask
    void ReportException(TryCatch* tryCatch);
    Handle<Value> ThrowError(const char* msg);
    Handle<Value> ThrowTypeError(const char* msg);
    
    
    
}

#endif /* defined(__ScriptJS__Logger__) */
