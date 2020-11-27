#include <linux/malloc.h>
#include <asm/semaphore.h>

#include "boolean.h"

#include "buffer.h"

char *buffer;
int buffersize = INIT_BUFFERSIZE, buffercount = 0, start, end;
struct semaphore buffer_sem = MUTEX;

void buffer_create(void)
{
    down(&buffer_sem);
    buffer = kmalloc(buffersize, GFP_KERNEL);
    start = end = 0;
    up(&buffer_sem);
}

int buffer_empty(void)
{
    if (buffercount == 0)
        return true;
    else
        return false;
}

int buffer_full(void)
{
    if (buffercount == buffersize)
        return true;
    else
        return false;
}

char buffer_read(void)
{
    return buffer[start];
}

void buffer_write(char c)
{
    buffer[end] = c;
    buffercount++;
    end++;
    if (end == buffersize)
        end = 0;
}

void buffer_update(void)
{
    start++;
    buffercount--;
    if (start == buffersize)
        start = 0;
}

void buffer_free(void)
{
    kfree(buffer);
}

int buffer_size(int new_buffersize)
{
    int i;
    char *new_buffer;

    if (new_buffersize < MIN_BUFFERSIZE || new_buffersize > MAX_BUFFERSIZE || new_buffersize < buffersize)
    {
        printk(KERN_ERR "Niepoprawny rozmiar bufora.\n");
        return -EINVAL;
    }

    down(&buffer_sem);
    new_buffer = kmalloc(new_buffersize, GFP_KERNEL);
    for (i = 0; i < buffercount; i++)
        new_buffer[i] = buffer[i];
    kfree(buffer);
    buffer = new_buffer;
    buffersize = new_buffersize;
    up(&buffer_sem);

    return 0;
}