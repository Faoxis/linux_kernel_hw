#include "linux/printk.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Samoilov Sergei");
MODULE_DESCRIPTION("bin_search_demo");
MODULE_VERSION("0.1");

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
// ----------------------- 

// ----------------------- Module Initialization and Exit -------------------
static int __init init_ex_queue(void) 
{
	pr_info("Initializing\n");
	return 0;
}

static void __exit exit_ex_queue(void)
{
	pr_info("Exiting module\n");
}

module_init(init_ex_queue);
module_exit(exit_ex_queue);
