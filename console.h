#ifndef CONSOLE_H
#define CONSOLE_H

#define ON_COLOR 0x17
#define OFF_COLOR 0x07

extern int position_x, position_y;

extern void turn_on(void);
extern void turn_off(void);
extern int change_position_x(int new_position_x);
extern int change_position_y(int new_position_y);

#endif