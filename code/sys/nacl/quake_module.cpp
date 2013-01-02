// Copyright 2011 The Native Client SDK Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include <ppapi/cpp/module.h>
#include <stdio.h>
#include "quake_instance.h"

extern "C"
{
// For Com_Printf
#include "../../qcommon/q_shared.h"
#include "../../qcommon/qcommon.h"
}

namespace quake_pepper
{

// The Module class.	The browser calls the CreateInstance() method to create
// an instance of you NaCl module on the web page.	The browser creates a new
// instance for each <embed> tag with type="application/x-ppapi-nacl-srpc".
class QuakePepperModule : public pp::Module
{
public:
	QuakePepperModule() : pp::Module() {}
	virtual ~QuakePepperModule() {}

	virtual pp::Instance* CreateInstance(PP_Instance instance)
	{
		Com_Printf("quake_module: CreateInstance\n");
		return new QuakePepperInstance(instance);
	}
};

}  // namespace quake_pepper

// Factory function called by the browser when the module is first loaded.
// The browser keeps a singleton of this module.	It calls the
// CreateInstance() method on the object you return to make instances.	There
// is one instance per <embed> tag on the page.  This is the main binding
// point for your NaCl module with the browser.
namespace pp
{

Module* CreateModule()
{
	Com_Printf("quake_module: CreateModule\n");
	return new quake_pepper::QuakePepperModule();
}

}  // namespace pp
