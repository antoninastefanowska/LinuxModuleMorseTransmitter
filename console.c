#include <linux/tty.h>
#include <char/console_struct.h>

#include "console.h"

#define currcons fg_console

int position_x = 0, position_y = 0;

void turn_on(void)
{
    unsigned short *address = (unsigned short *)(origin + position_y * 2 * ORIG_VIDEO_COLS + position_x * 2);
    unsigned short old_element = *address;
    unsigned short new_element = (0xff & old_element) | (ON_COLOR << 8);
    *address = new_element;
}

void turn_off(void)
{
    unsigned short *address = (unsigned short *)(origin + position_y * 2 * ORIG_VIDEO_COLS + position_x * 2);
    unsigned short old_element = *address;
    unsigned short new_element = (0xff & old_element) | (OFF_COLOR << 8);
    *address = new_element;
}

int change_position_x(int new_position_x)
{
    if (new_position_x < 0 || new_position_x > ORIG_VIDEO_COLS)
    {
        printk(KERN_ERR "Niepoprawna pozycja X.\n");
        return -EINVAL;
    }
    position_x = new_position_x;
    return 0;
}

int change_position_y(int new_position_y)
{
    if (new_position_y < 0 || new_position_y > ORIG_VIDEO_LINES)
    {
        printk(KERN_ERR "Niepoprawna pozycja Y.\n");
        return -EINVAL;
    }
    position_y = new_position_y;
    return 0;
}