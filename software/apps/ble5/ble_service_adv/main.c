// BLE Service example app
//
// Creates a BLE service and blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = 0xAABB,
  .adv_name          = "CS397/497", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

//static simple_ble_service_t led_service = {{
//  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
//              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
//}};
//
//
//static simple_ble_char_t led_state_char = {.uuid16 = 0x1089};
//static bool led_state = false;
//
static simple_ble_service_t floris_service = {{
  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
}};

static simple_ble_char_t led_state_char = {.uuid16 = 0x1089};
static bool led_state = false;
static simple_ble_char_t notify_state_char = {.uuid16 = 0x108A};
static bool notify_state = false;
static simple_ble_char_t print_state_char = {.uuid16 = 0x108B};
static char print_char = 'a';
/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {

  // Check LED characteristic
  if (simple_ble_is_char_event(p_ble_evt, &led_state_char)) {
    printf("Got write to LED characteristic!\n");

    // Use value written to control LED
    if (led_state != 0) {
      printf("Turning on LED!\n");
      nrf_gpio_pin_clear(LED1);
    } else {
      printf("Turning off LED!\n");
      nrf_gpio_pin_set(LED1);
    }
  }
  if (simple_ble_is_char_event(p_ble_evt, &print_state_char)) {

    //Use char written to print
    printf("The char written was %c\n", print_char);
  }
}


int main(void) {

  nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);

  printf("Board started. Initializing BLE: \n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(LED1);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&floris_service);
  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(led_state), (uint8_t*)&led_state,
      &floris_service, &led_state_char);
  simple_ble_add_characteristic(1,0,1,0,
      sizeof(notify_state), (uint8_t*)&notify_state,
      &floris_service, &notify_state_char); //read and notify but dont write
  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(print_char), (uint8_t*)&print_char,
      &floris_service, &print_state_char);
  // Start Advertising
  simple_ble_adv_only_name();


  while(1) {
    if((!nrf_gpio_pin_read(BUTTON1)) || (!nrf_gpio_pin_read(BUTTON2)) || (!nrf_gpio_pin_read(BUTTON3)) || (!nrf_gpio_pin_read(BUTTON4))){
      printf("pushed button sensed");
      if(notify_state){
        notify_state = false;
      } else {
        notify_state = true;
      }
        simple_ble_notify_char(&notify_state_char);
    }
    //power_manage();
  }
}

