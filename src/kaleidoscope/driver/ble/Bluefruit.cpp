/* Kaleidoscope - Firmware for computer input devices
 * Copyright (C) 2024-2025 Keyboard.io, inc.
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

#ifdef ARDUINO_ARCH_NRF52

#include "kaleidoscope/driver/ble/Bluefruit.h"  // for Base
#include "kaleidoscope/Runtime.h"
#include "kaleidoscope/hooks.h"
#include "kaleidoscope/host_connection_status.h"
#include "kaleidoscope/KeyEvent.h"
#include "kaleidoscope/key_defs/ble.h"
#include "kaleidoscope/keyswitch_state.h"
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "utility/bonding.h"

namespace kaleidoscope {
namespace driver {
namespace ble {

uint8_t BLEBluefruit::current_device_id = 0;
uint8_t BLEBluefruit::battery_level     = 0;
int8_t BLEBluefruit::pre_sleep_tx_power = CONN_TX_POWER;

ble_gap_addr_t BLEBluefruit::base_addr;

BLEDis BLEBluefruit::bledis;
BLEBas BLEBluefruit::blebas;
BLEUartWrapper BLEBluefruit::bleuart;

// Add static variable to track sleep start time
static uint32_t sleep_start_time = 0;

volatile uint32_t BLEBluefruit::bond_write_deadline_ms = 0;
volatile bool BLEBluefruit::bond_write_pending         = false;

/**
 * @brief Prepare BLE for sleep mode to conserve power
 * 
 * This method:
 * 1. Disables advertising auto restart on disconnect
 * 2. If not currently connected, reduces TX power to minimum level
 * 
 * These changes help reduce power consumption during deep sleep.
 */
void BLEBluefruit::prepareForSleep() {
  DEBUG_BLE_MSG("Preparing BLE for sleep");

  // Record when we're entering sleep
  sleep_start_time = millis();

  // Disable advertising auto restart
  Bluefruit.Advertising.restartOnDisconnect(false);

  // If not connected, reduce TX power to minimum
  if (!Bluefruit.Periph.connected()) {
    // Store current TX power before changing it
    pre_sleep_tx_power = Bluefruit.getTxPower();

    // Set to lowest power level
    Bluefruit.setTxPower(SLEEP_TX_POWER);
    DEBUG_BLE_MSG("Reduced TX power to minimum for sleep: ", SLEEP_TX_POWER, "dBm");
  }
}

/**
 * @brief Restore BLE after waking from sleep
 * 
 * This method:
 * 1. Re-enables advertising auto restart on disconnect
 * 2. Restores previous TX power level
 * 3. If not connected, restarts connectable advertising
 * 4. Forces advertising restart if we've been asleep for extended period
 */
void BLEBluefruit::restoreAfterSleep() {
  DEBUG_BLE_MSG("Restoring BLE after sleep");

  Bluefruit.Advertising.restartOnDisconnect(true);

  if (Bluefruit.getTxPower() != pre_sleep_tx_power) {
    Bluefruit.setTxPower(pre_sleep_tx_power);
    DEBUG_BLE_MSG("Restored TX power to: ", pre_sleep_tx_power, "dBm");
  }

  bool connected           = Bluefruit.Periph.connected();
  bool advertising         = Bluefruit.Advertising.isRunning();
  uint32_t sleep_duration  = millis() - sleep_start_time;
  bool connection_is_stale = false;

  // If we think we're connected but slept for a long time, validate the connection
  if (connected && sleep_duration > 900000) {  // 15 minutes
    DEBUG_BLE_MSG("Long sleep detected, validating connection...");

    // Try to get connection object and validate it
    if (Bluefruit.connected()) {
      uint16_t conn_handle = Bluefruit.connHandle();
      BLEConnection *conn  = Bluefruit.Connection(conn_handle);

      if (conn && conn->connected()) {
        // Try to get RSSI - this will fail if connection is stale
        int8_t rssi = conn->getRssi();

        // RSSI of 127 often indicates invalid/stale connection
        // RSSI worse than -90 dBm likely indicates connection issues
        if (rssi == 127 || rssi < -90) {
          DEBUG_BLE_MSG("Connection validation failed: invalid RSSI=", rssi);
          connection_is_stale = true;
        } else {
          // Try to get connection parameters - if this fails, connection is likely dead
          uint16_t interval = conn->getConnectionInterval();
          if (interval == 0 || interval > 1000) {  // Sanity check on interval
            DEBUG_BLE_MSG("Connection validation failed: invalid interval=", interval);
            connection_is_stale = true;
          } else {
            DEBUG_BLE_MSG("Connection validated: RSSI=", rssi, "dBm, interval=", interval);
          }
        }
      } else {
        DEBUG_BLE_MSG("Connection validation failed: could not get connection object");
        connection_is_stale = true;
      }
    } else {
      DEBUG_BLE_MSG("Connection validation failed: Bluefruit.connected() returned false");
      connection_is_stale = true;
    }
  }

  bool force_advertising = connection_is_stale;

  DEBUG_BLE_MSG("After sleep: connected=", connected, ", advertising=", advertising, ", sleep_duration=", sleep_duration, "ms, stale=", connection_is_stale);

  if ((!connected && current_device_id > 0) || force_advertising) {
    if (force_advertising && connected) {
      DEBUG_BLE_MSG("Forcing disconnect due to stale connection");
      // Disconnect the stale connection
      if (Bluefruit.connected()) {
        uint16_t conn_handle = Bluefruit.connHandle();
        DEBUG_BLE_MSG("Disconnecting stale handle: ", conn_handle);
        Bluefruit.disconnect(conn_handle);
      }
      delay(100);  // Allow disconnect to complete
    }

    DEBUG_BLE_MSG("Attempting to start advertising after sleep...");
    startConnectableAdvertising();
    if (!Bluefruit.Advertising.isRunning()) {
      DEBUG_BLE_MSG("First advertising attempt failed, forcing cleanup and retrying...");
      Bluefruit.Advertising.stop();
      if (Bluefruit.connected()) {
        uint16_t conn_handle = Bluefruit.connHandle();
        DEBUG_BLE_MSG("Disconnecting handle: ", conn_handle);
        Bluefruit.disconnect(conn_handle);
      }
      delay(100);
      startConnectableAdvertising();
      if (!Bluefruit.Advertising.isRunning()) {
        DEBUG_BLE_MSG("Second advertising attempt also failed. Giving up for now.");
      } else {
        DEBUG_BLE_MSG("Advertising started successfully on second attempt.");
      }
    } else {
      DEBUG_BLE_MSG("Advertising started successfully on first attempt.");
    }
  }
}

void BLEBluefruit::setup() {

  // Configure MTU and queue sizes. This should happen before 'begin'
  Bluefruit.configPrphConn(MTU_SIZE,               // MTU size
                           EVENT_LENGTH,           // Event length
                           HVN_QUEUE_SIZE,         // HVN queue size
                           WRITE_CMD_QUEUE_SIZE);  // Write CMD queue size

  //  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  if (!Bluefruit.begin()) {
    DEBUG_BLE_MSG("Failed to initialize Bluefruit");
    return;
  }

  // Configure connection parameters for low latency using constants from header
  Bluefruit.Periph.setConnInterval(CONN_INTERVAL_MIN_MS, CONN_INTERVAL_MAX_MS);
  Bluefruit.Periph.setConnSlaveLatency(SLAVE_LATENCY);
  Bluefruit.Periph.setConnSupervisionTimeout(SUPERVISION_TIMEOUT_MS);

  // Enable DC/DC regulator for better power efficiency
  sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);

  // Configure low power mode - keeping CONSTLAT for now
  sd_power_mode_set(NRF_POWER_MODE_CONSTLAT);

  // Set lower TX power - still good for 10m range
  Bluefruit.setTxPower(CONN_TX_POWER);

  // Disable Bluefruit's built in LED support since it's designed for a single-color LED, not our RGB LEDs.
  // TODO(anybody): make this configurable down the line.
  Bluefruit.autoConnLed(false);
  Bluefruit.setConnLedInterval(0);

  Bluefruit.Periph.setConnectCallback(BLEBluefruit::connect_cb);
  Bluefruit.Periph.setDisconnectCallback(BLEBluefruit::disconnect_cb);
  Bluefruit.Security.setSecuredCallback(BLEBluefruit::secured_cb);
  Bluefruit.Security.setPairCompleteCallback(BLEBluefruit::pairing_complete_cb);

  Bluefruit.setAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.setName(USB_PRODUCT);

  // Configure Device Information Service
  bledis.setPermission(SECMODE_ENC_NO_MITM, SECMODE_NO_ACCESS);
  bledis.setManufacturer(USB_MANUFACTURER);
  bledis.setModel(USB_PRODUCT);
  if (!bledis.begin()) {
    DEBUG_BLE_MSG("Failed to start Device Information Service");
  }

  // Configure Battery Service
  if (!blebas.begin()) {
    DEBUG_BLE_MSG("Failed to start Battery Service");
  }

  // Configure UART Service
  bleuart.setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
  if (!bleuart.begin()) {
    DEBUG_BLE_MSG("Failed to start UART Service");
  }

  Bluefruit.Advertising.setStopCallback(stop_advertising_cb);

  // Cache the device's base address during setup
  sd_ble_gap_addr_get(&base_addr);
}

void BLEBluefruit::startDiscoverableAdvertising() {
  DEBUG_BLE_MSG("Starting limited discoverable advertising");
  if (current_device_id == 0) {
    DEBUG_BLE_MSG("ERROR: No device slot selected");
    return;
  }

  DEBUG_BLE_MSG("Checking connection state...");
  if (Bluefruit.Periph.connected()) {
    DEBUG_BLE_MSG("Already connected.");
    if (Bluefruit.connected()) {
      DEBUG_BLE_MSG("Disconnecting current connection and removing bond to peer address so we can connect a new device to this slot");
      disconnectAndUnbond();
    }
  }

  stopAdvertising();
  configureAdvertising();

  DEBUG_BLE_MSG("Setting up new advertising data...");
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Hooks::onHostConnectionStatusChanged(current_device_id, kaleidoscope::HostConnectionStatus::Advertising);

  // Start advertising
  if (!Bluefruit.Advertising.start(ADVERTISING_TIMEOUT)) {

    DEBUG_BLE_MSG("Failed to start advertising");
  }
  DEBUG_BLE_MSG("Started connectable advertising");
}

bool BLEBluefruit::connected() {
  return Bluefruit.Periph.connected();
}

Stream &BLEBluefruit::serialPort() {
  return bleuart;
}

void BLEBluefruit::setBatteryLevel(uint8_t level) {
  if (level > 100) level = 100;
  battery_level = level;
  blebas.notify(level);
}

uint8_t BLEBluefruit::getBatteryLevel() {
  return battery_level;
}

void BLEBluefruit::selectDevice(uint8_t device_id) {
  if (device_id == 0) {
    DEBUG_BLE_MSG("ERROR: No device slot selected");
    return;
  }
  if (device_id != current_device_id) {
    if (Bluefruit.Periph.connected()) {
      DEBUG_BLE_MSG("Disconnecting current connection");
      disconnect();
      // Wait for disconnect to complete before changing device
      delay(100);
    } else {
      DEBUG_BLE_MSG("Device not connected, skipping disconnect");
    }
    stopAdvertising();
    Hooks::onHostConnectionStatusChanged(current_device_id, kaleidoscope::HostConnectionStatus::DeviceUnselected);

    DEBUG_BLE_MSG("Updating current_device_id: ", current_device_id, " -> ", device_id);

    char name_buffer[48];
    snprintf(name_buffer, sizeof(name_buffer), "%s (#%d)", USB_PRODUCT, device_id);
    Bluefruit.setName(name_buffer);
    Hooks::onHostConnectionStatusChanged(device_id, kaleidoscope::HostConnectionStatus::DeviceSelected);
    current_device_id = device_id;
  }
  startConnectableAdvertising();

  DEBUG_BLE_MSG(" ====== Device Selection Complete ======\n");
}

void BLEBluefruit::stopAdvertising() {
  if (Bluefruit.Advertising.isRunning()) {
    Bluefruit.Advertising.stop();
  }
}

bool BLEBluefruit::getConnectionSecurity(uint16_t conn_handle, ble_gap_conn_sec_t &sec) {
  if (sd_ble_gap_conn_sec_get(conn_handle, &sec) != NRF_SUCCESS) {
    DEBUG_BLE_MSG("Failed to get security status");
    return false;
  }
  return true;
}


void BLEBluefruit::stop_advertising_cb() {
  DEBUG_BLE_MSG("stop_advertising_cb: Advertising stopped");
}

void BLEBluefruit::connect_cb(uint16_t conn_handle) {
  BLEConnection *conn = Bluefruit.Connection(conn_handle);
  if (!conn) {
    DEBUG_BLE_MSG("ERROR: Could not get connection object");
    return;
  }

  DEBUG_BLE_MSG("Connection established, handle: ", conn_handle);

  // Check if this is a reconnection to a bonded device
  bool is_bonded = conn->bonded();
  DEBUG_BLE_MSG("Bonding status check: ", is_bonded ? "BONDED" : "NOT BONDED");

  // Get peer address for bonding checks
  ble_gap_addr_t peer_addr = conn->getPeerAddr();

  // Check bonding state more thoroughly
  DEBUG_BLE_MSG("Peer address: ",
                peer_addr.addr[5],
                ":",
                peer_addr.addr[4],
                ":",
                peer_addr.addr[3],
                ":",
                peer_addr.addr[2],
                ":",
                peer_addr.addr[1],
                ":",
                peer_addr.addr[0]);

  if (is_bonded) {
    DEBUG_BLE_MSG("*** RECONNECTION TO BONDED DEVICE DETECTED ***");
    DEBUG_BLE_MSG("Bonding state consistent - connection reports as bonded");
  } else {
    DEBUG_BLE_MSG("*** NEW CONNECTION TO UNBONDED DEVICE ***");
    DEBUG_BLE_MSG("Bonding state consistent - connection reports as not bonded");
  }

  // Log connection details
  DEBUG_BLE_MSG("Peer address: ",
                peer_addr.addr[5],
                ":",
                peer_addr.addr[4],
                ":",
                peer_addr.addr[3],
                ":",
                peer_addr.addr[2],
                ":",
                peer_addr.addr[1],
                ":",
                peer_addr.addr[0]);

  // Log initial connection parameters
  uint16_t interval = conn->getConnectionInterval();
  uint16_t latency  = conn->getSlaveLatency();
  uint16_t timeout  = conn->getSupervisionTimeout();
  DEBUG_BLE_MSG("Initial connection parameters - Interval: ", (int)(interval * 1.25f), "ms, Latency: ", latency, ", Timeout: ", (timeout * 10), "ms");

  // Log additional connection characteristics that might vary between devices
  int8_t rssi = conn->getRssi();
  DEBUG_BLE_MSG("Initial RSSI: ", rssi, " dBm");

  uint16_t mtu = conn->getMtu();
  DEBUG_BLE_MSG("Initial MTU: ", mtu, " bytes");

  uint16_t data_length = conn->getDataLength();
  DEBUG_BLE_MSG("Initial Data Length: ", data_length, " bytes");

  // Log timing information
  static uint32_t last_connect_time = 0;
  uint32_t now                      = millis();
  uint32_t time_since_last_connect  = now - last_connect_time;
  DEBUG_BLE_MSG("Time since last connect: ", time_since_last_connect, "ms");
  last_connect_time = now;

  // Log device-specific information
  DEBUG_BLE_MSG("Current device slot: ", current_device_id);
  DEBUG_BLE_MSG("Current BLE address: ",
                Bluefruit.getAddr().addr[5],
                ":",
                Bluefruit.getAddr().addr[4],
                ":",
                Bluefruit.getAddr().addr[3],
                ":",
                Bluefruit.getAddr().addr[2],
                ":",
                Bluefruit.getAddr().addr[1],
                ":",
                Bluefruit.getAddr().addr[0]);

  // Log power and voltage information if available
  DEBUG_BLE_MSG("TX Power: ", Bluefruit.getTxPower(), " dBm");

  // Check if this is a reconnection attempt
  static uint8_t connection_count = 0;
  connection_count++;
  DEBUG_BLE_MSG("Connection attempt #", connection_count);

  // Track connection start time for duration measurement
  static uint32_t connect_start_time = 0;
  connect_start_time                 = millis();
  DEBUG_BLE_MSG("Connection start time recorded");

  Bluefruit.Security.setMITM(true);
}

void BLEBluefruit::secured_cb(uint16_t conn_handle) {
  DEBUG_BLE_MSG("secured_cb called for handle: ", conn_handle);

  BLEConnection *conn = Bluefruit.Connection(conn_handle);
  if (!conn) {
    DEBUG_BLE_MSG("ERROR: Could not get connection object");
    return;
  }

  // Are we bonded?
  bool is_bonded = conn->bonded();
  if (is_bonded) {
    DEBUG_BLE_MSG("Connection reports as already bonded - being conservative with reconnection");

    // For bonded connections, be very conservative about any changes
    // The host already knows this device, so don't rock the boat
    DEBUG_BLE_MSG("Bonded connection detected - minimizing parameter changes");

    // Don't request any connection parameter changes for bonded connections
    // Let the host use whatever parameters it prefers
    DEBUG_BLE_MSG("Skipping connection parameter requests for bonded connection");

  } else {
    DEBUG_BLE_MSG("Connection not bonded, checking if we should request pairing");

    // Request pairing at most every 2 s to avoid spamming the host
    static uint32_t last_pairing_request = 0;
    uint32_t now                         = millis();
    if ((now - last_pairing_request) > 2000) {
      DEBUG_BLE_MSG("Requesting pairing after delay");
      last_pairing_request = now;
      conn->requestPairing();
    } else {
      DEBUG_BLE_MSG("Skipping pairing request, too soon since last attempt");
    }
  }

  // Get actual connection parameters
  uint16_t actual_interval = conn->getConnectionInterval();
  uint16_t actual_latency  = conn->getSlaveLatency();
  uint16_t actual_timeout  = conn->getSupervisionTimeout();

  DEBUG_BLE_MSG("Connection parameters in secured_cb:");
  char msg[64];
  snprintf(msg, sizeof(msg), "Interval: %dms", static_cast<int>(actual_interval * 1.25f));  // Convert from units to ms
  DEBUG_BLE_MSG(msg);
  snprintf(msg, sizeof(msg), "Latency: %d", actual_latency);
  DEBUG_BLE_MSG(msg);
  snprintf(msg, sizeof(msg), "Timeout: %dms", actual_timeout * 10);  // Convert to ms
  DEBUG_BLE_MSG(msg);

  // For bonded connections, be very conservative about parameter changes
  // Only request updates if parameters are clearly problematic
  bool needs_update = false;

  if (is_bonded) {
    // For bonded connections, be extremely conservative
    // Only update if parameters are clearly broken
    if ((actual_interval * 1.25f) > 200) {  // Only if interval > 250ms (very high)
      DEBUG_BLE_MSG("Bonded connection: interval extremely high, requesting update");
      needs_update = true;
    } else if (actual_timeout < 50) {  // Only if timeout < 500ms (very short)
      DEBUG_BLE_MSG("Bonded connection: timeout extremely short, requesting update");
      needs_update = true;
    } else {
      DEBUG_BLE_MSG("Bonded connection: parameters acceptable, no update needed");
    }
  } else {
    // For new connections, be more aggressive about optimization
    if ((actual_interval * 1.25f) > (CONN_INTERVAL_MAX_MS * 1.5f)) {
      DEBUG_BLE_MSG("New connection: interval too high, requesting update");
      needs_update = true;
    }

    if (actual_latency > (SLAVE_LATENCY * 2)) {
      DEBUG_BLE_MSG("New connection: latency too high, requesting update");
      needs_update = true;
    }

    if (actual_timeout < (SUPERVISION_TIMEOUT_MS / 10)) {
      DEBUG_BLE_MSG("New connection: timeout too short, requesting update");
      needs_update = true;
    }
  }

  if (needs_update) {
    DEBUG_BLE_MSG("Requesting connection parameter update...");
    conn->requestConnectionParameter(CONN_INTERVAL_MIN_MS, SLAVE_LATENCY, SUPERVISION_TIMEOUT_MS);
  } else {
    DEBUG_BLE_MSG("Connection parameters acceptable, no update needed");
  }

  // Get and log connection status and errors
  int8_t rssi = conn->getRssi();
  snprintf(msg, sizeof(msg), "RSSI: %d dBm", rssi);
  DEBUG_BLE_MSG(msg);

  uint16_t mtu = conn->getMtu();
  snprintf(msg, sizeof(msg), "MTU: %d bytes", mtu);
  DEBUG_BLE_MSG(msg);

  uint16_t data_length = conn->getDataLength();
  snprintf(msg, sizeof(msg), "Data Length: %d bytes", data_length);
  DEBUG_BLE_MSG(msg);

  snprintf(msg, sizeof(msg), "Connected. Handle: %d", conn_handle);
  DEBUG_BLE_MSG(msg);

  DEBUG_BLE_MSG("Setting host connection mode to BLE");
  kaleidoscope::Runtime.device().setHostConnectionMode(MODE_BLE);

  DEBUG_BLE_MSG("Calling onHostConnectionStatusChanged with Connected status");
  Hooks::onHostConnectionStatusChanged(current_device_id, kaleidoscope::HostConnectionStatus::Connected);

  DEBUG_BLE_MSG("Clearing HID report queue");
  kaleidoscope::driver::hid::bluefruit::blehid.clearReportQueue();

  DEBUG_BLE_MSG("Starting HID report processing");
  kaleidoscope::driver::hid::bluefruit::blehid.startReportProcessing();

  DEBUG_BLE_MSG("secured_cb completed successfully");
}

void BLEBluefruit::pairing_complete_cb(uint16_t conn_handle, uint8_t auth_status) {
  DEBUG_BLE_MSG("pairing_complete_cb called for handle: ", conn_handle, " with auth_status: ", auth_status);

  // Send pairing status before trying to get connection object
  Hooks::onHostConnectionStatusChanged(current_device_id,
                                       auth_status == 0 ? kaleidoscope::HostConnectionStatus::PairingSuccess
                                                        : kaleidoscope::HostConnectionStatus::PairingFailed);

  if (auth_status != 0) {
    DEBUG_BLE_MSG("Pairing failed with auth_status: ", auth_status);
    return;
  }

  // Only try to get connection object if we're still connected
  if (Bluefruit.Periph.connected()) {
    BLEConnection *conn = Bluefruit.Connection(conn_handle);
    if (!conn) {
      DEBUG_BLE_MSG("ERROR: Could not get connection object");
      return;
    }

    // Check bonding status after successful pairing
    bool is_bonded = conn->bonded();
    DEBUG_BLE_MSG("After pairing: bonding status = ", is_bonded ? "BONDED" : "NOT BONDED");

    if (is_bonded) {
      DEBUG_BLE_MSG("Bonding information successfully stored");

      // Log bonding details
      ble_gap_addr_t peer_addr = conn->getPeerAddr();
      DEBUG_BLE_MSG("Bonded peer address: ",
                    peer_addr.addr[5],
                    ":",
                    peer_addr.addr[4],
                    ":",
                    peer_addr.addr[3],
                    ":",
                    peer_addr.addr[2],
                    ":",
                    peer_addr.addr[1],
                    ":",
                    peer_addr.addr[0]);

      DEBUG_BLE_MSG("Bonding completed successfully");

      bond_write_pending     = true;
      bond_write_deadline_ms = millis() + 1500;
    } else {
      DEBUG_BLE_MSG("WARNING: Pairing succeeded but device reports as not bonded!");
      DEBUG_BLE_MSG("This might indicate a bonding storage issue");
    }

    // Connection parameters are already handled in secured_cb, no need to request again
    DEBUG_BLE_MSG("Pairing successful, connection parameters already configured");
  } else {
    DEBUG_BLE_MSG("Connection already closed when pairing completed");
  }

  DEBUG_BLE_MSG("pairing_complete_cb completed");
}

void BLEBluefruit::disconnect_cb(uint16_t conn_handle, uint8_t reason) {
  kaleidoscope::driver::hid::bluefruit::blehid.stopReportProcessing();
  DEBUG_BLE_MSG("Disconnected, reason = 0x", reason, HEX);

  // Log timing information for disconnect
  static uint32_t last_disconnect_time = 0;
  uint32_t now                         = millis();
  uint32_t time_since_last_disconnect  = now - last_disconnect_time;
  DEBUG_BLE_MSG("Time since last disconnect: ", time_since_last_disconnect, "ms");
  last_disconnect_time = now;

  // Log connection duration if we can track it
  static uint32_t connect_start_time = 0;
  if (connect_start_time > 0) {
    uint32_t connection_duration = now - connect_start_time;
    DEBUG_BLE_MSG("Connection duration: ", connection_duration, "ms");
    connect_start_time = 0;  // Reset for next connection
  }

  // Track disconnect patterns
  static uint8_t disconnect_count         = 0;
  static uint8_t remote_termination_count = 0;
  disconnect_count++;

  if (reason == BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION) {
    remote_termination_count++;
    DEBUG_BLE_MSG("Remote termination count: ", remote_termination_count, " out of ", disconnect_count, " total disconnects");
  }

  DEBUG_BLE_MSG("Total disconnect count: ", disconnect_count);

  // Log the reason code in a more readable format
  const char *reason_str = "Unknown";
  switch (reason) {
  case BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION:
    reason_str = "Remote User Terminated";
    break;
  case BLE_HCI_LOCAL_HOST_TERMINATED_CONNECTION:
    reason_str = "Local Host Terminated";
    break;
  case BLE_HCI_REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF:
    reason_str = "Remote Device Power Off";
    break;
  case BLE_HCI_CONNECTION_TIMEOUT:
    reason_str = "Connection Timeout";
    break;
  case BLE_HCI_CONN_FAILED_TO_BE_ESTABLISHED:
    reason_str = "Connection Failed to Establish";
    break;
  case BLE_HCI_UNSUPPORTED_REMOTE_FEATURE:
    reason_str = "Unsupported Remote Feature";
    break;
  case BLE_HCI_PAIRING_WITH_UNIT_KEY_UNSUPPORTED:
    reason_str = "Pairing with Unit Key Not Supported";
    break;
  case BLE_HCI_INSTANT_PASSED:
    reason_str = "Instant Passed";
    break;
  case BLE_HCI_DIFFERENT_TRANSACTION_COLLISION:
    reason_str = "Different Transaction Collision";
    break;
  case BLE_HCI_STATUS_CODE_LMP_PDU_NOT_ALLOWED:
    reason_str = "LMP PDU Not Allowed";
    break;
  case BLE_HCI_DIRECTED_ADVERTISER_TIMEOUT:
    reason_str = "Directed Advertising Timeout";
    break;
  case BLE_HCI_CONN_TERMINATED_DUE_TO_MIC_FAILURE:
    reason_str = "Connection Terminated Due to MIC Failure";
    break;
  default:
    reason_str = "Unknown Error Code";
    break;
  }
  DEBUG_BLE_MSG("Disconnect reason: ", reason_str, " (0x", reason, HEX, ")");

  // Error 0x516 indicates advertising failed - this can happen during rapid state changes
  // We track consecutive failures to prevent getting stuck in a retry loop
  static uint8_t failure_count      = 0;
  static uint32_t last_failure_time = 0;

  if (reason == 0x516) {
    DEBUG_BLE_MSG("Nordic error 0x516: Advertising procedure failed");

    // Reset failure count if more than 5 seconds have passed
    uint32_t now = millis();
    if ((now - last_failure_time) > 5000) {
      failure_count = 0;
    }
    last_failure_time = now;

    if (++failure_count >= 3) {
      DEBUG_BLE_MSG("Multiple advertising failures detected, stopping retry loop");
      failure_count = 0;
      Bluefruit.Advertising.stop();
      delay(500);
      return;
    }
  }

  switch (reason) {
  case BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION:
    DEBUG_BLE_MSG("Remote device terminated connection");

    // For devices that have multiple remote terminations, use a more conservative approach
    static uint8_t consecutive_remote_terminations = 0;
    consecutive_remote_terminations++;

    if (consecutive_remote_terminations >= 2) {
      DEBUG_BLE_MSG("Multiple consecutive remote terminations detected, using conservative reconnection");
      delay(500);  // Longer delay for problematic devices

      // Reset advertising parameters to be more conservative
      Bluefruit.Advertising.setInterval(80, 160);  // Slower advertising
      Bluefruit.Advertising.setFastTimeout(10);    // Shorter fast mode
    } else {
      delay(100);  // Normal delay
    }

    startDiscoverableAdvertising();
    break;

  case BLE_HCI_LOCAL_HOST_TERMINATED_CONNECTION:
    DEBUG_BLE_MSG("Local device terminated connection. Don't assume we should do something.");
    consecutive_remote_terminations = 0;  // Reset counter on local termination
    break;

  case BLE_HCI_REMOTE_DEV_TERMINATION_DUE_TO_POWER_OFF:
  case BLE_HCI_CONNECTION_TIMEOUT:
  default:
    DEBUG_BLE_MSG("Attempting to reconnect to last device");
    consecutive_remote_terminations = 0;  // Reset counter on other disconnect types

    if (current_device_id > 0) {
      delay(250);  // Give time for cleanup
      startConnectableAdvertising();
    }
    break;
  }

  Hooks::onHostConnectionStatusChanged(current_device_id, kaleidoscope::HostConnectionStatus::Disconnected);
}

void BLEBluefruit::startConnectableAdvertising() {
  stopAdvertising();
  configureAdvertising();
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE);

  DEBUG_BLE_MSG("Attempting to start advertising...");
  if (!Bluefruit.Advertising.start(ADVERTISING_TIMEOUT)) {
    DEBUG_BLE_MSG("Failed to start advertising");
  }
  DEBUG_BLE_MSG("Started connectable advertising");
  Hooks::onHostConnectionStatusChanged(current_device_id, kaleidoscope::HostConnectionStatus::Advertising);
}

void BLEBluefruit::printBLEAddress(const char *prefix, const uint8_t *addr) {
#if DEBUG_BLE
  Serial.print(prefix);

  for (int i = 5; i >= 0; i--) {
    Serial.print(addr[i], HEX);
    if (i > 0) Serial.print(":");
  }
  Serial.println("");
#endif  // DEBUG_BLE
}

void BLEBluefruit::configureAdvertising() {
  // Reset advertising state
  Bluefruit.Advertising.setType(BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED);
  Bluefruit.Advertising.clearData();
  Bluefruit.ScanResponse.clearData();

  setSlotSpecificAddress(current_device_id);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.Advertising.addService(hid::bluefruit::blehid);
  Bluefruit.Advertising.addName();
  Bluefruit.ScanResponse.addService(bleuart);

  // Configure advertising timing parameters for optimal discovery and power usage
  Bluefruit.Advertising.restartOnDisconnect(true);
  // Interval between 20ms (32*0.625) and 152.5ms (244*0.625)
  Bluefruit.Advertising.setInterval(32, 244);
  // Time in fast advertising mode before switching to slow mode
  Bluefruit.Advertising.setFastTimeout(30);

  // Log current advertising configuration for debugging connection issues
  DEBUG_BLE_MSG("Advertising configured for device slot: ", current_device_id);
  printBLEAddress("Using BLE address: ", Bluefruit.getAddr().addr);
}

void BLEBluefruit::setSlotSpecificAddress(uint8_t slot_id) {
  ble_gap_addr_t slot_addr = base_addr;
  slot_addr.addr[0] += slot_id;

  Bluefruit.setAddr(&slot_addr);
}

void BLEBluefruit::disconnect() {
  // Clear HID report queue before disconnecting
  kaleidoscope::driver::hid::bluefruit::blehid.clearReportQueue();

  // Disable auto-restart of advertising
  Bluefruit.Advertising.restartOnDisconnect(false);

  // Check current connection state and disconnect if needed
  if (Bluefruit.connected()) {
    uint16_t conn_handle = Bluefruit.connHandle();
    DEBUG_BLE_MSG("Connected to handle: ", conn_handle, " - Initiating disconnect...");

    // Send a BLE-specific null keyboard report before disconnecting to prevent stuck keys
    kaleidoscope::Runtime.device().setHostConnectionMode(MODE_BLE);
    kaleidoscope::Runtime.device().hid().keyboard().releaseAllKeys();

    if (Bluefruit.disconnect(conn_handle)) {
    } else {
      DEBUG_BLE_MSG("WARNING: Disconnect request failed!");
    }
  } else {
    DEBUG_BLE_MSG("No active connection");
  }
}


void BLEBluefruit::disconnectAndUnbond() {
  unbond();
  disconnect();
}

void BLEBluefruit::unbond() {
  uint16_t conn_handle = Bluefruit.connHandle();
  BLEConnection *conn  = Bluefruit.Connection(conn_handle);
  DEBUG_BLE_MSG("Disconnecting current connection and removing bond to peer address");
  ble_gap_addr_t peer_addr = conn->getPeerAddr();
  printBLEAddress("Removing bond to: ", peer_addr.addr);
  bond_remove_key(BLE_GAP_ROLE_PERIPH, &peer_addr);
}

// Event handler for BLE-specific key events
kaleidoscope::EventHandlerResult BLEBluefruit::onKeyEvent(kaleidoscope::KeyEvent &event) {
  // Only process key toggled on events
  if (!keyToggledOn(event.state))
    return kaleidoscope::EventHandlerResult::OK;

  // Define the BLE key range using constructed Key objects
  static constexpr Key ble_key_range_start     = Key(BLE_TOGGLE, KEY_FLAGS | SYNTHETIC);
  static constexpr Key ble_key_range_end       = Key(0x8F, KEY_FLAGS | SYNTHETIC);  // Include all reserved BLE slots
  static constexpr Key ble_operation_range_end = Key(BLE_PAIR, KEY_FLAGS | SYNTHETIC);
  static constexpr Key ble_device_range_start  = Key(BLE_SELECT_DEVICE_1, KEY_FLAGS | SYNTHETIC);

  // Check if this key is in the BLE range
  if (event.key.getRaw() < ble_key_range_start.getRaw() ||
      event.key.getRaw() > ble_key_range_end.getRaw())
    return kaleidoscope::EventHandlerResult::OK;

  // Extract the BLE keycode from the raw key value
  uint8_t keyCode = event.key.getKeyCode();

  // Quick check if this is a BLE-related key based on keycode ranges
  bool is_ble_operation    = (event.key.getRaw() >= ble_key_range_start.getRaw() &&
                           event.key.getRaw() <= ble_operation_range_end.getRaw());
  bool is_device_selection = (event.key.getRaw() >= ble_device_range_start.getRaw() &&
                              event.key.getRaw() <= ble_key_range_end.getRaw());

  // Handle BLE operation keys
  if (is_ble_operation) {
    return handleBLEOperationKey(keyCode);
  }

  // Handle device selection keys
  if (is_device_selection) {
    uint8_t device_id = keyCode - BLE_SELECT_DEVICE_1 + 1;
    selectDevice(device_id);
    kaleidoscope::Runtime.device().setHostConnectionMode(MODE_BLE);
    return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;
  }

  return kaleidoscope::EventHandlerResult::OK;
}

// Helper method to handle BLE operation keys
kaleidoscope::EventHandlerResult BLEBluefruit::handleBLEOperationKey(uint8_t keyCode) {
  switch (keyCode) {
  case BLE_TOGGLE:
    DEBUG_BLE_MSG("BLE key: Toggle connection mode");
    kaleidoscope::Runtime.device().toggleHostConnectionMode();
    return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;

  case BLE_OFF:
    DEBUG_BLE_MSG("BLE key: Turn off BLE");
    stopAdvertising();
    scheduleSafeDisconnect();
    kaleidoscope::Runtime.device().setHostConnectionMode(MODE_USB);
    return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;

  case BLE_PAIR:
    DEBUG_BLE_MSG("BLE key: Start pairing");
    startDiscoverableAdvertising();
    return kaleidoscope::EventHandlerResult::EVENT_CONSUMED;

  default:
    return kaleidoscope::EventHandlerResult::OK;
  }
}

void BLEBluefruit::scheduleSafeDisconnect() {
  uint32_t now = millis();
  if (bond_write_pending && ((int32_t)(bond_write_deadline_ms - now) > 0)) {
    uint32_t wait_ms = bond_write_deadline_ms - now;
    DEBUG_BLE_MSG("Delaying disconnect %ums to allow bond write", wait_ms);
    delay(wait_ms);
    bond_write_pending = false;
  }
  disconnect();
}

}  // namespace ble
}  // namespace driver
}  // namespace kaleidoscope

#endif
