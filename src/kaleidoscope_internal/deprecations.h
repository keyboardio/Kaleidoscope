#pragma once

#define DEPRECATED(tag)                                                 \
  __attribute__((deprecated(_DEPRECATE(_DEPRECATED_MESSAGE_ ## tag))))

#define _DEPRECATE(message) "\n"                                               \
  "------------------------------------------------------------------------\n" \
  message                                                                      \
  "\n"                                                                         \
  "------------------------------------------------------------------------\n" \

/* Messages */

#define _DEPRECATED_MESSAGE_KEYMAP_SIZE                                    \
  "Kaleidoscope.setup() does not require KEYMAP_SIZE anymore. It can be\n" \
  "safely removed."

#define _DEPRECATED_MESSAGE_USE                                                    \
   "Your sketch uses Kaleidoscope.use(), an old-style API to initialize\n"         \
   "plugins. To fix this, you need to modify your .ino sketch file, and\n"         \
   "replacing the text \"Kaleidoscope.use\" with \"KALEIDOSCOPE_INIT_PLUGINS\",\n" \
   "then remove the & from all of the plugins inside it, and finally, move\n"      \
   "it outside of \"setup()\":\n"                                                  \
   "\n"                                                                            \
   "If your current sketch looks like this: \n"                                    \
   "  void setup() {\n"                                                            \
   "    Kaleidoscope.use(&Plugin1, &Plugin2);\n"                                   \
   "    Kaleidoscope.setup();\n"                                                   \
   "  }\n"                                                                         \
   "\n"                                                                            \
   "You should change it so that it looks like this:\n"                            \
   "  KALEIDOSCOPE_INIT_PLUGINS(Plugin1, Plugin2);\n"                              \
   "  void setup() {\n"                                                            \
   "    Kaleidoscope.setup();\n"                                                   \
   "  }\n"                                                                         \
   "\n"                                                                            \
   "If this error doesn't make sense to you or you have any trouble, please\n"     \
   "send a copy of your .ino sketch file to help@keyboard.io and we can\n"         \
   "help fix it."

#define _DEPRECATED_MESSAGE_EVENT_HANDLER_HOOK                             \
  "The legacy plugin API based on hook registration is deprecated.\n"      \
  "\n"                                                                     \
  "Consider upgrading your plugins, or implementing the new interface\n"   \
  "described by `kaleidoscope::Plugin`. In particular, instead of using\n" \
  "`Kaleidoscope.useEventHandlerHook`, implement the\n"                    \
  "`.onKeyswitchEvent()` method instead."


#define _DEPRECATED_MESSAGE_LOOP_HOOK                                      \
  "The legacy plugin API based on hook registration is deprecated.\n"      \
  "\n"                                                                     \
  "Consider upgrading your plugins, or implementing the new interface\n"   \
  "described by `kaleidoscope::Plugin`. In particular, instead of using\n" \
  "`Kaleidoscope.useLoopHook`, implement `.beforeEachCycle`,\n"            \
  "`.beforeReportingState()`, or `.afterEachCycle()` instead."

#define _DEPRECATED_MESSAGE_USE_INSTEAD(new_method) \
  "Please use `" new_method "` instead."

#define _DEPRECATED_MESSAGE_KALEIDOSCOPEPLUGIN                                 \
  "The `KaleidoscopePlugin` class is deprecated. Please derive plugins from\n" \
  "`kaleidoscope::Plugin` instead."
