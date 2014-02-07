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
    //https://code.google.com/p/v8/source/browse/trunk/samples/lineprocessor.cc
    void log_internal(const Local<Value>& val, int depth = 0);
    void log(const FunctionCallbackInfo<Value>& info);
}

#endif /* defined(__ScriptJS__Logger__) */
