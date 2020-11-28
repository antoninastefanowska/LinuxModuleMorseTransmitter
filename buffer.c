#include <linux/malloc.h>
#include <asm/semaphore.h>

#include "boolean.h"

#include "buffer.h"

char *buffer[DEVICES];
int buffersize[DEVICES] = { [0 ... DEVICES-1] = INIT_BUFFERSIZE };
int buffercount[DEVICES] = { [0 ... DEVICES-1] = 0 };
int start[DEVICES], end[DEVICES];

struct semaphore buffer_sem[DEVICES] = { [0 ... DEVICES-1] = MUTEX };

void buffer_create(int sub_device)
{
    down(&buffer_sem[sub_device]);
    buffer[sub_device] = kmalloc(buffersize[sub_device], GFP_KERNEL);
    start[sub_device] = end[sub_device] = 0;
    up(&buffer_sem[sub_device]);
}

int buffer_empty(int sub_device)
{
    if (buffercount[sub_device] == 0)
        return true;
    else
        return false;
}

int buffer_full(int sub_device)
{
    if (buffercount[sub_device] == buffersize[sub_device])
        return true;
    else
        return false;
}

char buffer_read(int sub_device)
{
    return buffer[sub_device][start[sub_device]];
}

void buffer_write(int sub_device, char c)
{
    buffer[sub_device][end[sub_device]] = c;
    buffercount[sub_device]++;
    end[sub_device]++;
    if (end[sub_device] == buffersize[sub_device])
        end[sub_device] = 0;
}

void buffer_update(int sub_device)
{
    start[sub_device]++;
    buffercount[sub_device]--;
    if (start[sub_device] == buffersize[sub_device])
        start[sub_device] = 0;
}

void buffer_free(int sub_device)
{
    kfree(buffer[sub_device]);
}

int buffer_size(int sub_device, int new_buffersize)
{
    int i;
    char *new_buffer;

    if (new_buffersize < MIN_BUFFERSIZE || new_buffersize > MAX_BUFFERSIZE || new_buffersize < buffersize[sub_device])
    {
        printk(KERN_ERR "Niepoprawny rozmiar bufora.\n");
        return -EINVAL;
    }

    down(&buffer_sem[sub_device]);
    if (buffercount > 0)
    {
        new_buffer = kmalloc(new_buffersize, GFP_KERNEL);
        for (i = 0; i < buffercount[sub_device]; i++)
            new_buffer[i] = buffer[sub_device][i];
        kfree(buffer[sub_device]);
        buffer[sub_device] = new_buffer;
    }
    buffersize[sub_device] = new_buffersize;
    up(&buffer_sem[sub_device]);

    return 0;
}