/** @file
  * @brief 74HC595 driver
  *
  * SRCLK  pin 11 connected to SHREG_SRCLK_PIN
  * RCLK   pin 12 connected to SHREG_RCLK_PIN
  * SER    pin 14 connected to SHREG_SER_PIN
  * VCC    pin 16 3.3V
  * GND    pin 8  GND
  */

#include "shreg.h"
#include "nrfx_spim.h"
#include "nrfx_gpiote.h"
#include "app_error.h"

static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(1);
static uint8_t ser_buf; /**< 8-bit serial buffer */

static void gpio_init(void)
{
    ret_code_t err_code;

    if (nrfx_gpiote_is_init() == false) {
      err_code = nrfx_gpiote_init();
      APP_ERROR_CHECK(err_code);
    }

    nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(false);

    err_code = nrfx_gpiote_out_init(SHREG_RCLK_PIN, &out_config);
    APP_ERROR_CHECK(err_code);
}

static void spi_init(void)
{
    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    spi_config.mosi_pin = SHREG_SER_PIN;
    spi_config.sck_pin  = SHREG_SRCLK_PIN;
    spi_config.bit_order = NRF_SPIM_BIT_ORDER_LSB_FIRST;
    // spi_config.ss_pin  = SHREG_RCLK_PIN;
    // spi_config.ss_active_high = false;
    spi_config.frequency = NRF_SPIM_FREQ_1M;

    APP_ERROR_CHECK(nrfx_spim_init(&spi, &spi_config, NULL, NULL));
}

void shreg_init(void)
{
    gpio_init();
    spi_init();
}

void shreg_shift(uint8_t c)
{
    nrf_gpio_pin_clear(SHREG_RCLK_PIN);

    ser_buf = c;
    static const nrfx_spim_xfer_desc_t ser_xfer_desc = NRFX_SPIM_XFER_TX(&ser_buf, 1);
    APP_ERROR_CHECK(nrfx_spim_xfer(&spi, &ser_xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER));

    nrf_gpio_pin_set(SHREG_RCLK_PIN);
}

void shreg_clear(void)
{
    shreg_shift(0x00);
}
