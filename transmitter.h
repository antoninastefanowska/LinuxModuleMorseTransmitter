#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "driver.h"

extern int currently_transmitting[DEVICES];

extern void start_transmitting_character(unsigned long sub_device);

void start_transmitting_signal(unsigned long sub_device);
void finish_transmitting_signal(unsigned long sub_device);
void finish_transmitting_character(unsigned long sub_device);

#endif