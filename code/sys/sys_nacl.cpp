// Copyright 2011 The Native Client SDK Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

extern "C" {
#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sys_local.h"
}

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"

// TODO(sbc): define an interface for accessing the active SDL instance
// rather than peeking into the SDK globals like this.
extern pp::Instance* gNaclPPInstance;

dialogResult_t Sys_Dialog( dialogType_t type, const char *message, const char *title )
{
	printf("DIALOG: %s: %s", title, message);
	gNaclPPInstance->PostMessage(pp::Var(message));
	return DR_OK;
}
