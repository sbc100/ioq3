/*
 * Copyright (c) 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef QUAKE_INSTANCE_H
#define QUAKE_INSTANCE_H

#include <pthread.h>
#include <ppapi/cpp/input_event.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/rect.h>
#include "nacl-mounts/base/MainThreadRunner.h"

class QuakePepperInstance : public pp::Instance {
public:
	explicit QuakePepperInstance(PP_Instance instance);

	virtual ~QuakePepperInstance() {
			if (runner_) delete runner_;
	}

	virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

	void DidChangeView(const pp::Rect& position, const pp::Rect& clip);

	bool HandleInputEvent(const pp::InputEvent& event);

	bool quit() const { return quit_; }

private:
	pthread_t quake_thread_;
	MainThreadRunner *runner_;
	int width_;
	int height_;
	bool quit_;
};

#endif  // QUAKE_INSTANCE_H
