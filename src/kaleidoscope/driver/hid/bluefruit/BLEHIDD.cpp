/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2013-2025 Keyboard.io, inc.
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

#ifdef ARDUINO_ARCH_NRF52

#include "kaleidoscope/driver/hid/apis/AbsoluteMouseAPI.h"
#include "kaleidoscope/driver/hid/apis/BootKeyboardAPI.h"
#include "kaleidoscope/driver/hid/apis/ConsumerControlAPI.h"
#include "kaleidoscope/driver/hid/apis/MouseAPI.h"
#include "kaleidoscope/driver/hid/apis/SystemControlAPI.h"
#include "kaleidoscope/driver/hid/bluefruit/HIDD.h"
#include "kaleidoscope/driver/ble/Bluefruit.h"

namespace kaleidoscope {
namespace driver {
namespace hid {
namespace bluefruit {

// BLE-specific absolute mouse descriptor without buttons to avoid macOS classification conflicts
#define DESCRIPTOR_BLE_ABSOLUTE_MOUSE(...)             \
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),              \
    HID_USAGE(HID_USAGE_DESKTOP_MOUSE),                \
    HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
                                                       \
    /* Report ID, if any */                            \
    __VA_ARGS__                                        \
                                                       \
    HID_USAGE(HID_USAGE_DESKTOP_POINTER),              \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),           \
                                                       \
    /* X, Y - NO BUTTONS for BLE to avoid conflicts */ \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
    HID_USAGE(HID_USAGE_DESKTOP_X),                    \
    HID_USAGE(HID_USAGE_DESKTOP_Y),                    \
    HID_LOGICAL_MIN_N(0, 2),                           \
    HID_LOGICAL_MAX_N(0x7fff, 2),                      \
    HID_REPORT_SIZE(16),                               \
    HID_REPORT_COUNT(2),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
                                                       \
    /* Wheel [-127, 127] */                            \
    HID_USAGE(HID_USAGE_DESKTOP_WHEEL),                \
    HID_LOGICAL_MIN(0x81),                             \
    HID_LOGICAL_MAX(0x7f),                             \
    HID_REPORT_SIZE(8),                                \
    HID_REPORT_COUNT(1),                               \
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_RELATIVE), \
                                                       \
    HID_COLLECTION_END,                                \
    HID_COLLECTION_END

// BLE-specific absolute mouse report without buttons
struct BLEAbsoluteMouseReport {
  uint16_t xAxis;
  uint16_t yAxis;
  int8_t wheel;
} __attribute__((packed));

static const uint8_t report_desc[] = {
  DESCRIPTOR_BOOT_KEYBOARD(HID_REPORT_ID(RID_KEYBOARD)),
  DESCRIPTOR_MOUSE(HID_REPORT_ID(RID_MOUSE)),
  DESCRIPTOR_CONSUMER_CONTROL(HID_REPORT_ID(RID_CONSUMER_CONTROL)),
  DESCRIPTOR_SYSTEM_CONTROL(HID_REPORT_ID(RID_SYSTEM_CONTROL)),
  DESCRIPTOR_BLE_ABSOLUTE_MOUSE(HID_REPORT_ID(RID_ABS_MOUSE)),  // BLE-specific version
};

// Define the static member variables
TaskHandle_t HIDD::report_task_handle_ = nullptr;

HIDD::HIDD()
  : BLEHidGeneric(5, 1, 0), queue_handle_(nullptr) {}

err_t HIDD::begin() {
  uint16_t in_lens[] = {
    BOOT_REPORT_LEN,
    sizeof(HID_MouseReport_Data_t),
    sizeof(HID_ConsumerControlReport_Data_t),
    sizeof(HID_SystemControlReport_Data_t),
    sizeof(BLEAbsoluteMouseReport),  // Use BLE-specific size
  };
  uint16_t out_lens[] = {1};

  setHidInfo(BLE_HID_BCD, 0, BLE_HID_INFO_REMOTE_WAKE | BLE_HID_INFO_NORMALLY_CONNECTABLE);
  enableKeyboard(true);
  enableMouse(true);
  setReportLen(in_lens, out_lens, NULL);
  setReportMap(report_desc, sizeof(report_desc));

  // Initialize BLE HID service first
  err_t status = BLEHidGeneric::begin();
  if (status != ERROR_NONE) {
    return status;
  }

  // Create queue for reports
  queue_handle_ = xQueueCreateStatic(QUEUE_SIZE,
                                     sizeof(QueuedReport),
                                     queue_storage_,
                                     &queue_buffer_);
  if (!queue_handle_) {
    return NRF_ERROR_INTERNAL;
  }

  return ERROR_NONE;
}

void HIDD::setLEDcb(BLECharacteristic::write_cb_t fp) {
  setOutputReportCallback(RID_KEYBOARD, fp);
  _chr_boot_keyboard_output->setWriteCallback(fp);
}

void HIDD::end() {
  // Stop the report processing task if it's running
  stopReportProcessing();

  queue_handle_ = nullptr;  // Static queue doesn't need deletion
}

void HIDD::startReportProcessing() {
  // Bail out if we have no connection
  BLEConnection *conn = Bluefruit.Connection(0);
  if (!conn || !conn->connected()) {
    return;
  }

  DEBUG_BLE_MSG("Starting report processing");

  // Only create the task if it doesn't exist
  if (report_task_handle_ == nullptr) {
    BaseType_t task_created = xTaskCreate(
      processReportQueue_,
      "HID_Reports",
      configMINIMAL_STACK_SIZE * 2,
      this,
      2,  // Priority - higher than main loop, lower than BLE stack
      &report_task_handle_);

    if (task_created != pdPASS) {
      DEBUG_BLE_MSG("Failed to create report processing task");
      return;
    }

    DEBUG_BLE_MSG("Report processing task created");
  } else {
    DEBUG_BLE_MSG("Report processing task already running");
    // Notify the task in case it's waiting
    xTaskNotifyGive(report_task_handle_);
    DEBUG_BLE_MSG("Report processing task notified");
  }
}

void HIDD::stopReportProcessing() {
  DEBUG_BLE_MSG("Stopping report processing");

  // Only delete the task if it exists
  if (report_task_handle_ != nullptr) {
    // Notify task to wake it up if it's waiting
    xTaskNotifyGive(report_task_handle_);

    // Give the task some time to wake up and finish any processing
    // This ensures the task isn't deleted while it's in the middle of processing a report
    for (int i = 0; i < 10; i++) {
      // Check if there are still reports in the queue
      if (uxQueueMessagesWaiting(queue_handle_) == 0) {
        break;  // No more reports to process, we can delete the task
      }

      // Wait a bit for the task to process remaining reports
      DEBUG_BLE_MSG("Waiting for task to finish processing reports");
      vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Delete the task
    vTaskDelete(report_task_handle_);
    report_task_handle_ = nullptr;

    DEBUG_BLE_MSG("Report processing task deleted");
  } else {
    DEBUG_BLE_MSG("Report processing task not running");
  }
}

void HIDD::prepareForSleep() {
  // Process all queued reports, then shut down processing
  while (hasQueuedReports()) {
    delay(1);
  }
  stopReportProcessing();
}

void HIDD::clearReportQueue() {
  if (queue_handle_) {
    DEBUG_BLE_MSG("Clearing report queue");
    xQueueReset(queue_handle_);
  }
}

bool HIDD::hasQueuedReports() const {
  return queue_handle_ && (uxQueueMessagesWaiting(queue_handle_) > 0);
}

void HIDD::processReportQueue_(void *pvParameters) {
  HIDD *hidd = static_cast<HIDD *>(pvParameters);

  DEBUG_BLE_MSG("Starting report processing task");

  while (true) {
    // First, process any reports that are already in the queue
    bool processed_any = false;

    while (uxQueueMessagesWaiting(hidd->queue_handle_) > 0) {
      processed_any = true;

      // Try to process the next report
      if (!hidd->processNextReport_()) {
        // Failed to send - wait a bit before next retry
        DEBUG_BLE_MSG("Failed to send report, waiting %dms before next retry", RETRY_DELAY_MS);
        vTaskDelay(pdMS_TO_TICKS(RETRY_DELAY_MS));
      } else {
        // Successfully sent a report
        DEBUG_BLE_MSG("Sent report successfully, waiting %dms before next report", KEYSTROKE_INTERVAL_MS);
        vTaskDelay(pdMS_TO_TICKS(KEYSTROKE_INTERVAL_MS));
      }

      // Continue the loop to process more reports or retry
      if (processed_any) {
        continue;
      }
    }

    // Use a critical section to double-check the queue and prepare for sleep
    taskENTER_CRITICAL();

    if (uxQueueMessagesWaiting(hidd->queue_handle_) == 0) {
      // No reports to process, prepare for long sleep
      taskEXIT_CRITICAL();

      // Wait for notification with a long timeout for power saving
      ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(30000));  // 30 second timeout
    } else {
      // There are reports to process
      taskEXIT_CRITICAL();
      // Continue immediately to process them
    }
  }

  vTaskDelete(nullptr);  // Clean exit path if we ever break out of the loop
}

bool HIDD::processNextReport_() {
  // Check connection before attempting send
  BLEConnection *conn = Bluefruit.Connection(0);
  if (!conn || !conn->connected()) {
    return false;
  }

  QueuedReport report;
  // Peek at the next report without removing it
  if (xQueuePeek(queue_handle_, &report, 0) != pdTRUE) {
    return true;  // Queue is empty
  }

  bool success = false;
  switch (report.type) {
  case ReportType::BootKeyboard:
    success = BLEHidGeneric::bootKeyboardReport(report.data, report.length);
    break;
  case ReportType::BootMouse:
    success = BLEHidGeneric::bootMouseReport(report.data, report.length);
    break;
  case ReportType::Input:
    success = BLEHidGeneric::inputReport(report.report_id, report.data, report.length);
    break;
  }

  if (success) {
    // Only remove the item if send was successful
    xQueueReceive(queue_handle_, &report, 0);

    return true;
  } else if (report.retries_left > 0) {
    // Update retry count - we need to remove and re-add the item
    // because FreeRTOS doesn't support in-place updates for multi-item queues
    report.retries_left--;

    // Remove the old item
    QueuedReport dummy;
    xQueueReceive(queue_handle_, &dummy, 0);

    // Add it back with updated retry count at the front of the queue
    if (xQueueSendToFront(queue_handle_, &report, 0) != pdTRUE) {
      // If we can't re-queue, treat it as a failure
      DEBUG_BLE_MSG("Failed to re-queue report for retry");
      return true;
    }

    DEBUG_BLE_MSG("Retrying report, %d retries left", report.retries_left);
    return false;  // Signal failure so we'll wait before next retry
  } else {
    // Out of retries, remove the failed item
    DEBUG_BLE_MSG("Failed to send report, removing from queue");
    xQueueReceive(queue_handle_, &report, 0);
    return true;
  }
}

bool HIDD::queueReport_(ReportType type, uint8_t report_id, const void *data, uint8_t length) {
  if (!queue_handle_) return false;

  // Try to queue the report for 5 seconds before considering eviction
  const TickType_t QUEUE_RETRY_TIMEOUT = pdMS_TO_TICKS(5000);  // 5 seconds in ticks
  TickType_t start_time                = xTaskGetTickCount();

  // Keep trying to queue while there's no space and we haven't exceeded our timeout
  while (uxQueueSpacesAvailable(queue_handle_) == 0) {
    // Calculate remaining time in our retry window
    TickType_t now     = xTaskGetTickCount();
    TickType_t elapsed = now - start_time;

    // If we've exceeded our 5-second retry window, break out to try eviction
    if (elapsed >= QUEUE_RETRY_TIMEOUT) {
      DEBUG_BLE_MSG("Failed to queue report, exceeded retry window");
      break;
    }

    // Wait a short time before checking again
    // Use shorter delays early in the retry window, longer ones later
    TickType_t delay_time = (elapsed < pdMS_TO_TICKS(1000)) ? pdMS_TO_TICKS(1) :  // 1ms delays in first second
                              pdMS_TO_TICKS(10);                                  // 10ms delays after that
    vTaskDelay(delay_time);
  }

  // If queue is still full after retrying, we need to evict the oldest report
  if (uxQueueSpacesAvailable(queue_handle_) == 0) {
    // Log or track this eviction if needed
    QueuedReport dummy;
    if (xQueueReceive(queue_handle_, &dummy, 0) != pdTRUE) {
      // If we somehow failed to remove an item, we can't queue the new one
      return false;
    }
  }

  // Prepare the report structure for queuing
  QueuedReport report;
  report.type      = type;
  report.report_id = report_id;
  memcpy(report.data, data, length);
  report.length       = length;
  report.retries_left = MAX_BLE_NOTIFY_RETRIES;

  // Queue the new report
  BaseType_t success = xQueueSend(queue_handle_, &report, 0);

  if (success == pdTRUE) {

    // Ensure the report processing task is running
    startReportProcessing();
  }

  return success == pdTRUE;
}

bool HIDD::sendBootKeyboardReport(const void *data, uint8_t length) {
  return queueReport_(ReportType::BootKeyboard, 0, data, length);
}

bool HIDD::sendBootMouseReport(const void *data, uint8_t length) {
  return queueReport_(ReportType::BootMouse, 0, data, length);
}

bool HIDD::sendInputReport(uint8_t report_id, const void *data, uint8_t length) {
  return queueReport_(ReportType::Input, report_id, data, length);
}

HIDD blehid;

}  // namespace bluefruit
}  // namespace hid
}  // namespace driver
}  // namespace kaleidoscope

#endif /* ARDUINO_ARCH_NRF52 */
