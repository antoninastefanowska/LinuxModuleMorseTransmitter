#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>

#define MAJOR_MORSE 50

int main(int argc, char *argv[])
{
    int file, result, parameter_value, command_number, i;
    char *command = argv[2], *sub_device = argv[1], *parameter;
    
    file = open(sub_device, O_RDWR);

    if (argc > 3)
    {
        parameter = argv[3];

        if (strcmp(parameter, "--buffersize") == 0)
            command_number = 0;

        else if (strcmp(parameter, "--dot-duration") == 0)
            command_number = 2;

        else if (strcmp(parameter, "--dash-duration") == 0)
            command_number = 4;

        else if (strcmp(parameter, "--pause-duration") == 0)
            command_number = 6;

        else if (strcmp(parameter, "--position-x") == 0)
            command_number = 8;

        else if (strcmp(parameter, "--position-y") == 0)
            command_number = 10;

        else if (strcmp(parameter, "--color") == 0)
            command_number = 12;
    }

    if (strcmp(command, "-w") == 0)
    {
        parameter_value = atoi(argv[4]);
	    result = ioctl(file, _IOW(MAJOR_MORSE, command_number, int *), &parameter_value);
    }
    else if (strcmp(command, "-r") == 0)
    {
        command_number++;

        if (argc > 3)
        {
	        result = ioctl(file, _IOR(MAJOR_MORSE, command_number, int *), &parameter_value);
	        printf("%d\n", parameter_value);
        }
        else
        {
            for (i = 1; i <= 13; i += 2)
            {
                result = ioctl(file, _IOR(MAJOR_MORSE, i, int *), &parameter_value);
	            printf("%d\n", parameter_value);
            }
        }
    }	
    
    switch (errno)
    {
	case EBADF:
	    printf("EBADF\n");
	    break;
	case EFAULT:
	    printf("EFAULT\n");
	    break;
	case EINVAL:
	    printf("EINVAL\n");
	    break;
	case ENOTTY:
	    printf("ENOTTY\n");
	    break;
    }
    
    close(file);
    return result;
}