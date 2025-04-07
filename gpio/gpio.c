#include <linux/gpio/consumer.h> // For GPIO manipulation
#include <linux/init.h>          // For module initialization
#include <linux/module.h>        // For module functions

// Declare GPIO descriptors for LED and button
static struct gpio_desc *led, *button;

// Define GPIO pin numbers
#define IO_LED1 21
#define IO_BUTTON1 20

// GPIO offset (useful if needed, otherwise can be 0)
#define IO_OFFSET 0

// Initialization function for the module
static int __init my_init(void) {
  int status;

  // Initialize LED GPIO pin
  led = gpio_to_desc(IO_LED1 + IO_OFFSET);
  if (!led) {
    printk("gpioctrl - Error with pin 21\n");
    return -ENODEV; // Return error if the LED pin is invalid
  }

  // Initialize button GPIO pin
  button = gpio_to_desc(IO_BUTTON1 + IO_OFFSET);
  if (!button) {
    printk("gpioctrl - Error with pin 20\n");
    return -ENODEV; // Return error if the button pin is invalid
  }

  // Set LED pin as output
  status = gpiod_direction_output(led, 0);
  if (status) {
    printk("gpioctrl - Error setting pin 21 as output\n");
    return status; // Return error if setting the LED pin as output fails
  }

  // Set button pin as input
  status = gpiod_direction_input(button);
  if (status) {
    printk("gpioctrl - Error setting pin 20 as input\n");
    return status; // Return error if setting the button pin as input fails
  }

  // Turn on the LED
  gpiod_set_value(led, 1);

  // Print button status (pressed or not)
  printk("gpioctrl - Button is %spressed\n",
         gpiod_get_value(button) ? "" : "not ");

  return 0; // Everything went fine
}

// Exit function for the module
static void __exit my_exit(void) {
  gpiod_set_value(led, 0); // Turn off the LED
}

// Initialize and clean up the module
module_init(my_init);
module_exit(my_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example for using GPIOs without a device tree");
