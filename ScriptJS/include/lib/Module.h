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

namespace scriptjs {
    
	//! Interface for C++ to v8 bindings
	class Module {
	public:
		Module() {};
		~Module() {};
		virtual void Initialize(v8::Handle<v8::Object> target) {};
	};

}



#endif
