#include <stddef.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>

#include "boolean.h"
#include "buffer.h"
#include "queue.h"
#include "transmitter.h"
#include "console.h"
#include "encoder.h"
#include "timer.h"

#include "driver.h"

int usecount[DEVICES];

int device_unused(int sub_device)
{
    if (usecount[sub_device] == 0)
	    return true;
    else
	    return false;
}

int morse_open(struct inode *inode, struct file *file)
{
    int sub_device = MINOR(inode->i_rdev);
    if (sub_device >= DEVICES)
    {
        printk(KERN_ERR "Bledny numer urzadzenia.\n");
        return -EINVAL;
    }

    MOD_INC_USE_COUNT;
    if (device_unused(sub_device) == true && buffer_empty(sub_device) == true)
        buffer_create(sub_device);
    usecount[sub_device]++;
    return 0;
}

void morse_release(struct inode *inode, struct file *file)
{
    int sub_device = MINOR(inode->i_rdev);
    if (sub_device >= DEVICES)
    {
        printk(KERN_ERR "Bledny numer urzadzenia.\n");
        return;
    }
    if (!currently_transmitting[sub_device])
        buffer_free(sub_device);

    MOD_DEC_USE_COUNT;
    usecount[sub_device]--;
}

int morse_write(struct inode *inode, struct file *file, const char *pB, int count)
{
    int i, sleep_result;
    char c;
    int sub_device = MINOR(inode->i_rdev);
    if (sub_device >= DEVICES)
    {
        printk(KERN_ERR "Bledny numer urzadzenia.\n");
        return -EINVAL;
    }

    for (i = 0; i < count; i++)
    {
        c = get_user(pB + i);
        while (buffer_full(sub_device) == true)
        {
            sleep_result = queue_sleep(sub_device, i);
            if (sleep_result != 0)
                return sleep_result;
        }
        buffer_write(sub_device, c);

        if (!currently_transmitting[sub_device])
            start_transmitting_character(sub_device);
    }
    return count;
}

int morse_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    int err, user_input;
    int command_number = 0xff & cmd;
    int sub_device = MINOR(inode->i_rdev);
    
    if (sub_device >= DEVICES)
    {
        printk(KERN_ERR "Bledny numer urzadzenia.\n");
        return -EINVAL;
    }

    if (_IOC_DIR(cmd) != _IOC_NONE)
    {
        int len = _IOC_SIZE(cmd);
        if (_IOC_DIR(cmd) & _IOC_WRITE)
        {
            if ((err = verify_area(VERIFY_READ, (int *)arg, len)) < 0)
            {
                printk(KERN_ERR "Brak dostepu do pamieci.\n");
                return err;
            }
        }
        if (_IOC_DIR(cmd) & _IOC_READ)
        {
            if ((err = verify_area(VERIFY_WRITE, (int *)arg, len)) < 0)
            {
                printk(KERN_ERR "Brak dostepu do pamieci.\n");
                return err;
            }
        }
    }

    user_input = get_user((int *)arg);

    switch (command_number)
    {
    case 0:
        return buffer_change_size(sub_device, user_input);

    case 1:
        put_user(buffersize[sub_device], (int *)arg);
        return 0;

    case 2:
        return change_dot_duration(sub_device, user_input);

    case 3:
        put_user(dot_duration[sub_device], (int *)arg);
        return 0;

    case 4:
        return change_dash_duration(sub_device, user_input);

    case 5:
        put_user(dash_duration[sub_device], (int *)arg);
        return 0;

    case 6:
        return change_pause_duration(sub_device, user_input);

    case 7:
        put_user(pause_duration[sub_device], (int *)arg);
        return 0;

    case 8:
        return change_position_x(sub_device, user_input);

    case 9:
        put_user(position_x[sub_device], (int *)arg);
        return 0;
    
    case 10:
        return change_position_y(sub_device, user_input);

    case 11:
        put_user(position_y[sub_device], (int *)arg);
        return 0;

    case 12:
        return change_color(sub_device, user_input);
    
    case 13:
        put_user(color_index[sub_device], (int *)arg);
        return 0;

    default:
        printk(KERN_ERR "Niepoprawna komenda.\n");
        return -EINVAL;
    }
}

struct file_operations morse_ops = {
    write : morse_write,
    open : morse_open,
    release : morse_release,
    ioctl : morse_ioctl
};

int init_module(void)
{
    int result, i;

    queue_init();
    timer_init();
    console_init();

    for (i = 0; i < DEVICES; i++)
        usecount[i] = 0;

    result = register_chrdev(MORSE_MAJOR, "morse", &morse_ops);

    if (result == 0)
        printk("URZADZENIE OTWARTE\n");
    else
        printk("NIEPOWODZENIE OTWARCIA\n");
    return result;
}

void cleanup_module(void)
{
    int result = unregister_chrdev(MORSE_MAJOR, "morse");

    if (result == 0)
        printk("URZADZENIE ZAMKNIETE\n");
    else
        printk("NIEPOWODZENIE ZAMKNIECIA\n");
}