#ifndef ENCODER_H
#define ENCODER_H

#include "driver.h"

#define INTERVAL_DURATION 100

extern int dot_duration[DEVICES], dash_duration[DEVICES], pause_duration[DEVICES];

extern char *encode_char(char c);
extern int get_duration(int sub_device, char signal);
extern int change_dot_duration(int sub_device, int new_duration);
extern int change_dash_duration(int sub_device, int new_duration);
extern int change_pause_duration(int sub_device, int new_duration);

#endif
