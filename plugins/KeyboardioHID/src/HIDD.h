/* HID driver back end */

#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "HID.h"

/* Maximum length of output report sent by host */
#define HIDD_MAX_OUTPUT_REPORT 2

class HIDD : public PluggableUSBModule {
 public:
  HIDD(
    uint8_t _protocol       = HID_REPORT_PROTOCOL,
    uint8_t _idle           = 0,
    uint8_t _itfProtocol    = HID_PROTOCOL_NONE,
    uint8_t _inReportLen    = USB_EP_SIZE,
    uint8_t _interval       = 1,
    const void *_reportDesc = NULL,
    uint16_t descriptorSize = 0);
  virtual int SendReport(uint8_t id, const void *data, int len);
  uint8_t getProtocol() {
    return protocol;
  }

 protected:
  // Implementation of the PluggableUSBModule
  int getInterface(uint8_t *interfaceCount);
  int getDescriptor(USBSetup &setup);
  bool setup(USBSetup &setup);

  void plug() {
    PluggableUSB().plug(this);
  }

  /* Current HID protocol: Boot or Report */
  uint8_t protocol;
  uint8_t idle;
  /* Interface protocol, for interface descriptor (only boot devices) */
  uint8_t itfProtocol;
  uint8_t inReportLen;
  uint8_t outReportLen;
  uint8_t interval;
  const void *reportDesc;
  uint16_t descriptorSize;

  uint8_t outReport[HIDD_MAX_OUTPUT_REPORT];
  virtual void setReportCB(const void *data, uint16_t len) {
    (void)data;
    (void)len;
  }

 private:
  EPTYPE_DESCRIPTOR_SIZE epType[1];
  int SendReportNoID(const void *data, int len);
};
