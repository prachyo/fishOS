#include "pit.h"
#include "io.h"
#include "interrupt.h"
#include "common.h"
#include "pic.h"

#define PIT_CHANNEL_0_DATA  0x40
#define PIT_CHANNEL_1_DATA  0x41
#define PIT_CHANNEL_2_DATA  0x42
#define PIT_COMMAND         0x43

#define PIT_FREQUENCY       1193182 /* Hz */

static void (*callback)(void) = 0;

static void pit_handle_interrupt(cpu_state_t state, idt_info_t info,
                          exec_state_t exec)
{
    UNUSED_ARGUMENT(state);
    UNUSED_ARGUMENT(info);
    UNUSED_ARGUMENT(exec);
    if (callback != 0) {
        callback();
    }
    pic_acknowledge();
}

void pit_init(void)
{
    /* name | value | size | desc
     * --------------------------
     * chan |     0 |    2 | the channel to use, channel 0 = IRQ0
     * acs  |   0x3 |    2 | how the divider is sent, 3 = lobyte then hibyte
     * mode |   0x3 |    3 | the mode of the pit, mode 3 = square wave
     * bcd  |     0 |    1 | bcd or binary mode, 0 = binary, 1 = bcd
     */
    uint8_t data = 0x00 | (1 << 5) | (1 << 4) | (1 << 2) | (1 << 1);
    outb(PIT_COMMAND, data);

    register_interrupt_handler(PIT_INT_IDX, pit_handle_interrupt);
}

void pit_set_callback(uint16_t interval, void (*cb)(void))
{
    callback = cb;

    uint16_t divider = PIT_FREQUENCY / interval;

    outb(PIT_CHANNEL_0_DATA, (uint8_t) divider); /* the low byte */
    outb(PIT_CHANNEL_0_DATA, (uint8_t) (divider >> 8)); /* the high byte */
}