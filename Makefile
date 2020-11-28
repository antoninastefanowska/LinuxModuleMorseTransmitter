MODULE = morse.o
MODFILES = driver.o buffer.o console.o encoder.o queue.o timer.o transmitter.o
IOCTL = driver_settings.c
IOCTL.o = driver_settings.o

MODCFLAGS := -D__KERNEL__ -DMODULE -O2
DRIVERCFLAG := -I/usr/src/linux/drivers

compile:
	gcc $(MODCFLAGS) $(DRIVERCFLAG) -c console.c
	gcc $(MODCFLAGS) -c buffer.c
	gcc $(MODCFLAGS) -c queue.c
	gcc $(MODCFLAGS) -c timer.c
	gcc $(MODCFLAGS) -c encoder.c
	gcc $(MODCFLAGS) -c transmitter.c
	gcc $(MODCFLAGS) -c driver.c

assemble:
	ld -m elf_i386 -r -o ${MODULE} ${MODFILES}

clean:
	rm -fr ${MODULE} ${MODFILES}

ioctl:
	gcc ${IOCTL} -o ${IOCTL.o}