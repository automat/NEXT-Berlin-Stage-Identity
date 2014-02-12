//
//  Module.h
//  ScriptJS
//
//  Created by Henryk Wollik on 03/02/14.
//
//

#ifndef ScriptJS_Module_h
#define ScriptJS_Module_h

#include "ScriptJS_Prefix.h"

#define SCRIPTJS_MODULE_RETURN_UNDEFINED \
    args.GetReturnValue().Set(Undefined());

#define SCRIPTJS_MODULE_RETURN(arg) \
    args.GetReturnValue().Set(arg);

namespace scriptjs {
	//! Interface for C++ to v8 bindings
	class Module {
	public:
		virtual void Initialize(v8::Handle<v8::Object> exports) {};
	};
}



#endif
