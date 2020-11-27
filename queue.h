#ifndef QUEUE_H
#define QUEUE_H

extern void queue_init(void);
extern int queue_sleep(int current_index);
extern void queue_wake(void);

#endif