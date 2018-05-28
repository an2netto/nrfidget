/** @file
  * @brief interface for 74HC595 driver
  *
  * see pinout in 74HC595.c
  */

#include <stdint.h>

#ifndef __SHREG_H
#define __SHREG_H

void shreg_init(void);
void shreg_shift(uint8_t c);
void shreg_clear(void);

#endif /* __SHREG_H */
