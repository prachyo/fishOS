#ifndef TTY_H
#define TTY_H

#include "stdint.h"

void tty_init(void);
void tty_putc(char c);
void tty_write(const char* data, size_t size);
void tty_puts(const char* data);
void tty_clear(void);
void tty_mv_cursor(uint16_t row, uint16_t col);

#endif