#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

// Declare the proc node
static struct proc_dir_entry *custom_proc_node;
struct proc_ops driver_proc_ops;
static char msg[] = "";  // Default message

// Read function for the proc file
static ssize_t hello_driver_read(struct file *file_pointer,
                                  char *user_space_buffer,
                                  size_t count,
                                  loff_t *offset)
{
    int result;
    ssize_t len = strlen(msg);

    // If the offset is greater than or equal to the message length, return 0
    if (*offset >= len) {
        return 0;
    }

    // Copy the message to the user space buffer
    result = copy_to_user(user_space_buffer, msg, len);
    *offset += len;  // Update the offset
    printk("helloworld read\n");

    return len;  // Return the size of the read message
}

// Write function for the proc file
static ssize_t hello_driver_write(struct file *file_pointer,
                                   const char *user_space_buffer,
                                   size_t count,
                                   loff_t *offset)
{
    int result;
    ssize_t len = strlen(msg);

    // Copy the message from user space
    result = copy_from_user(msg, user_space_buffer, len);

    // If the offset exceeds the message length, stop writing
    if (*offset >= len) {
        return 0;
    }
    
    *offset += len;  // Update the offset
    printk("helloworld write\n");

    return len;  // Return the size of the written data
}

// Define proc operations
struct proc_ops hello_driver_ops = {
    .proc_read = hello_driver_read,
    .proc_write = hello_driver_write
};

// Initialization function for the module
static int helloworld_int(void)
{
    printk("We are in the driver\n");
    // Create the proc node "hello_driver"
    custom_proc_node = proc_create("hello_driver", 0, NULL, &hello_driver_ops);
    printk("Driver created\n");
    return 0;
}

// Exit function for the module
static void helloworld_exit(void)
{
    printk("We are exiting the driver\n");
    // Remove the proc node
    proc_remove(custom_proc_node);
    printk("Bye\n");
}

// Initialize and cleanup the module
module_init(helloworld_int);
module_exit(helloworld_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Helloworld driver with wrie and read functions");