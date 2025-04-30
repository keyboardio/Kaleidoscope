/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2023-2025 Keyboard.io, inc.
 *
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * Additional Permissions:
 * As an additional permission under Section 7 of the GNU General Public
 * License Version 3, you may link this software against a Vendor-provided
 * Hardware Specific Software Module under the terms of the MCU Vendor
 * Firmware Library Additional Permission Version 1.0.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#if defined(ARDUINO_ARCH_NRF52) || defined(KALEIDOSCOPE_VIRTUAL_BUILD)

#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "kaleidoscope/driver/storage/Base.h"

namespace kaleidoscope {
namespace driver {
namespace storage {

#define NRF52FLASH_DEBUG 1  // Set to 0 to disable debug messages

#if NRF52FLASH_DEBUG
#define DEBUG_MSG(...) Serial.println(F(__VA_ARGS__))
#else
#define DEBUG_MSG(...)
#endif

struct NRF52FlashProps : kaleidoscope::driver::storage::BaseProps {
  static constexpr uint16_t length = 16384;
};

template<typename _StorageProps>
class NRF52Flash : public kaleidoscope::driver::storage::Base<_StorageProps> {
 private:
  static constexpr const char *EEPROM_PATH = "/eeprom.dat";
  static Adafruit_LittleFS_Namespace::File file;
  static bool is_initialized;
  static uint8_t *contents;  // Add buffer to store file contents
  static bool dirty_;        // Add flag to track if contents have changed

  static bool init() {
    if (is_initialized) return true;

    DEBUG_MSG("[NRF52Flash] Initializing");
    
    // Allocate memory for contents
    if (contents == nullptr) {
      contents = new uint8_t[_StorageProps::length];
      // Initialize with uninitialized_byte as default
      memset(contents, _StorageProps::uninitialized_byte, _StorageProps::length);
      DEBUG_MSG("[NRF52Flash] Memory allocated");
    }

    // Initialize the filesystem
    InternalFS.end();
    if (!InternalFS.begin()) {
      DEBUG_MSG("[NRF52Flash] Failed to initialize filesystem");
      return false;
    }
    DEBUG_MSG("[NRF52Flash] Filesystem initialized");

    // Load file contents into memory if file exists
    if (InternalFS.exists(EEPROM_PATH)) {
      DEBUG_MSG("[NRF52Flash] File exists, loading contents");
      
      if (file.isOpen()) {
        file.close();
      }
      
      if (!file.open(EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
        DEBUG_MSG("[NRF52Flash] Failed to open file for reading");
        // Continue with default initialized memory
        is_initialized = true;
        return true;
      }
      
      // Check file size is correct
      size_t file_size = file.size();
      if (file_size != _StorageProps::length) {
        DEBUG_MSG("[NRF52Flash] File has incorrect size");
        file.close();
        // File will be fixed by verifyAndFixFile() later
        is_initialized = true;
        return true;
      }
      
      // Read file contents into memory
      size_t read_size = file.read(contents, _StorageProps::length);
      file.close();

      if (read_size != _StorageProps::length) {
        DEBUG_MSG("[NRF52Flash] Failed to read entire file");
        DEBUG_MSG("[NRF52Flash] Read ");
        char buffer[10];
        itoa(read_size, buffer, 10);
        DEBUG_MSG(buffer);
        DEBUG_MSG(" bytes out of ");
        itoa(_StorageProps::length, buffer, 10);
        DEBUG_MSG(buffer);
        // Keep the default initialized memory
        is_initialized = true;
        return true;
      }
      
      DEBUG_MSG("[NRF52Flash] File loaded successfully");
    } else {
      DEBUG_MSG("[NRF52Flash] No existing file found");
      // If file doesn't exist, we'll keep the default initialized memory
      // and create the file when we call commit()
    }

    is_initialized = true;
    DEBUG_MSG("[NRF52Flash] Initialization complete");
    return true;
  }

  static bool verifyAndFixFile() {
    if (!init()) return false;

    // Check if file exists and has correct size
    if (InternalFS.exists(EEPROM_PATH)) {
      DEBUG_MSG("[NRF52Flash] File exists, checking size");
      
      if (file.isOpen()) {
        file.close();
      }
      
      if (!file.open(EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
        DEBUG_MSG("[NRF52Flash] Failed to open file");
        return false;
      }

      size_t size = file.size();
      file.close();

      if (size == _StorageProps::length) {
        DEBUG_MSG("[NRF52Flash] File size is correct");
        return true;
      }

      DEBUG_MSG("[NRF52Flash] File exists but has wrong size, recreating it");
      
      // Close the file before removing it
      if (file.isOpen()) {
        file.close();
      }
      
      // Remove the file and create a new one with the correct size
      if (!InternalFS.remove(EEPROM_PATH)) {
        DEBUG_MSG("[NRF52Flash] Failed to remove existing file");
      }
    } else {
      DEBUG_MSG("[NRF52Flash] File doesn't exist, creating it");
    }

    // Create new file
    if (file.isOpen()) {
      file.close();
    }
    
    if (!file.open(EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_WRITE)) {
      DEBUG_MSG("[NRF52Flash] Failed to create file");
      return false;
    }

    DEBUG_MSG("[NRF52Flash] Writing initial data");
    
    // Initialize with uninitialized_byte
    uint8_t buffer[64];
    memset(buffer, _StorageProps::uninitialized_byte, sizeof(buffer));

    size_t remaining = _StorageProps::length;
    size_t total_written = 0;
    
    while (remaining > 0) {
      size_t to_write = (remaining < sizeof(buffer)) ? remaining : sizeof(buffer);
      size_t written = file.write(buffer, to_write);
      
      if (written != to_write) {
        DEBUG_MSG("[NRF52Flash] Failed to write to file");
        char numstr[10];
        itoa(written, numstr, 10);
        DEBUG_MSG(numstr);
        DEBUG_MSG(" bytes written out of ");
        itoa(to_write, numstr, 10);
        DEBUG_MSG(numstr);
        file.close();
        return false;
      }
      
      remaining -= written;
      total_written += written;
    }

    file.close();
    
    DEBUG_MSG("[NRF52Flash] File created successfully");
    char numstr[10];
    itoa(total_written, numstr, 10);
    DEBUG_MSG(numstr);
    DEBUG_MSG(" bytes written");
    
    return true;
  }

  static bool checkBounds(uint16_t offset, uint16_t size) {
    bool result = (offset + size <= _StorageProps::length);
    return result;
  }

  static bool writeToFile() {
    // Make sure the file is closed before we open it
    if (file.isOpen()) {
      file.close();
    }

    // First check if the file exists and has the correct size
    if (InternalFS.exists(EEPROM_PATH)) {
      if (!file.open(EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
        DEBUG_MSG("[NRF52Flash] Failed to open file for size check");
        return false;
      }
      
      size_t size = file.size();
      file.close();
      
      if (size != _StorageProps::length) {
        DEBUG_MSG("[NRF52Flash] File size is incorrect, recreating");
        InternalFS.remove(EEPROM_PATH);
        
        if (!verifyAndFixFile()) {
          DEBUG_MSG("[NRF52Flash] Failed to recreate file");
          return false;
        }
      }
    } else {
      DEBUG_MSG("[NRF52Flash] File doesn't exist for writing, creating it");
      if (!verifyAndFixFile()) {
        DEBUG_MSG("[NRF52Flash] Failed to create file");
        return false;
      }
    }

    // Open file with FILE_O_UPDATE to update it in-place
    if (!file.open(EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_UPDATE)) {
      DEBUG_MSG("[NRF52Flash] Failed to open file for updating");
      return false;
    }

    // Reset position to beginning
    if (!file.seek(0)) {
      DEBUG_MSG("[NRF52Flash] Failed to seek to beginning of file");
      file.close();
      return false;
    }
    
    DEBUG_MSG("[NRF52Flash] Writing data");
    size_t written = file.write(contents, _StorageProps::length);
    file.close();
    
    if (written != _StorageProps::length) {
      DEBUG_MSG("[NRF52Flash] Write failed");
      DEBUG_MSG("[NRF52Flash] Written ");
      // this only takes character arrays, so we need to convert the size_t to a character array
      char buffer[10];
      itoa(written, buffer, 10);
      DEBUG_MSG(buffer);
      DEBUG_MSG(", expected ");
      itoa(_StorageProps::length, buffer, 10);
      DEBUG_MSG(buffer);
      DEBUG_MSG(" bytes");
      return false;
    }

    DEBUG_MSG("[NRF52Flash] Write successful");
    return true;
  }

 public:
  template<typename T>
  static T &get(uint16_t offset, T &t) {
    if (!init() || !checkBounds(offset, sizeof(T))) {
      return t;
    }

    memcpy(&t, contents + offset, sizeof(T));
    return t;
  }

  template<typename T>
  static const T &put(uint16_t offset, T &t) {
    if (!init() || !checkBounds(offset, sizeof(T))) {
      return t;
    }

    // Update in-memory contents only
    memcpy(contents + offset, &t, sizeof(T));
    dirty_ = true;

    return t;
  }

  uint8_t read(int idx) {
    if (!init() || !checkBounds(idx, 1)) {
      return 0;
    }
    return contents[idx];
  }

  void write(int idx, uint8_t val) {
    if (!init() || !checkBounds(idx, 1)) {
      return;
    }
    contents[idx] = val;
    dirty_        = true;
  }

  void update(int idx, uint8_t val) {
    if (read(idx) != val) {
      write(idx, val);
    }
  }

  bool isSliceUninitialized(uint16_t offset, uint16_t size) {
    // First check if the file exists
    if (!InternalFS.exists(EEPROM_PATH)) {
      return true;
    }
    
    // Then try to open and read it
    if (!file.open(EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
      DEBUG_MSG("[NRF52Flash] Failed to open file for reading");
      return true;
    }
    
    if (!file.seek(offset)) {
      DEBUG_MSG("[NRF52Flash] Failed to seek to offset");
      file.close();
      return true;
    }

    uint8_t *buffer  = new uint8_t[size];
    size_t read_size = file.read(buffer, size);
    file.close();

    bool uninitialized = true;
    if (read_size == size) {
      for (uint16_t i = 0; i < size; i++) {
        if (buffer[i] != _StorageProps::uninitialized_byte) {
          uninitialized = false;
          break;
        }
      }
    }

    delete[] buffer;
    return uninitialized;
  }

  const uint16_t length() {
    return _StorageProps::length;
  }

  void setup() {
    if (!is_initialized) {
      verifyAndFixFile();
      is_initialized = true;
    }
  }

  void commit() {
    if (dirty_) {
      DEBUG_MSG("[NRF52Flash] Committing changes to file");
      if (writeToFile()) {
        DEBUG_MSG("[NRF52Flash] Commit successful");
        dirty_ = false;
      } else {
        DEBUG_MSG("[NRF52Flash] Commit failed");
      }
    } else {
      DEBUG_MSG("[NRF52Flash] No changes to commit");
    }
  }

  void erase() {
    // Initialize in-memory contents with uninitialized bytes
    memset(contents, _StorageProps::uninitialized_byte, _StorageProps::length);
    
    // Mark as dirty so it will be written on next commit
    dirty_ = true;
    
    // Write immediately to keep behavior consistent with previous implementation
    commit();
  }
};

template<typename _StorageProps>
bool NRF52Flash<_StorageProps>::is_initialized = false;

template<typename _StorageProps>
Adafruit_LittleFS_Namespace::File NRF52Flash<_StorageProps>::file(InternalFS);

template<typename _StorageProps>
uint8_t *NRF52Flash<_StorageProps>::contents = nullptr;

template<typename _StorageProps>
bool NRF52Flash<_StorageProps>::dirty_ = false;

}  // namespace storage
}  // namespace driver
}  // namespace kaleidoscope

#endif
