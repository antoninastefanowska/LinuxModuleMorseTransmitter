#ifndef CONSOLE_H
#define CONSOLE_H

#include "driver.h"

#define OFF_COLOR 0x07
#define MAX_COLOR_INDEX 6

extern int position_x[DEVICES], position_y[DEVICES];
extern int color_index[DEVICES];

extern void console_init(void);
extern void turn_on(int sub_device);
extern void turn_off(int sub_device);
extern int change_position_x(int sub_device, int new_position_x);
extern int change_position_y(int sub_device, int new_position_y);
extern int change_color(int sub_device, int new_color_index);

#endif