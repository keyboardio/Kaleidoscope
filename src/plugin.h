#pragma once

#include "kaleidoscope/event_handler_result.h"
#include "kaleidoscope_internal/event_dispatch.h"
#include "event_handlers.h"

#if KALEIDOSCOPE_ENABLE_V1_PLUGIN_API
#error The V1 plugin API has been removed, please see UPGRADING.md.
#endif

namespace kaleidoscope {

class Plugin {

 public:
  // Please see "event_handlers.h" for a list of supported event handlers and
  // their documentation!

#define DEFINE_AND_IMPLEMENT_EVENT_HANDLER_METHOD(                               \
      HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)    __NL__ \
                                                                          __NL__ \
    EventHandlerResult HOOK_NAME SIGNATURE {                              __NL__ \
      return EventHandlerResult::OK;                                      __NL__ \
    }

  _FOR_EACH_EVENT_HANDLER(DEFINE_AND_IMPLEMENT_EVENT_HANDLER_METHOD)

#undef DEFINE_AND_IMPLEMENT_EVENT_HANDLER_METHOD
};

} // namespace kaleidoscope
