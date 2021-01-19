// Blink app
//
// Blinks an LED

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "app_error.h"
#include "nrfx_gpiote.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

// Pin configurations
#include "nrf52840dk.h"

int main(void) {

  // Initialize.
  nrf_gpio_cfg_output(LED1);
  nrf_gpio_cfg_output(LED2);
  nrf_gpio_cfg_output(LED3);
  nrf_gpio_cfg_output(LED4);

  nrf_gpio_pin_set(LED1); //turns it on to start
  nrf_gpio_pin_set(LED2); //turns it on to start
  nrf_gpio_pin_clear(LED3); //turns it off to start
  nrf_gpio_pin_clear(LED4); //turns it off to start

  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");
  
  uint32_t i = 0;
  //Enter main loop.
  while (1) {
    nrf_gpio_pin_toggle(LED1);
    nrf_gpio_pin_toggle(LED2);
    nrf_gpio_pin_toggle(LED3);
    nrf_gpio_pin_toggle(LED4);
    nrf_delay_ms(100);
    if((i%100 == 0) && (i != 0))
    {
      printf("Toggled 100 times, on iteration: %lu\n", i);
    }
    i++;
  }
}

