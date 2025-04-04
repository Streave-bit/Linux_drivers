#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>



#define DEVICE_NAME "uled"

static char led_status = '0';
static dev_t dev_num;
static struct cdev uled_cdev;
static struct class *uled_class;

static ssize_t uled_read (struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    return simple_read_from_buffer(user_buffer, count, offset, &led_status,1);
}

static ssize_t uled_write (struct file *file, const char __user *user_buffer, size_t count, loff_t *offset)
{
    char buffer[2]={0};

    if(copy_from_user(buffer, user_buffer, min_t(size_t, count, sizeof(buffer)-1)))
    {
        return -EFAULT;
    }

    if (buffer[0]=='1')
    {
        led_status='1';
        pr_info("uled: LED ON\n");
    }
    else if (buffer[0]=='0')
    {
        led_status='0';
        pr_info("uled: LED OFF\n");
    }
    return count;
}

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .read = uled_read,
    .write = uled_write,
};

static int __init uled_init(void) {
    alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    cdev_init(&uled_cdev, &fops);
    cdev_add(&uled_cdev, dev_num, 1);
    uled_class = class_create(THIS_MODULE, "uled_class");
    device_create(uled_class, NULL, dev_num, NULL, DEVICE_NAME);
    pr_info("uled: Module initialized, use /dev/uled\n");
    return 0;
}

static void __exit uled_exit (void){
    device_destroy(uled_class, dev_num);
    class_destroy(uled_class);
    cdev_del(&uled_cdev);
    unregister_chrdev_region(dev_num,1);
    pr_info("uled: Module removed\n");
}

module_init(uled_init);
module_exit(uled_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("me");
MODULE_DESCRIPTION("Driver LED VIRTUELLE");