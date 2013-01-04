/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "quake_instance.h"
#include <assert.h>
#include <vector>
#include <SDL.h>
#include <SDL_nacl.h>
#include <SDL_video.h>
#include "ppapi/cpp/file_system.h"
#include "nacl-mounts/pepper/PepperMount.h"
#include "nacl-mounts/base/UrlLoaderJob.h"
#include "sys/mount.h"


extern "C"
{
// For Com_Printf
#include "../../qcommon/q_shared.h"
#include "../../qcommon/qcommon.h"
int quake_main(int argc, const char* argv[]);
}

bool sync(MainThreadRunner* runner, const char* filename)
{
	struct stat buf;
	if (stat(filename, &buf) != -1)
	{
		return true;
	}

	Com_Printf("downloading file: %s\n", filename);
	UrlLoaderJob::Result result;
	UrlLoaderJob* job = new UrlLoaderJob;
	job->set_url(filename);
	std::vector<char> data;
	job->set_dst(&data);
	job->set_result_dst(&result);
	runner->RunJob(job);
	if (result.status_code != 200)
	{
		Com_Printf("error fetching URL: %s: %d\n", filename, result.status_code);
		return false;
	}
	int fd = open(filename, O_CREAT | O_WRONLY);
	if (fd == -1)
	{
		Com_Printf("error (%s) opening file for write: %s\n", strerror(errno), filename);
		return false;
	}
	uint rtn = write(fd, &data[0], data.size());
	if (rtn != data.size())
	{
		Com_Printf("error writing %d bytes: %s\n", data.size(), strerror(errno));
		return false;
	}
	if (rtn != data.size())
	{
		Com_Printf("only wrote writing %d/%d bytes\n", rtn , data.size());
		return false;
	}

	close(fd);
	return true;
}

static void *quake_run(void *arg) {
	QuakePepperInstance* instance = (QuakePepperInstance*)arg;

	// MainThreadRunner for use by the quake thread
	MainThreadRunner* runner =  new MainThreadRunner(instance);

	// Mount local filesystem storage
	pp::FileSystem* fs = new pp::FileSystem(instance, PP_FILESYSTEMTYPE_LOCALPERSISTENT);
	Com_Printf("Created FS: %p\n", fs);
	PepperMount* mnt = new PepperMount(runner, fs, 0x100000);
	mnt->SetDirectoryReader(instance);
	mnt->SetPathPrefix("/");
	int ret = mount(0, "/baseq3", 0, 0, mnt);  
	if (ret != 0)
	{
		Com_Printf("mount failed\n");
		return NULL;
	}
	Com_Printf("Mounted PepperFS: %p\n", fs);

	for (int i = 0; i <= 8; i++)
	{
		char filename[128];
		Com_sprintf(filename, 128, "baseq3/pak%d.pk3", i);
		if (!sync(runner, filename))
			return NULL;
	}


	if (!sync(runner, "baseq3/q3config.cfg"))
		return NULL;

	// Run game
	static char const * argv[] = {"quake3", NULL};
	Com_Printf("calling quake_main\n");
	int rtn = quake_main(1, argv);
	Com_Printf("quake_main done: %d\n", rtn);

	return 0;
}

QuakePepperInstance::QuakePepperInstance(PP_Instance instance)
	: pp::Instance(instance),
		runner_(NULL),
		width_(0),
		height_(0),
		quit_(false) {
	RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
	RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);
}

int QuakePepperInstance::ReadDirectory(const std::string& path, std::set<std::string>* entries, const pp::CompletionCallback& cc)
{
	Com_Printf("ReadDirectory: %s\n", path.c_str());
	entries->insert("pak0.pk3");
	entries->insert("pak1.pk3");
	entries->insert("pak2.pk3");
	entries->insert("pak3.pk3");
	entries->insert("pak4.pk3");
	entries->insert("pak5.pk3");
	entries->insert("pak6.pk3");
	entries->insert("pak7.pk3");
	entries->insert("pak8.pk3");
	entries->insert("q3config.cfg");
	((pp::CompletionCallback)cc).Run(PP_OK);
	return 0;
}


bool QuakePepperInstance::Init(uint32_t argc, const char* argn[], const char* argv[]) {
	return true;
}

void QuakePepperInstance::DidChangeView(const pp::Rect& position, const pp::Rect& clip) {
	if (width_ && height_)
		return;

	if (position.size().width() == width_ &&
			position.size().height() == height_)
		return;  // Size didn't change, no need to update anything.

	width_ = position.size().width();
	height_ = position.size().height();

	SDL_NACL_SetInstance(pp_instance(), width_, height_);

	int lval = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	assert(lval >= 0);
	if (lval < 0)
		return;

	pthread_create(&quake_thread_, NULL, quake_run, this);
}

bool QuakePepperInstance::HandleInputEvent(const pp::InputEvent& event) {
	SDL_NACL_PushEvent(event);
	return true;
}
