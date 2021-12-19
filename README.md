# ioctl-calls
Implementing kernel modules to communicate to device files

chardev.c − Creates an input/output character device

chardev.h − the header file with the ioctl definitions. The declarations here have to be in a header file, because they need to be known both to the kernel module (in chardev.c) and the process calling ioctl (ioctl.c)

ioctl.c − the process to use ioctl's to control the kernel module. Until now we could have used cat for input and output. But now we need to do ioctl's, which require writing our own process.

**To run the program**

-> Invoke make from the directory containing these files.

	%make

-> To insert the module:

	%sudo insmod ./chardev.ko

-> To check for the device name and major and minor number associated, type the following:

	%dmesg

->To create a charecter device type the command shown in dmesg, it would look something like:
	
	%sudo mknod char_dev 200 0

-> Execute the ioctl program
	
	%gcc -o ioctl ioctl.c
	%./ioctl

->To unload the module type:
	
	%sudo rmmod chardev
