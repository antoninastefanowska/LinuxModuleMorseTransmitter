#include <linux/tty.h>
#include <char/console_struct.h>

#include "console.h"

#define currcons fg_console

int colors[] = { 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77, 0x87, 0x97, 0xA7, 0xB7, 0xC7, 0xD7, 0xE7, 0xF7 };

int position_x = 0, position_y = 0;
int color_index = 0;

void turn_on(void)
{
    unsigned short *address = (unsigned short *)(origin + position_y * 2 * ORIG_VIDEO_COLS + position_x * 2);
    unsigned short old_element = *address;
    unsigned short new_element = (0xff & old_element) | (colors[color_index] << 8);
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

int change_color(int new_color_index)
{
    if (new_color_index < 0 || new_color_index > 14)
    {
        printk(KERN_ERR "Niepoprawna pozycja Y.\n");
        return -EINVAL;
    }
    color_index = new_color_index;
    return 0;
}