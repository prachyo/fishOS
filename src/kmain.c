#include "tty.h"
#include "gdt.h"

#define UNUSED_ARGUMENT(x) (void) x
int kmain(void *mboot, unsigned int magic_number)
{
    UNUSED_ARGUMENT(mboot);
    UNUSED_ARGUMENT(magic_number);
    tty_init();
    gdt_init();
    tty_puts("hello world!");
    tty_mv_cursor(0, 0);
    return 0xDEADBEEF;
}