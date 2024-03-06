// FocusPlugin.h
#pragma once
#include <Kaleidoscope-FocusSerial.h>  // for Focus, FocusSerial

#include "kaleidoscope/plugin.h"  // for Plugin

namespace kaleidoscope {
namespace plugin {

class FocusPlugin {
 public:
  EventHandlerResult onNameQuery() {
    return ::Focus.sendName(getPluginName());
  }

 protected:
  // New virtual method to be overridden by derived classes to return the plugin name
  virtual const __FlashStringHelper *getPluginName() const = 0;
};

}  // namespace plugin
}  // namespace kaleidoscope
