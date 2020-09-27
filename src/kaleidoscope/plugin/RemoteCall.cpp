/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2019  Keyboard.io, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "kaleidoscope/plugin/RemoteCall.h"

#ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED

#include "kaleidoscope/plugin/FocusSerial.h"
#include "kaleidoscope/plugin/LEDControl.h"

namespace kaleidoscope {

namespace remote_call {
extern const PROGMEM int8_t firmware_checksum[8];
}

namespace plugin {
namespace remote_call {
struct TransferInfo {
  uintptr_t addr_int_;
  uint16_t size_bytes_;
  TransferInfo() {
    ::Focus.read(addr_int_);
    ::Focus.read(size_bytes_);
  }

  template<typename _T>
  void sendData() {
    ::Focus.send(*((_T*)addr_int_));
  }

  template<typename _T>
  void receiveData() {
    _T data;
    ::Focus.read(data);
    *(_T*)addr_int_ = data;
  }
};
} // namespace remote_call

// void showReaction() {
//    LEDControl::set_all_leds_to(255, 0, 0);
//    LEDControl::syncLeds();
//    delay(500);
//    LEDControl::set_all_leds_to(0, 0, 0);
//    LEDControl::syncLeds();
// }

EventHandlerResult RemoteCall_::onFocusEvent(const char *command) {
  using namespace remote_call;

  //showReaction();

  // The number of chars of the string "remote."
  //
  constexpr uint8_t cmd_start_offset = 7;

#ifdef KALEIDOSCOPE_REMOTE_CALL_HAVE_FIRMWARE_CHECKSUM
#define FIRMWARE_CHECKSUM_COMMAND "\nremote.checksum"
#else
#define FIRMWARE_CHECKSUM_COMMAND
#endif

  if (::Focus.handleHelp(command,
                         PSTR("remote.read"
                              "\nremote.write"
                              "\nremote.peek"
                              "\nremote.poke"
                              "\nremote.call"
                              FIRMWARE_CHECKSUM_COMMAND
                             )
                        )
     )
    return EventHandlerResult::OK;

  if (strcmp_P(command + cmd_start_offset, PSTR("read")) == 0) {

    TransferInfo ti;

    for (uint16_t n = 0; n < ti.size_bytes_; ++n) {
      auto addr = (uint8_t*)ti.addr_int_ + n;
      ::Focus.send(*addr);
    }
  } else if (strcmp_P(command + cmd_start_offset, PSTR("write")) == 0) {

    TransferInfo ti;

    for (uint16_t n = 0; n < ti.size_bytes_; ++n) {
      uint8_t data;
      ::Focus.read(data);
      auto addr = (uint8_t*)ti.addr_int_ + n;
      *addr = data;
    }
  } else if (strcmp_P(command + cmd_start_offset, PSTR("peek")) == 0) {
    TransferInfo ti;

    switch (ti.size_bytes_) {
    case 1:
      ti.sendData<uint8_t>();
      break;
    case 2:
      ti.sendData<uint16_t>();
      break;
    case 4:
      ti.sendData<uint32_t>();
      break;
    default:
      // Error
      break;
    }
  } else if (strcmp_P(command + cmd_start_offset, PSTR("poke")) == 0) {
    TransferInfo ti;

    switch (ti.size_bytes_) {
    case 1:
      ti.receiveData<uint8_t>();
      break;
    case 2:
      ti.receiveData<uint16_t>();
      break;
//          case 4:
//             ti.receiveData<uint32_t>();
//             break;
    default:
      // Error
      break;
    }
  } else if (strcmp_P(command + cmd_start_offset, PSTR("call")) == 0) {
    uintptr_t addr_int;
    ::Focus.read(addr_int);

    typedef void (*Func)();

    auto f = (Func)addr_int;

    f();
  }
#ifdef KALEIDOSCOPE_REMOTE_CALL_HAVE_FIRMWARE_CHECKSUM
  else if (strcmp_P(command + cmd_start_offset, PSTR("checksum")) == 0) {

    char hash[8];
    memcpy_P(hash, kaleidoscope::remote_call::firmware_checksum, 8);

    for (uint8_t i = 0; i < 8; ++i) {
      ::Focus.send(hash[i]);
    }
  }
#endif
  else {
    return EventHandlerResult::OK;
  }

  return EventHandlerResult::EVENT_CONSUMED;
}

} // namespace plugin
} // namespace kaleidoscope

#endif // #ifndef KALEIDOSCOPE_REMOTE_CALL_DISABLED

kaleidoscope::plugin::RemoteCall_ RemoteCall;
