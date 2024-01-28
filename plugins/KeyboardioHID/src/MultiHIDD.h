#pragma once

#include <Arduino.h>
#include <stdio.h>

#include "HID.h"
#include "HIDD.h"
#include "HID-Settings.h"

class HIDSubDescriptor {
 public:
  HIDSubDescriptor *next = NULL;
  HIDSubDescriptor(const void *d, const uint16_t l)
    : data(d), length(l) {}

  const void *data;
  const uint16_t length;
};

class HID_ : public HIDD {
 public:
  HID_();
  int begin();
  int SendReport(uint8_t id, const void *data, int len) override;
  void AppendDescriptor(HIDSubDescriptor *node);
  uint8_t getLEDs() {
    return outReport[1];
  }

 protected:
  // Implementation of the PluggableUSBModule
  int getDescriptor(USBSetup &setup) override;
  uint8_t getShortName(char *name);

 private:
  HIDSubDescriptor *rootNode;
};

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
HID_ &HID();
