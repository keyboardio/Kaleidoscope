#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#include "plugin.h"

namespace kaleidoscope {
class Storage {
 public:
  template<typename P, typename T>
  static T& get(P &plugin, T& t, uint16_t offset = 0) {
    uint16_t plugin_offset = getOffset(plugin);
    return EEPROM.get(plugin_offset + offset, t);
  }

  template<typename P, typename T>
  static const T& put(P &plugin, T& t, uint16_t offset = 0) {
    uint16_t plugin_offset = getOffset(plugin);
    return EEPROM.put(plugin_offset + offset, t);
  }

  static void report();

  template<typename Plugin__>
  static uint16_t getOffset(Plugin__ &plugin);
};
}
