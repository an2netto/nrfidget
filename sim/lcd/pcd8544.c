/** @file
  * @brief 5110 LCD driver
  *
  * Signal        (Nokia 5110) PCD8544 pin
  * Reset         (RST, pin 1) connected to P0.11
  * SSI0Fss       (CE,  pin 2) connected to GND
  * Data/Command  (DC,  pin 3) connected to P0.15
  * SSI0Tx        (DIN, pin 4) connected to P0.17
  * SSI0Clk       (CLK, pin 5) connected to P0.19
  * 3.3V          (VCC, pin 6)
  * back light    (BL,  pin 7) not connected
  * Ground        (GND, pin 8)
  */

#include "nrfx_spim.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "app_error.h"

#include <stdlib.h>
#include <string.h>

#define WIDTH PCD8544_WIDTH
#define HEIGHT PCD8544_HEIGHT

#define BLACK 1
#define WHITE 0

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }

static uint8_t pcd8544_buffer[WIDTH * HEIGHT / 8]; /**< pcd8544 memory buffer */
static uint8_t tx_cnd_buf; /**< single transfer command buffer */

static uint8_t xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax; /**< partial update: reduces how much is refreshed */

static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(0);

static inline void write_command(uint8_t c)
{
    tx_cnd_buf = c;
    static const nrfx_spim_xfer_desc_t tx_cnd_xfer_desc = NRFX_SPIM_XFER_TX(&tx_cnd_buf, 1);
    nrf_gpio_pin_clear(PCD8544_DC_PIN);
    APP_ERROR_CHECK(nrfx_spim_xfer(&spi, &tx_cnd_xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER));
}

static inline void write_data(uint8_t * tx_data_buf_offset, size_t count)
{
    const nrfx_spim_xfer_desc_t tx_data_xfer_desc = NRFX_SPIM_XFER_TX(tx_data_buf_offset, count);
    nrf_gpio_pin_set(PCD8544_DC_PIN);
    APP_ERROR_CHECK(nrfx_spim_xfer(&spi, &tx_data_xfer_desc, NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER));
}

static void gpio_init(void)
{
    ret_code_t err_code;

    if (nrfx_gpiote_is_init() == false) {
      err_code = nrfx_gpiote_init();
      APP_ERROR_CHECK(err_code);
    }

    nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(false);

    err_code = nrfx_gpiote_out_init(PCD8544_DC_PIN, &out_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_gpiote_out_init(PCD8544_RST_PIN, &out_config);
    APP_ERROR_CHECK(err_code);
}

static void spi_init(void)
{
    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    spi_config.mosi_pin = PCD8544_DIN_PIN;
    spi_config.sck_pin  = PCD8544_CLK_PIN;
    spi_config.frequency = NRF_SPIM_FREQ_4M;

    APP_ERROR_CHECK(nrfx_spim_init(&spi, &spi_config, NULL, NULL));
}

static void pcd8544_update_bounding_box(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax)
{
    if (xmin < xUpdateMin) xUpdateMin = xmin;
    if (xmax > xUpdateMax) xUpdateMax = xmax;
    if (ymin < yUpdateMin) yUpdateMin = ymin;
    if (ymax > yUpdateMax) yUpdateMax = ymax;
}

static void pcd8544_pixel_draw(uint8_t x, uint8_t y, uint8_t color)
{
    if ((x < 0) || (x >= WIDTH) || (y < 0) || (y >= HEIGHT)) {
      return;
    }
    // x is which column
    if (color) {
      pcd8544_buffer[x+ (y/8)* WIDTH] |= 1<<(y%8);
    }
    else {
      pcd8544_buffer[x+ (y/8)* WIDTH] &= ~(1<<(y%8));
    }
    pcd8544_update_bounding_box(x, y, x, y);
}

// Bresenham's algorithm - thx wikpedia
static void pcd8544_line_draw(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
      _swap_int16_t(x0, y0);
      _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
      _swap_int16_t(x0, x1);
      _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
      ystep = 1;
  } else {
      ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
        pcd8544_pixel_draw(y0, x0, color);
    } else {
        pcd8544_pixel_draw(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
        y0 += ystep;
        err += dx;
    }
  }
}

static void pcd8544_rect_draw(uint8_t x, uint8_t y, uint16_t width, uint16_t height, uint8_t color)
{
    for (uint8_t i=x; i<x+width; i++) {
      pcd8544_line_draw(i, y, i, y+height-1, color);
    }
}

static void pcd8544_display(void) {
    uint8_t col, maxcol, p;

    for (p = 0; p < 6; p++) {
      // check if this page is part of update
      if ( yUpdateMin >= ((p+1)*8) ) {
        continue;   // nope, skip it!
      }
      if (yUpdateMax < p*8) {
        break;
      }

      write_command(PCD8544_SETYADDR | p);

      col = xUpdateMin;
      maxcol = xUpdateMax;

      write_command(PCD8544_SETXADDR | col);

      nrf_gpio_pin_set(PCD8544_DC_PIN);

      write_data(pcd8544_buffer + WIDTH*p + col, 1 + maxcol - col);

      // for (; col <= maxcol; col++) {
      //   spi_write(pcd8544_buffer[WIDTH*p + col]);
      // }
    }

    write_command(PCD8544_SETYADDR); // no idea why this is necessary but it is to finish the last byte?
    xUpdateMin = WIDTH - 1;
    xUpdateMax = 0;
    yUpdateMin = HEIGHT - 1;
    yUpdateMax = 0;
}

static void pcd8544_init(void)
{
    nrf_gpio_pin_clear(PCD8544_RST_PIN);
    nrf_delay_ms(200);
    nrf_gpio_pin_set(PCD8544_RST_PIN);

    write_command(0x21); // LCD extended commands
    write_command(0xB8); // set LCD Vop (contrast)
    write_command(0x04); // set temp coefficient
    write_command(0x14); // LCD bias mode 1:40

    // write_command(0x20); // LCD basic commands
    // write_command(0x09); // LCD all segments on

    write_command(PCD8544_FUNCTIONSET);
    write_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);  // set display control to normal or inverted

    pcd8544_rect_draw(0, 0, WIDTH, HEIGHT, WHITE);
    pcd8544_display();
}

static void pcd8544_uninit(void)
{
    nrfx_spim_uninit(&spi);
}
