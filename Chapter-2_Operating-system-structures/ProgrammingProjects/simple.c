/**
 * simple.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make". This creates several files but the kernel file we want is "simple.ko".
 * 
 * To load the kernel module, run "sudo insmod simple.ko"
 * 
 * To check if the module is loaded, run "lsmod [| grep simple]" to see the loaded modules and "sudo dmesg | tail". You'll find the printk msg. Remember to use sudo for dmesg as it needs permission. 
 * 
 * To remove the module, run "sudo rmmod simple". Compulsory to run this after you load the module (or else it shows the error : "insmod: ERROR: could not insert module simple.ko: File exists
")
 * 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/* This function is called when the module is loaded. */
static int simple_init(void)
{
       printk(KERN_INFO "Loading Custom Module\n");

       return 0;
}

/* This function is called when the module is removed. */
static void simple_exit(void) {
	printk(KERN_INFO "Removing Custom Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
