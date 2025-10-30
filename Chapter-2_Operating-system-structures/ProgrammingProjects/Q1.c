/**
 * Q1.c
 *
 * A kernel module that creates entry in /proc file system and stores jiffies value. 
 * 
 * To compile, run makefile by entering "make". This creates several files but the kernel file we want is "Q1.ko". ensure the object file is changed accordingly.
 * 
 * To load the kernel module, run "sudo insmod Q1.ko"
 * 
 * To check if the module is loaded, run "lsmod [| grep Q1]" to see the loaded modules and "sudo dmesg | tail". You'll find the printk msg. Remember to use sudo for dmesg as it needs permission. 
 * 
 * To run the process, run "cat /proc/jiffies"
 * 
 * To remove the module, run "sudo rmmod simple". Compulsory to run this after you load the module (or else it shows the error : "insmod: ERROR: could not insert module simple.ko: File exists
")
 * 
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h> /* Need this header file to do all /proc file system related operations */
#include <asm/uaccess.h> /* Need this to safkey copy data from kernel space to user space */

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies" /* Process name defined here i.e. /proc/jiffies */

/* function prototype
 * file is a pointer to file structure for /proc/jiffies
 *  user_buf is a pointer to user space buffer to which data is copied from the kernel buffer
 * count is number of bytes the user requested
 * pos keeps track of file read position like a cursor
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

/* This structure tells kernel how to handle operations on /proc/jiffies
 * Since recent updates need to use proc_ops instead of file_operations structure 
 */
static struct proc_ops proc_ops = {
        .proc_read = proc_read, /* Every time /proc/jiffies is read, the kernel calls proc_read() */
};

/* Function called when module is loaded
 * proc_create makes an entry on the /proc file system with the parameters
 * PROC_NAME is name of process
 * 0 is the permmissions
 * NULL is the parent directory (since NULL it's root itself i.e. /proc)
 * proc_ops is the operations structure we created
 */
static int proc_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* Function called when module is removed
 * removes entry from /proc file system
 */
static void proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* Kernel calls proc_read()
 * completed being a static variable ensures that only one read happens, else cat will call proc_read() indefinetely i.e. until 0 return value is gotten
 * So first completed = 0, makes it completed = 1 and stores jiffies value in user buffer. in next operation, completed = 1 and so is reset and returned EOF
 * sprintf() writes to kernel buffer and return number of bytes returned which is stored in rv
 * copy_to_user defined is asm/uaccess.h safely writes data to user buffer
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "jiffies: %lu\n", jiffies);
    
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Display jiffies in /proc/jiffies");
MODULE_AUTHOR("SGG");