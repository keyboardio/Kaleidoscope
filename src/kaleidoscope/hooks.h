#pragma once

#include <Arduino.h>

namespace kaleidoscope {
union Key;
}

#include "plugin.h"
#include "event_handlers.h"

namespace kaleidoscope {

// The reason why the hook routing entry point functions live within the hooks
// namespace and not directly within a namespace is, that we want to prevent
// accidental naming conflicts.

namespace hooks {
#define DEFINE_WEAK_HOOK_FUNCTION(                                             \
    HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)    __NL__ \
                                                                        __NL__ \
   EventHandlerResult HOOK_NAME SIGNATURE;

_FOR_EACH_EVENT_HANDLER(DEFINE_WEAK_HOOK_FUNCTION)

#undef DEFINE_WEAK_HOOK_FUNCTION
}

}
