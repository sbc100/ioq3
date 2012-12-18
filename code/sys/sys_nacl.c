// Copyright 2011 The Native Client SDK Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sys_local.h"

dialogResult_t Sys_Dialog( dialogType_t type, const char *message, const char *title )
{
	printf("DIALOG: %s: %s", title, message);
	return DR_OK;
}
