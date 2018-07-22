#pragma once

#define _INLINE_STORAGE_HANDLER_FOR_PLUGIN(PLUGIN)                                       __NL__ \
  EventHandler__::call(PLUGIN, current_offset, hook_args...);                            __NL__ \
  current_offset += PLUGIN.storageSize();

#define _INLINE_STORAGE_OFFSET_COUNTER(PLUGIN)                                           __NL__ \
  if ((uint16_t)&PLUGIN == (uint16_t)&plugin)                                            __NL__ \
    return current_offset;                                                               __NL__ \
  current_offset += PLUGIN.storageSize();

#define __KALEIDOSCOPE_INIT_STORAGE_PLUGINS(...)                                         __NL__ \
  /* Iterate through plugins, calling onStorageReport for each of them. */               __NL__ \
  template<typename EventHandler__, typename... Args__ >                                 __NL__ \
  static kaleidoscope::EventHandlerResult                                                __NL__ \
  apply_storage(Args__&&... hook_args) {                                                 __NL__ \
    uint16_t current_offset = 0;                                                         __NL__ \
    MAP(_INLINE_STORAGE_HANDLER_FOR_PLUGIN, __VA_ARGS__)                                 __NL__ \
    return kaleidoscope::EventHandlerResult::OK;                                         __NL__ \
  }                                                                                      __NL__ \
                                                                                         __NL__ \
  template<typename EventHandler__, typename Plugin__>                                   __NL__ \
  static uint16_t apply_offsetCount(Plugin__ &plugin) {                                  __NL__ \
    uint16_t current_offset = 0;                                                         __NL__ \
    MAP(_INLINE_STORAGE_OFFSET_COUNTER, __VA_ARGS__)                                     __NL__ \
    return current_offset;                                                               __NL__ \
  }

#define _KALEIDOSCOPE_INIT_STORAGE_HANDLERS()                                            __NL__ \
  namespace kaleidoscope_internal {                                                      __NL__ \
  struct EventHandler_onStorageReport {                                                  __NL__ \
    template<typename Plugin__>                                                          __NL__ \
    static void                                                                          __NL__ \
    call(Plugin__ &plugin, uint16_t offset) {                                            __NL__ \
      if (plugin.storageSize()) {                                                        __NL__ \
        Serial.print(plugin.name());                                                     __NL__ \
        Serial.print(" ");                                                               __NL__ \
        Serial.print(offset, DEC);                                                       __NL__ \
        Serial.print(" - ");                                                             __NL__ \
        Serial.println(offset + plugin.storageSize() - 1, DEC);                          __NL__ \
      }                                                                                  __NL__ \
    }                                                                                    __NL__ \
  };                                                                                     __NL__ \
                                                                                         __NL__ \
  struct EventHandler_getStorageOffset {                                                 __NL__ \
    template<typename Plugin__>                                                          __NL__ \
    static uint16_t                                                                      __NL__ \
    call(Plugin__ &plugin) {                                                             __NL__ \
      return plugin.storageSize();                                                       __NL__ \
    }                                                                                    __NL__ \
  };                                                                                     __NL__ \
  }                                                                                      __NL__ \
                                                                                         __NL__ \
  namespace kaleidoscope {                                                               __NL__ \
  void Storage::report() {                                                               __NL__ \
    kaleidoscope_internal::EventDispatcher::template                                     __NL__ \
      apply_storage<kaleidoscope_internal::EventHandler_onStorageReport>();              __NL__ \
  }                                                                                      __NL__ \
                                                                                         __NL__ \
  template<typename Plugin__>                                                            __NL__ \
  uint16_t Storage::getOffset(Plugin__ &plugin) {                                        __NL__ \
    return kaleidoscope_internal::EventDispatcher::template                              __NL__ \
      apply_offsetCount<kaleidoscope_internal::EventHandler_getStorageOffset>(plugin);   __NL__ \
  }                                                                                      __NL__ \
  }
