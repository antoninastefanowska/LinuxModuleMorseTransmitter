#include <stddef.h>

#include "boolean.h"
#include "buffer.h"
#include "encoder.h"
#include "console.h"
#include "queue.h"
#include "timer.h"
#include "driver.h"

#include "transmitter.h"

char *current_encoded_char;
int current_signal_index, current_signal_size;
int currently_transmitting;

int word_size(char *word)
{
    int count = 0;
    while (*(word + count) != '\0')
        count++;
    return count;
}

void start_transmitting_character(unsigned long arg)
{
    char c = buffer_read();
    current_encoded_char = encode_char(c);
    if (current_encoded_char != NULL)
    {
        currently_transmitting = true;
        current_signal_size = word_size(current_encoded_char);
        current_signal_index = 0;
        start_transmitting_signal(0);
    }
    else
        finish_transmitting_character(0);
}

void start_transmitting_signal(unsigned long arg)
{
    char signal = current_encoded_char[current_signal_index];
    int duration = get_duration(signal);
    if (signal != ' ')
        turn_on();
    timer_start(finish_transmitting_signal, duration);
}

void finish_transmitting_signal(unsigned long arg)
{
    char signal = current_encoded_char[current_signal_index];
    if (signal != ' ')
        turn_off();

    current_signal_index++;
    if (current_signal_index < current_signal_size)
        timer_start(start_transmitting_signal, INTERVAL_DURATION);
    else
        timer_start(finish_transmitting_character, INTERVAL_DURATION);
}

void finish_transmitting_character(unsigned long arg)
{
    buffer_update();
    queue_wake();
    if (buffer_empty() == true)
    {
        currently_transmitting = false;
        if (device_unused() == true)
            buffer_free();
    }
    else
        start_transmitting_character(0);
}