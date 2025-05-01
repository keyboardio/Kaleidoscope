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

/**
 * Helper class to manage file operations for NRF52 storage
 * Handles chunking of storage data across multiple files
 * to work around LittleFS limitations
 */
template<typename _StorageProps>
class StorageFileManager {
 private:
  // File paths and constants
  static constexpr const char* V1_STORAGE_FILE_PATH = "/eeprom.dat";
  static constexpr const char* PAGE_FILE_PREFIX = "/eeprom";
  static constexpr const char* PAGE_FILE_SUFFIX = ".dat";
  static constexpr uint16_t PAGE_FILE_SIZE = 1024;  // 4KB per file maximum, but we use a smaller size to reduce the number of files a given write needs to touch, which improves perf.
  static constexpr uint16_t PAGE_COUNT = (_StorageProps::length + PAGE_FILE_SIZE - 1) / PAGE_FILE_SIZE;
  
  // Storage for data and state tracking
  static uint8_t buffer_[_StorageProps::length];
  static uint8_t dirty_flags_[PAGE_COUNT];
  static bool is_initialized_;
  static bool any_data_loaded_;
  
  static Adafruit_LittleFS_Namespace::File file_;
  
  // Get the path for a specific page
  static void getPagePath(char* path_buffer, size_t buffer_size, uint16_t page_index) {
    snprintf(path_buffer, buffer_size, "%s%d%s", PAGE_FILE_PREFIX, page_index + 1, PAGE_FILE_SUFFIX);
  }
  
  // Initialize the filesystem
  static bool initFilesystem() {
    // Try to shut down filesystem first in case it's in a bad state
    InternalFS.end();
    
    // Reinitialize the filesystem
    if (!InternalFS.begin()) {
      DEBUG_MSG("[StorageManager] Failed to initialize filesystem");
      return false;
    }
    
    DEBUG_MSG("[StorageManager] Filesystem initialized");
    return true;
  }
  
  // Check if V1 storage file exists and migrate if needed
  static bool checkAndMigrateV1StorageFile() {
    if (InternalFS.exists(V1_STORAGE_FILE_PATH)) {
      DEBUG_MSG("[StorageManager] V1 storage file exists");
      
      if (file_.isOpen()) file_.close();
      
      if (!file_.open(V1_STORAGE_FILE_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
        DEBUG_MSG("[StorageManager] Failed to open V1 storage file");
        return false;
      }
      
      size_t v1_size = file_.size();
      file_.close();
      
      // Check if the V1 file size matches our current configuration
      if (v1_size != _StorageProps::length) {
        DEBUG_MSG("[StorageManager] V1 storage file size mismatch - cannot migrate");
        DEBUG_MSG("[StorageManager] Deleting incompatible V1 storage file");
        
        // Delete the V1 file without migration
        if (InternalFS.remove(V1_STORAGE_FILE_PATH)) {
          DEBUG_MSG("[StorageManager] Deleted incompatible V1 storage file");
        } else {
          DEBUG_MSG("[StorageManager] Failed to delete V1 storage file");
        }
        
        return true;
      }
      
      // Size matches, proceed with migration
      DEBUG_MSG("[StorageManager] Migrating V1 storage file");
      
      if (!file_.open(V1_STORAGE_FILE_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
        DEBUG_MSG("[StorageManager] Failed to reopen V1 storage file");
        return false;
      }
      
      size_t bytes_read = file_.read(buffer_, v1_size);
      file_.close();
      
      if (bytes_read != v1_size) {
        DEBUG_MSG("[StorageManager] Failed to read V1 storage file");
        return false;
      }
      
      // Mark all pages as dirty to ensure they're all written
      memset(dirty_flags_, 1, PAGE_COUNT);
      
      // Write to new pages
      bool success = commitChanges();
      
      // Delete v1 file if successful
      if (success && InternalFS.remove(V1_STORAGE_FILE_PATH)) {
        DEBUG_MSG("[StorageManager] Migration completed successfully");
      }
      
      return success;
    }
    
    return true; // No v1 storage file to migrate
  }
  
  // Load data from a specific page
  static bool loadPage(uint16_t page_index) {
    char path[32]; // Buffer for path
    getPagePath(path, sizeof(path), page_index);
    uint16_t offset = page_index * PAGE_FILE_SIZE;
    
    if (!InternalFS.exists(path)) {
      // Fill with uninitialized bytes if page file doesn't exist
      memset(buffer_ + offset, _StorageProps::uninitialized_byte, 
             min(PAGE_FILE_SIZE, _StorageProps::length - offset));
      return false;
    }
    
    if (file_.isOpen()) file_.close();
    
    if (!file_.open(path, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
      DEBUG_MSG("[StorageManager] Failed to open page file for reading");
      return false;
    }
    
    uint16_t expected_size = min(PAGE_FILE_SIZE, _StorageProps::length - offset);
    size_t read_size = file_.read(buffer_ + offset, expected_size);
    file_.close();
    
    if (read_size != expected_size) {
      DEBUG_MSG("[StorageManager] Failed to read entire page file");
      return false;
    }
    
    return true;
  }
  
  // Save a specific page if it's dirty
  static bool savePage(uint16_t page_index) {
    // Skip if not dirty
    if (!dirty_flags_[page_index]) {
      return true;
    }
    
    char path[32]; // Buffer for path
    getPagePath(path, sizeof(path), page_index);
    uint16_t offset = page_index * PAGE_FILE_SIZE;
    uint16_t page_size = min(PAGE_FILE_SIZE, _StorageProps::length - offset);
    
    if (file_.isOpen()) file_.close();
    
    // Use appropriate open mode based on file existence
    bool file_exists = InternalFS.exists(path);
    if (!file_.open(path, 
                   file_exists ? Adafruit_LittleFS_Namespace::FILE_O_OVERWRITE : 
                                 Adafruit_LittleFS_Namespace::FILE_O_WRITE)) {
      DEBUG_MSG("[StorageManager] Failed to open page file for writing");
      return false;
    }
    
    // If file exists, seek to beginning
    if (file_exists && !file_.seek(0)) {
      DEBUG_MSG("[StorageManager] Failed to seek to start of file");
      file_.close();
      return false;
    }
    
    const uint16_t WRITE_CHUNK_SIZE = 1024;
    size_t total_written = 0;
    
    for (uint16_t write_offset = 0; write_offset < page_size; write_offset += WRITE_CHUNK_SIZE) {
      uint16_t bytes_to_write = min(WRITE_CHUNK_SIZE, page_size - write_offset);
      size_t bytes_written = file_.write(buffer_ + offset + write_offset, bytes_to_write);
      
      if (bytes_written != bytes_to_write) {
        DEBUG_MSG("[StorageManager] Partial write failure");
        file_.close();
        return false;
      }
      
      total_written += bytes_written;
      delay(1); // Small delay between writes
    }
    
    file_.flush();
    file_.close();
    
    if (total_written != page_size) {
      DEBUG_MSG("[StorageManager] Failed to write page file");
      return false;
    }
    
    // Clear dirty flag
    dirty_flags_[page_index] = 0;
    return true;
  }

 public:
  // Initialize the storage manager
  static bool init() {
    if (is_initialized_) return true;
    
    // Initialize with defaults
    memset(buffer_, _StorageProps::uninitialized_byte, _StorageProps::length);
    memset(dirty_flags_, 0, PAGE_COUNT);
    
    // Initialize filesystem
    if (!initFilesystem()) {
      return false;
    }
    
    // Check for v1 storage file
    if (!checkAndMigrateV1StorageFile()) {
      // Continue even if migration fails
    }
    
    // Load data from pages
    any_data_loaded_ = false;
    for (uint16_t i = 0; i < PAGE_COUNT; i++) {
      if (loadPage(i)) {
        any_data_loaded_ = true;
      }
    }
    
    is_initialized_ = true;
    return true;
  }
  
  // Mark a region as dirty
  static void markDirty(uint16_t offset, uint16_t size) {
    if (!is_initialized_) return;
    
    uint16_t start_page = offset / PAGE_FILE_SIZE;
    uint16_t end_page = (offset + size - 1) / PAGE_FILE_SIZE;
    
    for (uint16_t page = start_page; page <= end_page && page < PAGE_COUNT; page++) {
      dirty_flags_[page] = 1;
    }
  }
  
  // Get a pointer to the buffer
  static uint8_t* getBuffer() {
    return buffer_;
  }
  
  // Check if any pages are dirty
  static bool isDirty() {
    if (!is_initialized_) return false;
    
    for (uint16_t i = 0; i < PAGE_COUNT; i++) {
      if (dirty_flags_[i]) return true;
    }
    
    return false;
  }
  
  // Commit changes to all dirty pages
  static bool commitChanges() {
    if (!is_initialized_ || !isDirty()) return true;
    
    bool all_success = true;
    DEBUG_MSG("[StorageManager] Committing changes");
    
    for (uint16_t i = 0; i < PAGE_COUNT; i++) {
      if (dirty_flags_[i]) {
        bool success = savePage(i);
        if (!success) {
          all_success = false;
          DEBUG_MSG("[StorageManager] WARNING: Failed to save page");
        }
      }
    }
    
    return all_success;
  }
  
  // Check if the manager has been successfully initialized
  static bool isInitialized() {
    return is_initialized_;
  }
  
  // Check if any data was loaded from storage
  static bool anyDataLoaded() {
    return any_data_loaded_;
  }
  
  // Verify and fix files if needed
  static bool verifyAndFixFiles() {
    if (!init()) return false;
    
    bool needs_fixing = false;
    
    // Check if any page files are missing or wrong size
    for (uint16_t i = 0; i < PAGE_COUNT; i++) {
      char path[32]; // Buffer for path
      getPagePath(path, sizeof(path), i);
      
      if (!InternalFS.exists(path)) {
        DEBUG_MSG("[StorageManager] Page file missing");
        needs_fixing = true;
        break;
      }
      
      // Check file size
      if (file_.isOpen()) file_.close();
      
      if (!file_.open(path, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
        DEBUG_MSG("[StorageManager] Failed to open page file for verification");
        needs_fixing = true;
        break;
      }
      
      uint16_t expected_size = min(PAGE_FILE_SIZE, _StorageProps::length - (i * PAGE_FILE_SIZE));
      size_t actual_size = file_.size();
      file_.close();
      
      if (actual_size != expected_size) {
        DEBUG_MSG("[StorageManager] Page file has wrong size");
        needs_fixing = true;
        break;
      }
    }
    
    if (needs_fixing) {
      DEBUG_MSG("[StorageManager] Storage corruption detected, erasing all page files");
      // Delete all page files and start fresh instead of trying to fix them
      deleteAllPageFiles();
      // Mark everything as dirty for next commit
      markDirty(0, _StorageProps::length);
      // Commit changes to save the default values to files
      return commitChanges();
    }
    
    return true;
  }
  
  // Delete all storage page files from the filesystem
  static void deleteAllPageFiles() {
    DEBUG_MSG("[StorageManager] Deleting all page files");
    
    for (uint16_t i = 0; i < PAGE_COUNT; i++) {
      char path[32]; // Buffer for path
      getPagePath(path, sizeof(path), i);
      
      if (InternalFS.exists(path)) {
        if (InternalFS.remove(path)) {
          DEBUG_MSG("[StorageManager] Deleted page file");
        } else {
          DEBUG_MSG("[StorageManager] Failed to delete page file");
        }
      }
    }
    
    // Reset our state
    memset(buffer_, _StorageProps::uninitialized_byte, _StorageProps::length);
    memset(dirty_flags_, 1, PAGE_COUNT);
  }
};

// Static member initialization
template<typename _StorageProps>
uint8_t StorageFileManager<_StorageProps>::buffer_[_StorageProps::length];

template<typename _StorageProps>
uint8_t StorageFileManager<_StorageProps>::dirty_flags_[StorageFileManager<_StorageProps>::PAGE_COUNT];

template<typename _StorageProps>
bool StorageFileManager<_StorageProps>::is_initialized_ = false;

template<typename _StorageProps>
bool StorageFileManager<_StorageProps>::any_data_loaded_ = false;

template<typename _StorageProps>
Adafruit_LittleFS_Namespace::File StorageFileManager<_StorageProps>::file_(InternalFS);

/**
 * NRF52Flash storage driver implementation
 * Uses StorageFileManager to handle underlying storage
 */
template<typename _StorageProps>
class NRF52Flash : public kaleidoscope::driver::storage::Base<_StorageProps> {
 private:
  static bool dirty_;
  
  static bool init() {
    return StorageFileManager<_StorageProps>::init();
  }
  
  static bool checkBounds(uint16_t offset, uint16_t size) {
    return (offset + size <= _StorageProps::length);
  }

 public:
  template<typename T>
  static T &get(uint16_t offset, T &t) {
    if (!init() || !checkBounds(offset, sizeof(T))) {
      return t;
    }
    
    uint8_t* buffer = StorageFileManager<_StorageProps>::getBuffer();
    memcpy(&t, buffer + offset, sizeof(T));
    return t;
  }

  template<typename T>
  static const T &put(uint16_t offset, T &t) {
    if (!init() || !checkBounds(offset, sizeof(T))) {
      return t;
    }
    
    uint8_t* buffer = StorageFileManager<_StorageProps>::getBuffer();
    if (memcmp(buffer + offset, &t, sizeof(T)) != 0) {
      memcpy(buffer + offset, &t, sizeof(T));
      StorageFileManager<_StorageProps>::markDirty(offset, sizeof(T));
      dirty_ = true;
    }
    
    return t;
  }

  uint8_t read(int idx) {
    if (!init() || !checkBounds(idx, 1)) {
      return 0;
    }
    return StorageFileManager<_StorageProps>::getBuffer()[idx];
  }

  void write(int idx, uint8_t val) {
    if (!init() || !checkBounds(idx, 1)) {
      return;
    }
    
    uint8_t* buffer = StorageFileManager<_StorageProps>::getBuffer();
    if (buffer[idx] != val) {
      buffer[idx] = val;
      StorageFileManager<_StorageProps>::markDirty(idx, 1);
      dirty_ = true;
    }
  }

  void update(int idx, uint8_t val) {
    if (read(idx) != val) {
      write(idx, val);
    }
  }

  bool isSliceUninitialized(uint16_t offset, uint16_t size) {
    if (!init() || !checkBounds(offset, size)) {
      return true;
    }
    
    uint8_t* buffer = StorageFileManager<_StorageProps>::getBuffer();
    for (uint16_t i = 0; i < size; i++) {
      if (buffer[offset + i] != _StorageProps::uninitialized_byte) {
        return false;
      }
    }
    
    return true;
  }

  const uint16_t length() {
    return _StorageProps::length;
  }

  void setup() {
    StorageFileManager<_StorageProps>::verifyAndFixFiles();
  }

  void commit() {
    if (dirty_ && StorageFileManager<_StorageProps>::isDirty()) {
      DEBUG_MSG("[NRF52Flash] Committing changes");
      StorageFileManager<_StorageProps>::commitChanges();
      dirty_ = false;
    }
  }

  void erase() {
    if (!init()) return;
    
    // Have the storage manager delete all page files
    StorageFileManager<_StorageProps>::deleteAllPageFiles();
    
    // Mark everything as dirty for next commit
    StorageFileManager<_StorageProps>::markDirty(0, _StorageProps::length);
    dirty_ = true;
    commit();
  }
};

template<typename _StorageProps>
bool NRF52Flash<_StorageProps>::dirty_ = false;

}  // namespace storage
}  // namespace driver
}  // namespace kaleidoscope

#endif

