#include "tty.h"
#include "stdint.h"
#include "vga.h"
#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define TTY_CURSOR_DATA_PORT 0x3D5
#define TTY_CURSOR_INDEX_PORT 0x3D4

#define TTY_HIGH_BYTE 14
#define TTY_LOW_BYTE 15

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer = (uint8_t *) VGA_MEMORY;

void tty_init(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	tty_mv_cursor(0,0);
}

void tty_setcolor(uint8_t color) {
	terminal_color = color;
}

void tty_mv_cursor_up() {
	terminal_row--;
	tty_mv_cursor(terminal_row, terminal_column);
}

void tty_mv_cursor_down() {
	terminal_row++;
	tty_mv_cursor(terminal_row, terminal_column);
}

void tty_mv_cursor_start() {
	terminal_column = 0;
	tty_mv_cursor(terminal_row, terminal_column);
}

void tty_mv_cursor_end() {
	terminal_column = 79;
	tty_mv_cursor(terminal_row, terminal_column);
}

char tty_readc(size_t row, size_t col) {
	const size_t index = row * VGA_WIDTH + col;
	char *cell = terminal_buffer[index];
	return cell;

}

void tty_put_at(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	tty_advance_cursor();
}

void tty_putc(char c) {
	unsigned char uc = c;
	if(c != '\n')
		tty_put_at(uc, terminal_color, terminal_column, terminal_row);

	if(c == '\n') {
		tty_mv_cursor_down();
		tty_mv_cursor_start();
	} else {
		tty_advance_cursor();
	}
	
}

void tty_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void tty_puts(const char* str) {
	while (*str != '\0') {
        tty_putc(*str++);
    }
}

void tty_clear(void) {
    return tty_init();
}

void tty_set_cursor(uint16_t dest) {
	outb(TTY_CURSOR_INDEX_PORT, TTY_HIGH_BYTE);
    outb(TTY_CURSOR_DATA_PORT, dest >> 8);
    outb(TTY_CURSOR_INDEX_PORT, TTY_LOW_BYTE);
    outb(TTY_CURSOR_DATA_PORT, dest);
}

void tty_mv_cursor(uint16_t row, uint16_t col) {
    uint16_t loc = row*VGA_WIDTH + col;
	terminal_row = row;
	terminal_column = col;
	tty_set_cursor(loc);
}

void tty_advance_cursor(void) {
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			tty_scroll();
	}
}

static void tty_scroll()
{
    uint32_t r, c;
    for (r = 1; r < VGA_HEIGHT; ++r) {
        for (c = 0; c < VGA_WIDTH; ++c) {
            tty_put_at(tty_readc(r, c), terminal_color, r - 1, c);
        }
    }

    for (c = 0; c < VGA_WIDTH; ++c) {
        fb_write(' ', VGA_HEIGHT - 1, c);
    }

	tty_mv_cursor(VGA_HEIGHT -1, 0);
}