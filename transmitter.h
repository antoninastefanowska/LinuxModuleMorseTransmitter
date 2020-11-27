#ifndef TRANSMITTER_H
#define TRANSMITTER_H

extern int currently_transmitting;

extern void start_transmitting_character(unsigned long arg);

void start_transmitting_signal(unsigned long arg);
void finish_transmitting_signal(unsigned long arg);
void finish_transmitting_character(unsigned long arg);

#endif