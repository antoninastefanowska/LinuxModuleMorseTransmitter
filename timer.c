#include <stddef.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <asm/semaphore.h>

#include "driver.h"

#include "timer.h"

struct semaphore timer_sem[DEVICES];
struct timer_list timer[DEVICES];

void timer_init(void)
{
    int i;
    for (i = 0; i < DEVICES; i++)
    {
        init_timer(&timer[i]);
        timer_sem[i] = MUTEX;
    }
}

void timer_start(int sub_device, void (*handler)(unsigned long), int wait_time)
{
    down(&timer_sem[sub_device]);

    del_timer(&timer[sub_device]);
    timer[sub_device].function = handler;
    timer[sub_device].expires = jiffies + (int)(HZ / 1000.0 * wait_time);
    timer[sub_device].data = sub_device;
    add_timer(&timer[sub_device]);

    up(&timer_sem[sub_device]);
}