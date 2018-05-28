
/** @file
  * @brief interface for 5110 LCD driver
  *
  * see pinout in pcd8544.c
  */

#include "stdint.h"

#ifndef __LCD_H
#define __LCD_H

void LCD_init(void);
void LCD_uninit(void);
void LCD_clear(void);
void LCD_char(uint8_t c);
uint8_t LCD_hello(void); /* draw single bitmap column per call, ring-buffer style */
void LCD_printf(const char *fmt, ...);

#endif /* __LCD_H */
