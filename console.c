#include <linux/tty.h>
#include <char/console_struct.h>

#include "console.h"

#define currcons fg_console

int colors[] = { 0x17, 0x27, 0x37, 0x47, 0x57, 0x67, 0x77 };

int position_x[DEVICES], position_y[DEVICES];
int color_index[DEVICES];

void console_init(void)
{
    int i;
    for (i = 0; i < DEVICES; i++)
    {
        position_x[i] = i;
        position_y[i] = 0;
        color_index[i] = 0;
    }
}

void turn_on(int sub_device)
{
    unsigned short *address = (unsigned short *)(origin + position_y[sub_device] * 2 * ORIG_VIDEO_COLS + position_x[sub_device] * 2);
    unsigned short old_element = *address;
    unsigned short new_element = (0xff & old_element) | (colors[color_index[sub_device]] << 8);
    *address = new_element;
}

void turn_off(int sub_device)
{
    unsigned short *address = (unsigned short *)(origin + position_y[sub_device] * 2 * ORIG_VIDEO_COLS + position_x[sub_device] * 2);
    unsigned short old_element = *address;
    unsigned short new_element = (0xff & old_element) | (OFF_COLOR << 8);
    *address = new_element;
}

int change_position_x(int sub_device, int new_position_x)
{
    int i;
    if (new_position_x < 0 || new_position_x > ORIG_VIDEO_COLS)
    {
        printk(KERN_ERR "Niepoprawna pozycja X.\n");
        return -EINVAL;
    }
    for (i = 0; i < DEVICES; i++)
    {
        if (i != sub_device && position_x[i] == new_position_x && position_y[sub_device] == position_y[i])
        {
            printk(KERN_ERR "Zajeta pozycja X.\n");
            return -EINVAL;
        }
    }
    position_x[sub_device] = new_position_x;
    return 0;
}

int change_position_y(int sub_device, int new_position_y)
{
    int i;
    if (new_position_y < 0 || new_position_y > ORIG_VIDEO_LINES)
    {
        printk(KERN_ERR "Niepoprawna pozycja Y.\n");
        return -EINVAL;
    }
    for (i = 0; i < DEVICES; i++)
    {
        if (i != sub_device && position_y[i] == new_position_y && position_x[sub_device] == position_x[i])
        {
            printk(KERN_ERR "Zajeta pozycja Y.\n");
            return -EINVAL;
        }
    }
    position_y[sub_device] = new_position_y;
    return 0;
}

int change_color(int sub_device, int new_color_index)
{
    if (new_color_index < 0 || new_color_index > MAX_COLOR_INDEX)
    {
        printk(KERN_ERR "Niepoprawna pozycja Y.\n");
        return -EINVAL;
    }
    color_index[sub_device] = new_color_index;
    return 0;
}