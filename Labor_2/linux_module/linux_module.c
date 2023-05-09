/*
 * Simple example to show Kernel Module Programming
 *
 * Author: Rainer Keller, HFT-Stuttgart
 */
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

__init int init_module(void)
{
	printk (KERN_INFO "Hello world from linux_module\n");
	return 0;
}

__exit void my_cleanup_module(void)
{
}

module_exit(my_cleanup_module);

