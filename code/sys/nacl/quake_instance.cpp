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

extern "C"
{
// For Com_Printf
#include "../../qcommon/q_shared.h"
#include "../../qcommon/qcommon.h"
int quake_main(int argc, const char* argv[]);
}

static void *quake_run(void *arg) {
	static char const * argv[] = {"quake3", NULL};
	Com_Printf("calling quake_main\n");
	int rtn = quake_main(1, argv);
	Com_Printf("quake_main done: %d\n", rtn);
	return 0;
}

QuakePepperInstance::QuakePepperInstance(PP_Instance instance)
	: pp::Instance(instance),
		quit_(false),
		width_(0),
		height_(0),
		runner_(NULL) {
	RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
	RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);
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

	pthread_create(&quake_thread_, NULL, quake_run, NULL);
}

bool QuakePepperInstance::HandleInputEvent(const pp::InputEvent& event) {
	SDL_NACL_PushEvent(event);
	return true;
}
