#include <linux/sched.h>

#include "driver.h"

#include "queue.h"

struct wait_queue *queue[DEVICES];

void queue_init(void)
{
    int i;
    for (i = 0; i < DEVICES; i++)
        init_waitqueue(&queue[i]);
}

int queue_sleep(int sub_device, int current_index)
{
    interruptible_sleep_on(&queue[sub_device]);
    if (current->signal & ~current->blocked)
    {
        if (current_index == 0)
            return -ERESTARTSYS;
        return current_index;
    }
    else
        return 0;
}

void queue_wake(int sub_device)
{
    wake_up(&queue[sub_device]);
}