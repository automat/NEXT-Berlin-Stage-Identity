//
//  WrappedObj.h
//  ScriptJS
//
//  Created by Henryk Wollik on 03/02/14.
//
//

#ifndef ScriptJS_WrappedObj_h
#define ScriptJS_WrappedObj_h

#include "ScriptJS.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"

// Follows: https://github.com/joyent/node/blob/master/doc/api/addons.markdown

class ModuleClassCpp : public scriptjs::Module {
    void Initialize(Handle<Object> exports);
};

#endif
