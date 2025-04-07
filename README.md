# Linux Kernel Driver Examples

This repository contains a set of simple Linux kernel module examples for learning and testing purposes. These drivers demonstrate basic interactions with the Linux kernel, including character devices, GPIOs, and the `/proc` filesystem.

## Contents

### 1. `uled-driver/` - Virtual LED Character Device
A basic character device driver that simulates a virtual LED.  
You can write `'1'` or `'0'` to `/dev/uled` to turn the virtual LED on or off.  
Reading from the device returns the current LED state.

### 2. `gpio-driver/` - GPIO Button and LED Control
A simple GPIO driver that uses raw GPIO numbers to configure:
- One pin as an output (LED)
- One pin as an input (Button)  
It prints the button state and toggles the LED during initialization.

### 3. `procfs-driver/` - Proc Filesystem Interface
A lightweight driver that creates a custom `/proc/hello_driver` node.  
It supports reading and writing basic strings to and from user space using the procfs interface.

---

## CI/CD Pipeline

This project includes a GitHub Actions CI/CD workflow to:
- **Build the drivers** in a Docker container with a compatible kernel
- **Run static analysis** using `cppcheck`
- Optionally add support for **clang-format** to enforce code style

### Why CI/CD?
Continuous Integration and Continuous Deployment (CI/CD) helps:
- Automatically verify your code with every push
- Catch build and style issues early
- Maintain consistent code quality across the team

The Docker container is configured with a kernel headers environment that mimics a real Linux system for consistent driver compilation.

---

## Requirements

To build and test locally:
- Linux system (Ubuntu 20.04 or 22.04 recommended)
- Kernel headers installed (`linux-headers-$(uname -r)`)
- `make`, `gcc`, and optionally `cppcheck` or `clang-format`

---

## Usage

To build any driver, simply:

```bash
cd <driver-folder>
make

