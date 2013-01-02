// Copyright 2011 The Native Client SDK Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

extern "C" {
#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sys_local.h"
}

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/var.h"

// TODO(sbc): define an interface for accessing the active SDL instance
// rather than peeking into the SDK globals like this.
extern pp::Instance* gNaclPPInstance;

static void PostMessage(void* message, int32_t unused)
{
	printf("XXXXX\n");
	gNaclPPInstance->PostMessage(pp::Var((const char*)message));
}

dialogResult_t Sys_Dialog( dialogType_t type, const char *message, const char *title )
{
	printf("DIALOG: %s: %s\n", title, message);
	pp::Module::Get()->core()->CallOnMainThread(0, pp::CompletionCallback(&PostMessage, (void*)message), PP_OK);
	return DR_OK;
}
