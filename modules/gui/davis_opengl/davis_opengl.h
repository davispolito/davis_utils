/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            davis_opengl
   vendor:        davis
   version:       0.0.1
   name:          Davis OPENGL
   description:
   dependencies: davis_core, davis_look_and_feel

   website:
   license:       BSD 3-Clause

   END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

/** Config: DAVIS_ENABLE_OPEN_GL_CONTEXT
            If selected an juce OpenGLCOntext is attached. Not a big difference on OSX, but vital on Windows.
  */
#ifndef DAVIS_ENABLE_OPEN_GL_CONTEXT
#define DAVIS_ENABLE_OPEN_GL_CONTEXT 1
#endif

// JUCE includes
#include <juce_core/juce_core.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>


#if JUCE_MODULE_AVAILABLE_juce_opengl && DAVIS_ENABLE_OPEN_GL_CONTEXT
#define DAVIS_OPENGL_IS_AVAILABLE 1
#include <juce_opengl/juce_opengl.h>
#else
#define DAVIS_OPENGL_IS_AVAILABLE 0
#endif

#include "open_gl/open_gl_component.h"
#include "open_gl/open_gl_background.h"
#include "open_gl/open_gl_image.h"
#include "open_gl/open_gl_image_component.h"
#include "open_gl/open_gl_multi_image.h"
#include "open_gl/open_gl_multi_quad.h"
#include "open_gl/overlay.h"
#include "open_gl/shaders.h"