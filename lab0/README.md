# A Kernel Seedling

This project implements a Linux kernel module that creates a `/proc/count` file to display the total number of running processes in the system. The module demonstrates basic kernel programming concepts including:
- Creating and managing procfs entries
- Accessing system information
- Kernel module initialization and cleanup

## Building

To build the kernel module:
```shell
make
```
This will create the `proc_count.ko` module file.

## Running

To load the kernel module:
```shell
sudo insmod proc_count.ko
```

To view the number of running processes:
```shell
cat /proc/count
```
You should see output like:
```
157
```

## Cleaning Up

To remove the kernel module and clean build files:
```shell
sudo rmmod proc_count
make clean
```

## Testing

Run the test suite:
```shell
python -m unittest
```

## System Information

Please report the kernel release version you tested your module on. You can get this information using:
```shell
uname -r -s -v
```

Example output:
```
Linux 5.14.8-arch1-1 #1 SMP PREEMPT Sun, 26 Sep 2021 19:36:15 +0000
```

Make sure this matches a release number from https://www.kernel.org/.
