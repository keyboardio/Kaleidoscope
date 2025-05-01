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
  // Split storage into four 4KB files to work around LittleFS 8KB limit
  static constexpr const char *LEGACY_EEPROM_PATH = "/eeprom.dat";
  static constexpr const char *EEPROM_PATH_PART1 = "/eeprom1.dat";
  static constexpr const char *EEPROM_PATH_PART2 = "/eeprom2.dat";
  static constexpr const char *EEPROM_PATH_PART3 = "/eeprom3.dat";
  static constexpr const char *EEPROM_PATH_PART4 = "/eeprom4.dat";
  static constexpr uint16_t MAX_FILE_SIZE = 4096;  // 4KB per file maximum
  static bool data_loss_reported;  // Track if we've already reported data loss
  static uint8_t chunk_dirty_flags[4];  // Bitmap to track which chunks have been modified
  
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
      
      // Clear dirty flags
      memset(chunk_dirty_flags, 0, sizeof(chunk_dirty_flags));
    }

    // Initialize the filesystem
    InternalFS.end();
    if (!InternalFS.begin()) {
      DEBUG_MSG("[NRF52Flash] Failed to initialize filesystem");
      return false;
    }
    DEBUG_MSG("[NRF52Flash] Filesystem initialized");

    // Check for migration case - legacy file exists
    if (InternalFS.exists(LEGACY_EEPROM_PATH)) {
      DEBUG_MSG("[NRF52Flash] Legacy file exists, migrating data");
      if (!migrateFromLegacyFile()) {
        DEBUG_MSG("[NRF52Flash] Migration failed, using default values");
      }
    } else {
      // Try to load split files if they exist
      loadFileContent();
    }

    is_initialized = true;
    DEBUG_MSG("[NRF52Flash] Initialization complete");
    return true;
  }

  static bool loadPartFile(const char* path, uint16_t offset) {
    if (!InternalFS.exists(path)) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer), 
               "[NRF52Flash] Part file %s does not exist", path);
      DEBUG_MSG(buffer);
      return false;
    }
    
    if (file.isOpen()) {
      file.close();
    }
    
    if (!file.open(path, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer),
               "[NRF52Flash] Failed to open part file %s for reading", path);
      DEBUG_MSG(buffer);
      return false;
    }
    
    // Check file size is correct (should be MAX_FILE_SIZE or less for last part)
    size_t file_size = file.size();
    uint16_t expected_size = min(MAX_FILE_SIZE, _StorageProps::length - offset);
    
    if (file_size != expected_size) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer), 
               "[NRF52Flash] Part file %s has incorrect size: %u, expected: %u", 
               path, (unsigned int)file_size, (unsigned int)expected_size);
      DEBUG_MSG(buffer);
      file.close();
      return false;
    }
    
    // Read file contents into memory at the correct offset
    size_t read_size = file.read(contents + offset, expected_size);
    file.close();

    if (read_size != expected_size) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer),
               "[NRF52Flash] Failed to read entire part file %s", path);
      DEBUG_MSG(buffer);
      return false;
    }
    
    char buffer[80];
    snprintf(buffer, sizeof(buffer), 
             "[NRF52Flash] Part file %s loaded successfully (%u bytes)", 
             path, (unsigned int)read_size);
    DEBUG_MSG(buffer);
    return true;
  }

  static bool migrateFromLegacyFile() {
    DEBUG_MSG("[NRF52Flash] Starting migration from legacy file");
    
    // Try to open the legacy file
    if (file.isOpen()) {
      file.close();
    }
    
    if (!file.open(LEGACY_EEPROM_PATH, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
      DEBUG_MSG("[NRF52Flash] Failed to open legacy file for reading");
      return false;
    }
    
    // Check how much data we can actually read
    size_t legacy_size = file.size();
    size_t bytes_to_read = min((size_t)_StorageProps::length, legacy_size);
    
    char buffer[80];
    snprintf(buffer, sizeof(buffer), 
             "[NRF52Flash] Legacy file size: %u bytes, reading %u bytes", 
             (unsigned int)legacy_size, (unsigned int)bytes_to_read);
    DEBUG_MSG(buffer);
    
    // Read legacy file data into memory
    size_t bytes_read = file.read(contents, bytes_to_read);
    file.close();
    
    if (bytes_read != bytes_to_read) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer), 
               "[NRF52Flash] Failed to read legacy file: read %u of %u bytes", 
               (unsigned int)bytes_read, (unsigned int)bytes_to_read);
      DEBUG_MSG(buffer);
      return false;
    }
    
    // If legacy file was smaller than our total length, fill the rest with uninitialized bytes
    if (bytes_read < _StorageProps::length) {
      DEBUG_MSG("[NRF52Flash] Legacy file smaller than required, padding with uninitialized bytes");
      memset(contents + bytes_read, _StorageProps::uninitialized_byte, _StorageProps::length - bytes_read);
    }
    
    // Write to new split files
    bool success = writeToFile();
    
    if (success) {
      // Delete the legacy file only after successful migration
      if (InternalFS.remove(LEGACY_EEPROM_PATH)) {
        DEBUG_MSG("[NRF52Flash] Legacy file deleted after successful migration");
      } else {
        DEBUG_MSG("[NRF52Flash] Warning: Could not delete legacy file");
      }
      DEBUG_MSG("[NRF52Flash] Migration completed successfully");
    } else {
      DEBUG_MSG("[NRF52Flash] Migration failed");
    }
    
    return success;
  }

  static void markChunkDirty(uint16_t offset, uint16_t size) {
    uint16_t start_chunk = offset / MAX_FILE_SIZE;
    uint16_t end_chunk = (offset + size - 1) / MAX_FILE_SIZE;
    
    // Mark each affected chunk as dirty
    for (uint16_t chunk = start_chunk; chunk <= end_chunk && chunk < sizeof(chunk_dirty_flags); chunk++) {
      chunk_dirty_flags[chunk] = 1;
    }
    
    dirty_ = true;
  }

  static bool isChunkDirty(uint16_t chunk_index) {
    if (chunk_index < sizeof(chunk_dirty_flags)) {
      return chunk_dirty_flags[chunk_index] != 0;
    }
    return false;
  }

  static bool writePartFile(const char* path, uint16_t offset) {
    // Check if this chunk is dirty
    uint16_t chunk_index = offset / MAX_FILE_SIZE;
    if (!isChunkDirty(chunk_index)) {
      DEBUG_MSG("[NRF52Flash] Chunk not modified, skipping write");
      return true;
    }
    
    // Make sure the file is closed before we open it
    if (file.isOpen()) {
      file.close();
    }
    
    // Calculate the size to write for this part
    uint16_t bytes_to_write = min(MAX_FILE_SIZE, _StorageProps::length - offset);
    
    // Create or update the part file
    char buffer[80];
    snprintf(buffer, sizeof(buffer), 
             "[NRF52Flash] Updating part file %s (%u bytes)", 
             path, (unsigned int)bytes_to_write);
    DEBUG_MSG(buffer);
    
    // Use FILE_O_OVERWRITE for existing files, FILE_O_WRITE for new files
    bool use_update_mode = InternalFS.exists(path);
    
    if (use_update_mode) {
      if (!file.open(path, Adafruit_LittleFS_Namespace::FILE_O_OVERWRITE)) {
        DEBUG_MSG("[NRF52Flash] Failed to open part file for updating");
        return false;
      }
      
      // Make sure we start at the beginning of the file
      if (!file.seek(0)) {
        DEBUG_MSG("[NRF52Flash] Failed to seek to start of file");
        file.close();
        return false;
      }
    } else {
      if (!file.open(path, Adafruit_LittleFS_Namespace::FILE_O_WRITE)) {
        DEBUG_MSG("[NRF52Flash] Failed to create part file");
        return false;
      }
    }
    
    // LittleFS seems to have a limit of 4k per write for overwrite/update mode.
    // I wish I understood what was really going on there --jesse
    const uint16_t CHUNK_SIZE = 4096; 
    size_t written = 0;
    
    for (uint16_t chunk_offset = 0; chunk_offset < bytes_to_write; chunk_offset += CHUNK_SIZE) {
      uint16_t chunk_size = min(CHUNK_SIZE, bytes_to_write - chunk_offset);
      size_t bytes_written = file.write(contents + offset + chunk_offset, chunk_size);
      
      if (bytes_written != chunk_size) {
        // Check for specific error code
        char err_buffer[80];
        if (bytes_written == -28) { // LFS_ERR_NOSPC
          snprintf(err_buffer, sizeof(err_buffer), 
                  "[NRF52Flash] No space left on device (offset: %u, trying to write: %u bytes)",
                  (unsigned int)(offset + chunk_offset), (unsigned int)chunk_size);
        } else {
          snprintf(err_buffer, sizeof(err_buffer), 
                  "[NRF52Flash] Failed to write part file: %d of %u bytes written (offset: %u)",
                  (int)bytes_written, (unsigned int)chunk_size, (unsigned int)(offset + chunk_offset));
        }
        DEBUG_MSG(err_buffer);
        file.close();
        return false;
      }
      
      written += bytes_written;
      
      // Add small delay between chunks
      delay(5);
    }
    
    file.flush();
    file.close();
    
    // Clear the dirty flag for this chunk after successful write
    if (chunk_index < sizeof(chunk_dirty_flags)) {
      chunk_dirty_flags[chunk_index] = 0;
    }
    
    char success_buffer[80];
    snprintf(success_buffer, sizeof(success_buffer), 
             "[NRF52Flash] Part file %s written successfully (%u bytes)", 
             path, (unsigned int)written);
    DEBUG_MSG(success_buffer);
    return true;
  }

  static bool loadFileContent() {
    bool any_file_loaded = false;
    
    // Try to load part 1
    bool part1_loaded = loadPartFile(EEPROM_PATH_PART1, 0);
    if (part1_loaded) {
      any_file_loaded = true;
    }
    
    // Try to load part 2 if needed
    if (_StorageProps::length > MAX_FILE_SIZE) {
      bool part2_loaded = loadPartFile(EEPROM_PATH_PART2, MAX_FILE_SIZE);
      if (part2_loaded) {
        any_file_loaded = true;
      }
    }
    
    // Try to load part 3 if needed
    if (_StorageProps::length > MAX_FILE_SIZE * 2) {
      bool part3_loaded = loadPartFile(EEPROM_PATH_PART3, MAX_FILE_SIZE * 2);
      if (part3_loaded) {
        any_file_loaded = true;
      }
    }
    
    // Try to load part 4 if needed
    if (_StorageProps::length > MAX_FILE_SIZE * 3) {
      bool part4_loaded = loadPartFile(EEPROM_PATH_PART4, MAX_FILE_SIZE * 3);
      if (part4_loaded) {
        any_file_loaded = true;
      }
    }
    
    if (any_file_loaded) {
      DEBUG_MSG("[NRF52Flash] EEPROM data loaded successfully");
    } else {
      DEBUG_MSG("[NRF52Flash] No EEPROM files found, using defaults");
    }
    
    return any_file_loaded;
  }

  static bool checkBounds(uint16_t offset, uint16_t size) {
    bool result = (offset + size <= _StorageProps::length);
    return result;
  }
  
  static bool verifyAndFixFiles() {
    if (!init()) return false;

    bool files_ok = true;
    bool needs_fixing = false;
    
    // Check if part files exist and have correct sizes
    if (!verifyPartFile(EEPROM_PATH_PART1, 0)) {
      files_ok = false;
      needs_fixing = true;
      DEBUG_MSG("[NRF52Flash] First part file needs to be fixed");
    }
    
    if (_StorageProps::length > MAX_FILE_SIZE) {
      if (!verifyPartFile(EEPROM_PATH_PART2, MAX_FILE_SIZE)) {
        files_ok = false;
        needs_fixing = true;
        DEBUG_MSG("[NRF52Flash] Second part file needs to be fixed");
      }
    }
    
    if (_StorageProps::length > MAX_FILE_SIZE * 2) {
      if (!verifyPartFile(EEPROM_PATH_PART3, MAX_FILE_SIZE * 2)) {
        files_ok = false;
        needs_fixing = true;
        DEBUG_MSG("[NRF52Flash] Third part file needs to be fixed");
      }
    }
    
    if (_StorageProps::length > MAX_FILE_SIZE * 3) {
      if (!verifyPartFile(EEPROM_PATH_PART4, MAX_FILE_SIZE * 3)) {
        files_ok = false;
        needs_fixing = true;
        DEBUG_MSG("[NRF52Flash] Fourth part file needs to be fixed");
      }
    }
    
    // Proactively fix the files if needed
    if (needs_fixing) {
      DEBUG_MSG("[NRF52Flash] Fixing files now");
      if (writeToFile()) {
        DEBUG_MSG("[NRF52Flash] Files fixed successfully");
        files_ok = true;
      } else {
        DEBUG_MSG("[NRF52Flash] Failed to fix files");
      }
    }
    
    return files_ok;
  }
  
  static bool verifyPartFile(const char* path, uint16_t offset) {
    if (!InternalFS.exists(path)) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer), 
               "[NRF52Flash] Part file %s doesn't exist, will be created on commit", 
               path);
      DEBUG_MSG(buffer);
      return false;
    }
    
    if (file.isOpen()) {
      file.close();
    }
    
    if (!file.open(path, Adafruit_LittleFS_Namespace::FILE_O_READ)) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer), 
               "[NRF52Flash] Failed to open part file %s for verification",
               path);
      DEBUG_MSG(buffer);
      return false;
    }
    
    size_t size = file.size();
    file.close();
    
    uint16_t expected_size = min(MAX_FILE_SIZE, _StorageProps::length - offset);
    
    if (size != expected_size) {
      char buffer[80];
      snprintf(buffer, sizeof(buffer), 
               "[NRF52Flash] Part file %s has wrong size: %u, expected: %u", 
               path, (unsigned int)size, (unsigned int)expected_size);
      DEBUG_MSG(buffer);
      return false;
    }
    
    return true;
  }

  static bool writeToFile() {
    bool success = true;
    DEBUG_MSG("[NRF52Flash] Starting to write files");
    
    // Try writing part 1
    bool part1_success = writePartFile(EEPROM_PATH_PART1, 0);
    if (!part1_success) {
      DEBUG_MSG("[NRF52Flash] Failed to write first part file");
      success = false;
    }
    
    // Try writing part 2 if needed
    if (_StorageProps::length > MAX_FILE_SIZE) {
      bool part2_success = writePartFile(EEPROM_PATH_PART2, MAX_FILE_SIZE);
      if (!part2_success) {
        DEBUG_MSG("[NRF52Flash] Failed to write second part file");
        success = false;
      }
    }
    
    // Try writing part 3 if needed
    if (_StorageProps::length > MAX_FILE_SIZE * 2) {
      bool part3_success = writePartFile(EEPROM_PATH_PART3, MAX_FILE_SIZE * 2);
      if (!part3_success) {
        DEBUG_MSG("[NRF52Flash] Failed to write third part file");
        success = false;
      }
    }
    
    // Try writing part 4 if needed
    if (_StorageProps::length > MAX_FILE_SIZE * 3) {
      bool part4_success = writePartFile(EEPROM_PATH_PART4, MAX_FILE_SIZE * 3);
      if (!part4_success) {
        DEBUG_MSG("[NRF52Flash] Failed to write fourth part file");
        success = false;
      }
    }
    
    if (success) {
      DEBUG_MSG("[NRF52Flash] All files written successfully");
    } else if (!data_loss_reported) {
      DEBUG_MSG("[NRF52Flash] WARNING: Failed to save all EEPROM data");
      data_loss_reported = true;
    }
    
    return success;
  }

  static void format() {
    DEBUG_MSG("[NRF52Flash] Performing filesystem cleanup");
    InternalFS.end();
    InternalFS.erase();
    InternalFS.format();
    InternalFS.begin();
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

    // Check if data is actually changing
    if (memcmp(contents + offset, &t, sizeof(T)) != 0) {
      // Update in-memory contents
      memcpy(contents + offset, &t, sizeof(T));
      // Mark the affected chunks as dirty
      markChunkDirty(offset, sizeof(T));
    }

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
    
    // Only update if value is changing
    if (contents[idx] != val) {
      contents[idx] = val;
      markChunkDirty(idx, 1);
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
    
    bool uninitialized = true;
    for (uint16_t i = 0; i < size; i++) {
      if (contents[offset + i] != _StorageProps::uninitialized_byte) {
        uninitialized = false;
        break;
      }
    }
    
    return uninitialized;
  }

  const uint16_t length() {
    return _StorageProps::length;
  }

  void setup() {
    if (!is_initialized) {
      verifyAndFixFiles();
      is_initialized = true;
    }
  }

  void commit() {
    if (dirty_) {
      DEBUG_MSG("[NRF52Flash] Committing changes to files");
      
      // Only attempt to write if there are dirty chunks
      bool any_dirty = false;
      for (uint8_t i = 0; i < sizeof(chunk_dirty_flags); i++) {
        if (chunk_dirty_flags[i]) {
          any_dirty = true;
          break;
        }
      }
      
      if (!any_dirty) {
        DEBUG_MSG("[NRF52Flash] No chunks marked dirty, skipping commit");
        dirty_ = false;
        return;
      }
      
      if (writeToFile()) {
        DEBUG_MSG("[NRF52Flash] Commit successful");
        dirty_ = false;
      } else {
        DEBUG_MSG("[NRF52Flash] Commit partially failed, some data may not be saved");
        // We still consider the commit "successful" to avoid constant retry loops
        dirty_ = false;
      }
    } else {
      DEBUG_MSG("[NRF52Flash] No changes to commit");
    }
  }

  void erase() {
    // Initialize in-memory contents with uninitialized bytes
    memset(contents, _StorageProps::uninitialized_byte, _StorageProps::length);
    // Mark all chunks as dirty
    memset(chunk_dirty_flags, 1, sizeof(chunk_dirty_flags));
    dirty_ = true;     // Mark as dirty so it will be written on next commit
    commit();    // Write immediately to keep behavior consistent with previous implementation
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

template<typename _StorageProps>
bool NRF52Flash<_StorageProps>::data_loss_reported = false;

template<typename _StorageProps>
uint8_t NRF52Flash<_StorageProps>::chunk_dirty_flags[4] = {0};

}  // namespace storage
}  // namespace driver
}  // namespace kaleidoscope

#endif

