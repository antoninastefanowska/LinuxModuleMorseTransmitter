#ifndef BUFFER_H
#define BUFFER_H

#include "driver.h"

#define MIN_BUFFERSIZE 0
#define MAX_BUFFERSIZE 1024
#define INIT_BUFFERSIZE 256

extern int buffersize[DEVICES];

extern void buffer_create(int sub_device);
extern int buffer_empty(int sub_device);
extern int buffer_full(int sub_device);
extern char buffer_read(int sub_device);
extern void buffer_write(int sub_device, char c);
extern void buffer_free(int sub_device);
extern int buffer_change_size(int sub_device, int new_buffersize);

#endif