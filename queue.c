#include <linux/sched.h>

#include "queue.h"

struct wait_queue *queue;

void queue_init(void)
{
    init_waitqueue(&queue);
}

int queue_sleep(int current_index)
{
    interruptible_sleep_on(&queue);
    if (current->signal & ~current->blocked)
    {
        if (current_index == 0)
            return -ERESTARTSYS;
        return current_index;
    }
    else
        return 0;
}

void queue_wake(void)
{
    wake_up(&queue);
}