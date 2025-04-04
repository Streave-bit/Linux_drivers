#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/cdev.h>


#define DEVICE_NAME "vled"

static dev_t dev_num;
static struct class *vled_class;
static struct device *vled_device;


static int __init vled_driver_init (void)
{
    int inr;
    inr = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (inr < 0)
    {
        pr_info("Module initilized incorrectly \n");
        return inr;
    }
    vled_class = class_create(THIS_MODULE, "vled_class");
    if (IS_ERR(vled_class)) 
    {
        pr_err("failed to create dev");
        return PTR_ERR(vled_class);
    }
    cdev_init(&vled_cdev, &fops);
    inr = cdev_add(&vled_cdev, dev_num, 1);
    if (inr)
    {
        pr_err("Failed to add dev");
        unregister_chrdev_region(dev_num, 1);
        return inr;
    }
    vled_class = class_create (THIS_MODULE, "vled_class");
    if (vled_class)
    {
        pr_err("Failed to create class");
        cdev_del(&vled_cdev);
        unregister_chrdev_region(dev_num, 1);
        return inr;
    }

    vled_device = device_create(vled_class, NULL, dev_num, NULL, "vled_device");


    pr_info("Module initilized correctly \n");
    return 0;

}

static void __exit vled_driver_exit (void)
{
    unregister_chrdev_region(dev_num, 1);
    // if (buffer)
    // {
    //     kfree(buffer);
    // }
    pr_info("Exiting the vled_driver \n");
}


module_init(vled_driver_init);
module_exit(vled_driver_exit);

MODULE_LICENSE("GPL");