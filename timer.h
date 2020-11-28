#ifndef TIMER_H
#define TIMER_H

extern void timer_init(void);
extern void timer_start(int sub_device, void (*handler)(unsigned long), int wait_time);

#endif