// Copyright 2011 The Native Client SDK Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"
#include "sys_local.h"

#include <sys/mount.h>
#include <unistd.h>

dialogResult_t Sys_Dialog(dialogType_t type, const char *message, const char *title)
{
	printf("DIALOG: %s: %s\n", title, message);
	return DR_OK;
}

void Sys_PlatformInitNacl(void) {
	int rtn = mount(".",  /* source. Use relative URL */
			"/data",  /* target */
			"httpfs",  /* filesystemtype */
			0,  /* mountflags */
			"manifest=nacl_manifest.txt");  /* data */
	Com_Printf("Created url mount: %d\n", rtn);
  chdir("/data");
	Com_Printf("chdir data: %d\n", rtn);

}
