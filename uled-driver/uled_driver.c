#include <linux/module.h>      // Required for modules
#include <linux/fs.h>          // Required for file operations
#include <linux/init.h>        // For module initialization and cleanup
#include <linux/uaccess.h>     // For user space access functions
#include <linux/cdev.h>        // For character device operations

#define DEVICE_NAME "uled"     // Define the device name

// Global variables for device status
static char led_status = '0';   // LED status (0 for OFF, 1 for ON)
static dev_t dev_num;           // Device number
static struct cdev uled_cdev;   // Character device structure
static struct class *uled_class; // Device class

// Read function for the virtual LED device
static ssize_t uled_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    // Simple read function returning the current LED status
    return simple_read_from_buffer(user_buffer, count, offset, &led_status, 1);
}

// Write function for the virtual LED device
static ssize_t uled_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *offset)
{
    char buffer[2] = {0};

    // Copy data from user space to buffer
    if (copy_from_user(buffer, user_buffer, min_t(size_t, count, sizeof(buffer) - 1)))
    {
        return -EFAULT; // Return error if copy fails
    }

    // Change LED status based on user input
    if (buffer[0] == '1')
    {
        led_status = '1'; // Turn on the LED
        pr_info("uled: LED ON\n");
    }
    else if (buffer[0] == '0')
    {
        led_status = '0'; // Turn off the LED
        pr_info("uled: LED OFF\n");
    }

    return count; // Return the number of bytes written
}

// Define the file operations structure
static struct file_operations fops = 
{
    .owner = THIS_MODULE,    // Set the owner to this module
    .read = uled_read,       // Assign read operation
    .write = uled_write,     // Assign write operation
};

// Initialization function for the module
static int __init uled_init(void)
{
    // Allocate a device number dynamically
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

    // Initialize and add the character device
    cdev_init(&uled_cdev, &fops);
    cdev_add(&uled_cdev, dev_num, 1);

    // Create a class for the device
    uled_class = class_create("uled_class");

    // Create the device under /dev
    device_create(uled_class, NULL, dev_num, NULL, DEVICE_NAME);

    pr_info("uled: Module initialized, use /dev/uled\n");
    return 0; // Return 0 to indicate successful initialization
}

// Exit function for the module
static void __exit uled_exit(void)
{
    // Destroy the device and class
    device_destroy(uled_class, dev_num);
    class_destroy(uled_class);

    // Remove the character device and unregister the device number
    cdev_del(&uled_cdev);
    unregister_chrdev_region(dev_num, 1);

    pr_info("uled: Module removed\n");
}

// Register the initialization and cleanup functions
module_init(uled_init);
module_exit(uled_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Virtual LED Driver");
