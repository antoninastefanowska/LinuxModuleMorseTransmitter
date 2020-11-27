#ifndef ENCODER_H
#define ENCODER_H

#define INTERVAL_DURATION 100

extern int dot_duration, dash_duration, pause_duration;

extern char *encode_char(char c);
extern int get_duration(char signal);
extern int change_dot_duration(int new_duration);
extern int change_dash_duration(int new_duration);
extern int change_pause_duration(int new_duration);

#endif
