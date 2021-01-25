// BLE Advertisement Raw app
//
// Sends a BLE advertisement with raw bytes

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:4e:xx:xx
        .platform_id       = 0x4E,   // used as 4th octect in device BLE address
        .device_id         = 0xAABB, // must be unique on each device you program!
        .adv_name          = "flori", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;


int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);

  // Start Advertising
  //uint8_t ble_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX] = {0x02, 0x01, 0x06, 0x06, 0x09, 0x66, 0x6c, 0x6f, 0x72, 0x69, 0x03, 0x03, 0xAA, 0xFE, 0x03, 0x16, 0xAA, 0xFE, 0x10};//need to update length and finish payload with url
  simple_ble_es_with_name("google.com");
  //requires frame type: 0x10 (URL)
  //service data 0x16 - value: 0xAA 0xFE 0x10 (url) 0xba 0x03 (https:) 0x67 0x6f 0x6f 0x67 0x6c 0x65 0x2e 0x63 0x6f 0x6d (google.com)
  //Complete list of 16-bit Service UUIDs data type value - set to eddystone uid 0xAAFE
  printf("Started BLE advertisements\n");

  while(1) {
    power_manage();
  }
}

