#pragma once

/* Class VFXPostProcess */
// The idea for post process is to
// copy the framebuffer to a texture
// where it can be manipulated in the code
// or by shaders.

#include <SDL_opengl.h>

#include <spine/spine.h>
#include <spine/extension.h>

#include <vector>

#include "common.h"
#include "model_data.h"
#include "grass_entity.h"
#include "linear_algebra.h"

class VFXPostProcess
{
public:
	void do_post_process();
};

#pragma once
