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
    char *available_parameters[] = { "--buffersize", "--dot-duration", "--dash-duration", "--pause-duration", "--position-x", "--position-y", "--color" };
    
    file = open(sub_device, O_RDWR);

    if (argc > 3)
    {
        parameter = argv[3];
        for (i = 0; i < 7; i++)
        {
            if (strcmp(parameter, available_parameters[i]) == 0)
            {
                command_number = i * 2;
                break;
            }
        }
    }

    if (strcmp(command, "-w") == 0)
    {
        parameter_value = atoi(argv[4]);
	    result = ioctl(file, _IOW(MAJOR_MORSE, command_number, int *), &parameter_value);
    }
    else if (strcmp(command, "-r") == 0)
    {
        if (argc > 3)
        {        
            command_number++;
	        result = ioctl(file, _IOR(MAJOR_MORSE, command_number, int *), &parameter_value);
	        printf("%d\n", parameter_value);
        }
        else
        {
            for (i = 0; i < 7; i++)
            {
                command_number = i * 2 + 1;
                result = ioctl(file, _IOR(MAJOR_MORSE, command_number, int *), &parameter_value);
	            printf("%s %d\n", available_parameters[i], parameter_value);
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