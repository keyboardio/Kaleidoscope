#pragma once
#include <inttypes.h>
#include "PtrArray.h"

/** The EventDispatcher class is a small abstraction over disaptching
 * input events to a connected party.
 * This may be a USB device or a Bluetooth Low Energy device, or
 * perhaps some other future device connection kind.
 * The dispatcher allows a slightly easier separation of implementation
 * detail from the main matrix scan and dispatch loop. */
class EventDispatcher {
public:
  // Meyers singleton to avoid SIOF issues
  static PtrArray<8, EventDispatcher> &eventDispatchers() {
    static PtrArray<8, EventDispatcher> dispatchers;
    return dispatchers;
  }

  virtual ~EventDispatcher() = default;
  EventDispatcher() {
    eventDispatchers().insertOrReplace(this);
  }

  /** The connection type defines a bitmask that indicates the
   * desired endpoints to which to transmit an event */
  enum ConnectionType {
    Usb = 0b01, // Connected via USB
    Ble = 0b10, // Connected via BLE
  };

  /** Will be called by Kaleidoscope::setup */
  virtual void begin() = 0;

  /** Query available connections.
   * The implementation should turn on the appropriate ConnectionType bit
   * in the provided connectionMask.  Make sure that any existing bits
   * are preserved because the implementation may be composed of
   * multiple different connections. */
  virtual void queryConnectionTypes(uint8_t &connectionMask) = 0;

  /** Consumer keycode press and release.
   * connectionMask is set to indicate which connections should act
   * upon this request; the implementation should NOP if the connectionMask
   * doesn't match that of the implementation. */
  virtual void consumerPress(uint8_t connectionMask, uint8_t keyCode) = 0;
  virtual void consumerRelease(uint8_t connectionMask, uint8_t keyCode) = 0;

  /** system control keycode press and release.
   * See above for commentary on connectionMask. */
  virtual void systemPress(uint8_t connectionMask, uint8_t keyCode) = 0;
  virtual void systemRelease(uint8_t connectionMask, uint8_t keyCode) = 0;

  /** Regular key press and release.
   * See above for commentary on connectionMask. */
  virtual void keyPress(uint8_t connectionMask, uint8_t keyCode) = 0;
  virtual void keyRelease(uint8_t connectionMask, uint8_t keyCode) = 0;
  virtual void keyReleaseAll(uint8_t connectionMask) = 0;
  virtual void keySendReport(uint8_t connectionMask) = 0;
};
