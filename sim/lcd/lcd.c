/** @file
  * @brief interface for 5110 LCD driver
  *
  * see pinout in pcd8544.c
  */

#include <stdarg.h>

#include "pcd8544.c"
#include "font.c"

static uint8_t m_cursor_x, m_cursor_y;
static const uint8_t m_hello_str[] = "HELLO";
static const uint8_t m_hello_str_len = 5;

static void LCD_print(const char * cc, uint8_t size);
//static void LCD_set_cursor(uint8_t x, uint8_t y, uint8_t size); // TODO interface after testing
static bool LCD_incr_cursor(uint8_t size);
static void LCD_char_draw(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size);
static uint8_t LCD_hello_draw(uint8_t x, uint8_t y, uint8_t color, uint8_t bg, uint8_t size);

void LCD_init(void)
{
    gpio_init();
    spi_init();
    pcd8544_init();
}

void LCD_uninit(void)
{
    pcd8544_uninit();
}

void LCD_clear(void) {
    m_cursor_x = 0;
    m_cursor_y = 0;

    pcd8544_rect_draw(m_cursor_x, m_cursor_y, WIDTH, HEIGHT, WHITE);
    pcd8544_display();
}

void LCD_char(uint8_t c) {
    LCD_char_draw(m_cursor_x, m_cursor_y, c, BLACK, WHITE, 1);
    pcd8544_display();

    if (LCD_incr_cursor(1)) {
      pcd8544_rect_draw(m_cursor_x, m_cursor_y, WIDTH, HEIGHT, WHITE);
    }
}

void LCD_printf(const char * fmt, ...) {
  	static char buf[256];
  	char *p;
  	va_list lst;

  	va_start(lst, fmt);
  	vsprintf(buf, fmt, lst);
  	va_end(lst);

  	p = buf;
  	LCD_print(p, 1);
}

uint8_t LCD_hello(void)
{
    uint8_t result = LCD_hello_draw(m_cursor_x, m_cursor_y, BLACK, WHITE, 1);
    pcd8544_display();

    m_cursor_x++;
    if (m_cursor_x == WIDTH) {
      m_cursor_x = 0;
      pcd8544_rect_draw(m_cursor_x, m_cursor_y, WIDTH, HEIGHT, WHITE);
    }

    return result;
}

/*
static void LCD_set_cursor(uint8_t x, uint8_t y, uint8_t size) {
    // clip right bottom left top
    if ((x >= WIDTH)
      || (y >= HEIGHT)
      || ((x + PCD8544_FONT_WIDTH * size - 1) < 0)
      || ((y + PCD8544_FONT_HEIGHT * size - 1) < 0)) {
      return;
    }
    m_cursor_x = x;
    m_cursor_y = y;
}
*/

// return clear flag
static bool LCD_incr_cursor(uint8_t size) {
    m_cursor_x += PCD8544_FONT_WIDTH * size;

    if (m_cursor_x > (WIDTH - PCD8544_FONT_WIDTH)) {
      m_cursor_y += PCD8544_FONT_HEIGHT * size;
      m_cursor_x = 0;
    }

    if (m_cursor_y > (HEIGHT - PCD8544_FONT_HEIGHT)) {
      m_cursor_y = 0;
      return true;
    }
    return false;
}

static void LCD_print(const char * cc, uint8_t size) {
    while (*cc) {
      if (*cc == '\n') {
  			m_cursor_y += PCD8544_FONT_HEIGHT;
  			m_cursor_x = 0;
  		}
      else if (*cc == '\r') {
  			m_cursor_x = 0;
  		}
      else if (*cc == '\t') {
  			m_cursor_x += PCD8544_FONT_WIDTH * 4;
  		}
      else {
        LCD_char(*cc);
      }
      cc++;
    }
}

// 5 pixels wide + 2 padding = 12 characters per row, six rows total with size 1
static void LCD_char_draw(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size) {
    for (uint8_t i=0; i<5; i++) { // Char bitmap = 5 columns
      uint8_t line = font[c][i];
      for (uint8_t j=0; j<8; j++, line >>= 1) {
        if (size == 1) {
          pcd8544_pixel_draw(x+i, y+j, line & 1 ? color : bg);
        }
        else {
          pcd8544_rect_draw(x+i*size, y+j*size, size, size, line & 1 ? color : bg);
        }
      }
    }

    if (size == 1) {
      pcd8544_line_draw(x+5, y, x+5, y+8-1, bg);
    }
    else {
      pcd8544_rect_draw(x+5*size, y, size, 8*size, bg);
    }
}

static uint8_t LCD_hello_draw(uint8_t x, uint8_t y, uint8_t color, uint8_t bg, uint8_t size) {
    static uint8_t hello_col_counter = 0;
    static uint8_t hello_char_counter = 0;
    static uint8_t result = 0;

    uint8_t c = m_hello_str[hello_char_counter % m_hello_str_len];
    // uint8_t i = hello_col_counter;

    uint8_t line = font[c][hello_col_counter];
    result = line;
    for (uint8_t j=0; j<8; j++, line >>= 1) {
      if (size == 1) {
        pcd8544_pixel_draw(x, y+j, line & 1 ? color : bg);
      }
      else {
        pcd8544_rect_draw(x+size, y+j*size, size, size, line & 1 ? color : bg);
      }
    }

    hello_col_counter++;

    if (hello_col_counter == PCD8544_FONT_WIDTH) {
      hello_col_counter = 0;
      hello_char_counter++;
      result = 0;

      if (size == 1) {
        pcd8544_line_draw(x, y, x, y+8-1, bg);
      }
      else {
        pcd8544_rect_draw(x*size, y, size, 8*size, bg);
      }
    }

    return result;
}
