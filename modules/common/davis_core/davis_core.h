/*
==============================================================================

BEGIN_JUCE_MODULE_DECLARATION

   ID:            davis_core
   vendor:        davis
   version:       0.0.1
   name:          Davis Core
   description:
   dependencies:

   website:
   license:       BSD 3-Clause

   END_JUCE_MODULE_DECLARATION

==============================================================================
*/

#pragma once

#if JUCE_MODULE_AVAILABLE_juce_core
#define DAVIS_USING_JUCE 1
#include <juce_core/juce_core.h>
#else
#define DAVIS_USING_JUCE 0
#endif

#if ! DAVIS_USING_JUCE
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 0

#endif //DAVIS_USING_JUCE
/** Namespace reserved for code written for davis projects. */
namespace davis
{

/** Namespace reserved for code that has an unstable API or is otherwise not yet production ready. */
namespace experimental
{
}

} // namespace davis

#include "utils/common/common.h"
#include "utils/common/Identifiers.h"
#include "utils/common/startup.h"
#include "utils/common/utils.h"
#include "utils/common/templates/Factory.h"

