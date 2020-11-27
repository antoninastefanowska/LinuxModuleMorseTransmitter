#ifndef BUFFER_H
#define BUFFER_H

#define MIN_BUFFERSIZE 0
#define MAX_BUFFERSIZE 1024
#define INIT_BUFFERSIZE 8

extern int buffersize;

extern void buffer_create(void);
extern int buffer_empty(void);
extern int buffer_full(void);
extern char buffer_read(void);
extern void buffer_write(char c);
extern void buffer_update(void);
extern void buffer_free(void);
extern int buffer_size(int new_buffersize);

#endif