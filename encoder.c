#include <stddef.h>
#include <linux/kernel.h>
#include <linux/errno.h>

#include "encoder.h"

char *dictionary[] = {
    "*-", 
    "-***", 
    "-*-*", 
    "-**", 
    "*", 
    "**-*",
    "--*", 
    "****", 
    "**", 
    "*---", 
    "-*-", 
    "*-**",
    "--", 
    "-*", 
    "---", 
    "*--*", 
    "--*-", 
    "*-*",
    "***", 
    "-", 
    "**-", 
    "***-", 
    "*--", 
    "-**-",
    "-*--", 
    "--**", 
    "-----", 
    "*----", 
    "**---", 
    "***--",
    "****-", 
    "*****", 
    "-****", 
    "--***", 
    "---**", 
    "----*" 
};

int dot_duration[DEVICES] = { [0 ... DEVICES-1] = 500 };
int dash_duration[DEVICES] = { [0 ... DEVICES-1] = 1000 };
int pause_duration[DEVICES] = { [0 ... DEVICES-1] = 1000 };

char *encode_char(char c)
{
    if (c >= 'A' && c <= 'Z')
        return dictionary[c - 'A'];

    else if (c >= 'a' && c <= 'z')
        return dictionary[c - 'a'];

    else if (c >= '0' && c <= '9')
        return dictionary[c - '0' + 26];

    else if (c == ' ')
        return " ";

    else
        return NULL;
}

int get_duration(int sub_device, char signal)
{
    switch (signal)
    {
    case '*':
        return dot_duration[sub_device];
        break;
    case '-':
        return dash_duration[sub_device];
        break;
    case ' ':
        return pause_duration[sub_device];
        break;
    default:
        return 0;
        break;
    }
}

int change_dot_duration(int sub_device, int new_duration)
{
    if (new_duration < 0)
    {
        printk(KERN_ERR "Niepoprawny czas trwania.\n");
        return -EINVAL;
    }
    dot_duration[sub_device] = new_duration;
    return 0;
}

int change_dash_duration(int sub_device, int new_duration)
{
    if (new_duration < 0)
    {
        printk(KERN_ERR "Niepoprawny czas trwania.\n");
        return -EINVAL;
    }
    dash_duration[sub_device] = new_duration;
    return 0;
}

int change_pause_duration(int sub_device, int new_duration)
{
    if (new_duration < 0)
    {
        printk(KERN_ERR "Niepoprawny czas trwania.\n");
        return -EINVAL;
    }
    pause_duration[sub_device] = new_duration;
    return 0;
}