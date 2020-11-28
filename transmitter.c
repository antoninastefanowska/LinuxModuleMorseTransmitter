#include <stddef.h>

#include "boolean.h"
#include "buffer.h"
#include "encoder.h"
#include "console.h"
#include "queue.h"
#include "timer.h"

#include "transmitter.h"

char *current_encoded_char[DEVICES];
int current_signal_index[DEVICES], current_signal_size[DEVICES];
int currently_transmitting[DEVICES];

int word_size(char *word)
{
    int count = 0;
    while (*(word + count) != '\0')
        count++;
    return count;
}

void start_transmitting_character(unsigned long sub_device)
{
    char c = buffer_read(sub_device);
    current_encoded_char[sub_device] = encode_char(c);
    if (current_encoded_char[sub_device] != NULL)
    {
        currently_transmitting[sub_device] = true;
        current_signal_size[sub_device] = word_size(current_encoded_char[sub_device]);
        current_signal_index[sub_device] = 0;
        start_transmitting_signal(sub_device);
    }
    else
        finish_transmitting_character(sub_device);
}

void start_transmitting_signal(unsigned long sub_device)
{
    char signal = current_encoded_char[sub_device][current_signal_index[sub_device]];
    int duration = get_duration(sub_device, signal);
    if (signal != ' ')
        turn_on(sub_device);
    timer_start(sub_device, finish_transmitting_signal, duration);
}

void finish_transmitting_signal(unsigned long sub_device)
{
    char signal = current_encoded_char[sub_device][current_signal_index[sub_device]];
    if (signal != ' ')
        turn_off(sub_device);

    current_signal_index[sub_device]++;
    if (current_signal_index[sub_device] < current_signal_size[sub_device])
        timer_start(sub_device, start_transmitting_signal, INTERVAL_DURATION);
    else
        timer_start(sub_device, finish_transmitting_character, INTERVAL_DURATION);
}

void finish_transmitting_character(unsigned long sub_device)
{
    buffer_update(sub_device);
    queue_wake(sub_device);
    if (buffer_empty(sub_device) == true)
    {
        currently_transmitting[sub_device] = false;
        if (device_unused(sub_device) == true)
            buffer_free(sub_device);
    }
    else
        start_transmitting_character(sub_device);
}