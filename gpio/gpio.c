#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
MODULE_LICENSE("GPL");

static struct proc_dir_entry *custom_proc_node;
struct proc_ops driver_proc_ops;
static char msg[] = "";

static ssize_t hello_driver_read(struct file *file_pointer,
                                 char *user_space_buffer, size_t count,
                                 loff_t *offset)

{
  int result;
  ssize_t len = strlen(msg);
  if (*offset >= len) {
    return 0;
  }
  result = copy_to_user(user_space_buffer, msg, len);
  *offset += len;
  printk("helloworld read\n");

  return len;
}

static ssize_t hello_driver_write(struct file *file_pointer,
                                  const char *user_space_buffer, size_t count,
                                  loff_t *offset) {
  int result;
  ssize_t len = strlen(msg);
  result = copy_from_user(msg, user_space_buffer, len);
  if (*offset >= len) {
    return 0;
  }
  *offset += len;
  printk("helloworld write\n");

  return len;
}

struct proc_ops hello_driver_ops = {.proc_read = hello_driver_read,
                                    .proc_write = hello_driver_write};

static int helloworld_int(void) {
  printk("We are ine the driver");
  custom_proc_node = proc_create("hello_driver", 0, NULL, &hello_driver_ops);
  printk("Driver created");
  return 0;
}

static void helloworld_exit(void) {
  printk("We are existing the driver");
  proc_remove(custom_proc_node);
  printk("Bye");
}

module_init(helloworld_int);
module_exit(helloworld_exit);