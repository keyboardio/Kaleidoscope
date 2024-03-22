/*
Copyright (c) 2014-2015 NicoHood
Copyright (c) 2015-2024 Keyboard.io, Inc

See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

BootKeyboardAPI::BootKeyboardAPI(uint8_t bootkb_only_)
  : bootkb_only(bootkb_only_) {
}


void BootKeyboardAPI::begin() {
}


void BootKeyboardAPI::end() {
  releaseAll();
  sendReport();
}


uint8_t BootKeyboardAPI::getBootOnly() {
  return bootkb_only;
}

/*
 * Set whether to send only Boot Protocol regardless of whether Boot
 * Protocol has been requested. Used by USBQuirks to toggle protocol
 * modes.
 *
 * This is only really safe to call after detaching from USB, because
 * otherwise, the report format might get out of sync with what the host
 * expects, and the host probably won't see the new Report Descriptor
 * until after a re-attach.
 */
void BootKeyboardAPI::setBootOnly(uint8_t bootonly) {
  bootkb_only = bootonly;
  setReportDescriptor(bootkb_only);
}


void BootKeyboardAPI::convertReport(uint8_t *boot, const uint8_t *nkro) {
  uint8_t n_boot_keys = 0;
  uint8_t b;
  // Convert NKRO report to boot report
  memset(boot, HID_KEYBOARD_NO_EVENT, BOOT_KEY_BYTES);
  for (uint8_t i = 0; i < NKRO_KEY_BYTES; i++) {
    b = nkro[i];
    if (b == 0) {
      continue;
    }
    for (uint8_t j = 0; j < 8; j++) {
      bool bit = b & 1;
      b >>= 1;
      if (bit == 0) {
        continue;
      }
      // Check is here so we can set all BOOT_KEY_BYTES
      if (n_boot_keys >= BOOT_KEY_BYTES) {
        // Send rollover error if too many keys are held
        memset(boot, HID_KEYBOARD_ERROR_ROLLOVER, BOOT_KEY_BYTES);
        return;
      }
      boot[n_boot_keys++] = 8 * i + j;
    }
  }
}

/* Send a report without the extra modifier change handling */
int BootKeyboardAPI::sendReportUnchecked() {
  HID_BootKeyboardReport_Data_t out_report;
  out_report.modifiers = last_report_.modifiers;
  out_report.reserved  = 0;
  memcpy(out_report.nkro_keys, last_report_.keys, sizeof(last_report_.keys));
  convertReport(out_report.boot_keycodes, out_report.nkro_keys);
  size_t reportlen;
  // Send only boot report if host requested boot protocol, or if configured as boot-only
  if (getProtocol() == HID_PROTOCOL_BOOT || bootkb_only) {
    reportlen = BOOT_REPORT_LEN;
  } else {
    reportlen = sizeof(out_report);
  }
  int returnCode = SendHIDReport(&out_report, reportlen);
  return returnCode;
}

// Sending the current HID report to the host:
//
// Depending on the differences between the current and previous HID reports, we
// might need to send one or two extra reports to guarantee that the host will
// process the changes in the correct order. There are two important scenarios
// to consider:
//
// 1. If a non-modifier keycode toggles off in the same report as a modifier
// changes, the host might process the modifier change first. For example, if
// both `shift` and `4` toggle off in the same report (most likely from a
// `LSHIFT(Key_4)` key being released), and that key has been held long enough
// to trigger character repeat, we could end up with a plain `4` in the output
// at the end of the repeat: `$$$$4` instead of `$$$$$`.
//
// 2. If a non-modifier keycode toggles on in the same report as a modifier
// changes, the host might process the non-modifer first. For example, pressing
// and holding an `LSHIFT(Key_4)` key might result in `4$$$` rather than `$$$$`.
//
// Therefore, each call to `sendReport()` must send (up to) three reports to the
// host to guarantee the correct order of processing:
//
// 1. A report with toggled-off non-modifiers removed.
// 2. A report with changes to modifiers.
// 3. A report with toggled-on non-modifiers added.

int BootKeyboardAPI::sendReport() {
  // If the new HID report differs from the previous one both in active modifier
  // keycodes and non-modifier keycodes, we will need to send at least one extra
  // report. First, we compare the modifiers bytes of the two reports.
  const uint8_t old_modifiers = last_report_.modifiers;
  const uint8_t new_modifiers = report_.modifiers;

  const uint8_t changed_modifiers = old_modifiers ^ new_modifiers;

  if (changed_modifiers != 0) {
    // There was at least one modifier change (toggled on or off), remove any
    // non-modifiers from the stored previous report that toggled off in the new
    // report, and send it to the host.
    bool non_modifiers_toggled_off = false;
    for (uint8_t i = 0; i < NKRO_KEY_BYTES; ++i) {
      byte released_keycodes = last_report_.keys[i] & ~(report_.keys[i]);
      if (released_keycodes != 0) {
        last_report_.keys[i] &= ~released_keycodes;
        non_modifiers_toggled_off = true;
      }
    }
    if (non_modifiers_toggled_off) {
      sendReportUnchecked();
    }
    // Next, update the modifiers byte of the stored previous report, and send
    // it.
    last_report_.modifiers = new_modifiers;
    sendReportUnchecked();
  }

  // Finally, copy the new report to the previous one, and send it.
  if (memcmp(last_report_.keys, report_.keys, sizeof(report_.keys)) != 0) {
    memcpy(last_report_.keys, report_.keys, sizeof(report_.keys));
    return sendReportUnchecked();
  }
  // A note on return values: Kaleidoscope doesn't actually check the return
  // value of `sendReport()`, so this function could be changed to return
  // void. It would be nice if we could do something to recover from an error
  // here, but it's not at all clear what that should be. Also note that if the
  // extra reports above return an error, there's not much we can do to try to
  // recover. We could try to send the report again, but that would be likely to
  // fail as well.
  return -1;
}

/* Returns true if the modifer key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboardAPI::isModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(report_.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if the modifer key passed in was being sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboardAPI::wasModifierActive(uint8_t k) {
  if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    return !!(last_report_.modifiers & (1 << k));
  }
  return false;
}

/* Returns true if *any* modifier will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboardAPI::isAnyModifierActive() {
  return report_.modifiers > 0;
}

/* Returns true if *any* modifier was being sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboardAPI::wasAnyModifierActive() {
  return last_report_.modifiers > 0;
}


/* Returns true if the non-modifier key passed in will be sent during this key report
 * Returns false in all other cases
 * */
bool BootKeyboardAPI::isKeyPressed(uint8_t k) {
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return !!(report_.keys[k / 8] & bit);
  }
  return false;
}

/* Returns true if the non-modifer key passed in was sent during the previous key report
 * Returns false in all other cases
 * */
bool BootKeyboardAPI::wasKeyPressed(uint8_t k) {

  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    return !!(last_report_.keys[k / 8] & bit);
  }
  return false;
}


size_t BootKeyboardAPI::press(uint8_t k) {
  // If the key is in the range of 'printable' keys
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (uint8_t(k) % 8);
    report_.keys[k / 8] |= bit;
    return 1;
  } else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // It's a modifier key
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    report_.modifiers |= (1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

size_t BootKeyboardAPI::release(uint8_t k) {
  // If we're releasing a printable key
  if (k <= HID_LAST_KEY) {
    uint8_t bit = 1 << (k % 8);
    report_.keys[k / 8] &= ~bit;
    return 1;
  } else if (k >= HID_KEYBOARD_FIRST_MODIFIER && k <= HID_KEYBOARD_LAST_MODIFIER) {
    // It's a modifier key
    // Convert key into bitfield (0 - 7)
    k = k - HID_KEYBOARD_FIRST_MODIFIER;
    report_.modifiers &= ~(1 << k);
    return 1;
  }

  // No empty/pressed key was found
  return 0;
}

void BootKeyboardAPI::releaseAll() {
  // Release all keys
  memset(&report_.allkeys, 0x00, sizeof(report_.allkeys));
}
