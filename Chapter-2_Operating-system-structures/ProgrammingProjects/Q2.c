/**
 * Q2.c
 *
 * A kernel module that creates entry in /proc file system and stores total time elapsed til proc called value. 
 * 
 * To compile, run makefile by entering "make". This creates several files but the kernel file we want is "Q2.ko". ensure the object file is changed accordingly.
 * 
 * To load the kernel module, run "sudo insmod Q2.ko"
 * 
 * To check if the module is loaded, run "lsmod [| grep Q2]" to see the loaded modules and "sudo dmesg | tail". You'll find the printk msg. Remember to use sudo for dmesg as it needs permission. 
 * 
 * To run the process, run "cat /proc/seconds"
 * 
 * To remove the module, run "sudo rmmod Q2". Compulsory to run this after you load the module (or else it shows the error : "insmod: ERROR: could not insert module simple.ko: File exists
")
 * 
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h> 
#include <linux/jiffies.h>
#include <asm/param.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

int startTime, endTime;

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops proc_ops = {
    .proc_read = proc_read,
};

static int proc_init(void) {
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO"Loaded /proc/%s\n", PROC_NAME);
    startTime = jiffies;
    return 0;
}

static void proc_exit(void) {
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO"Closed /proc/%s\n", PROC_NAME);
}

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    unsigned long elapsed_ms;

    if (completed) {
        completed = 0;
        return 0;
    }
    completed = 1;
    endTime = jiffies;
    elapsed_ms = jiffies_to_msecs(endTime - startTime);
    
    rv = sprintf(buffer, "Total elapsed time till process is runs: %lums\n", elapsed_ms);

    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Display total elapsed time till process is called");
MODULE_AUTHOR("SGG");