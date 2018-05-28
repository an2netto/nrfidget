/**
 * Copyright (c) 2018
 *
 * All rights reserved.
 *
 */

#include <stdbool.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "app_error.h"
#include "lcd.h"
#include "shreg.h"

void in_pin_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint8_t col = LCD_hello();
    shreg_shift(col);
}

static void gpio_init(void)
{
    ret_code_t err_code;

    err_code = nrfx_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    // in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrfx_gpiote_in_init(PIN_IN, &in_config, in_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrfx_gpiote_in_event_enable(PIN_IN, true);
}

int main(void)
{
    gpio_init();
    LCD_init();
    shreg_init();

    shreg_clear();

    // LCD_printf("start\n");


    while (true)
    {
      // shreg_shift(0x0F);
      // nrf_delay_ms(1000);
      // shreg_shift(0xF0);
      // nrf_delay_ms(1000);
    }
}


/** @} */
