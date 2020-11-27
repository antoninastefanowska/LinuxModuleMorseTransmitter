#include <stddef.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <asm/semaphore.h>

#include "timer.h"

struct semaphore timer_sem = MUTEX;
struct timer_list timer;

void timer_init()
{
    init_timer(&timer);
}

void timer_start(void (*handler)(unsigned long), int wait_time)
{
    down(&timer_sem);

    del_timer(&timer);
    timer.function = handler;
    timer.expires = jiffies + (int)(HZ / 1000.0 * wait_time);
    add_timer(&timer);

    up(&timer_sem);
}