#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	pr_info("Hello, World from the kernel!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Goodbye, World from the kernel!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OldALoneFag");
MODULE_DESCRIPTION("A simple Hello World module for the Linux kernel");
