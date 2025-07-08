#include "linux/gfp_types.h"
#include "linux/printk.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/bitmap.h>
#include <linux/moduleparam.h>
#include <linux/string.h>

// ------------------- start info ----------------------
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Samoilov Sergei");
MODULE_DESCRIPTION("bitmap demo");
MODULE_VERSION("0.1");

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

// ------------------ bitmap difinition -----------------
#define BITMAP_SIZE 64
static DECLARE_BITMAP(bitmap, BITMAP_SIZE);

// ------------------- set slot -------------------------
static int set_slot_cb(const char *val, const struct kernel_param *kp) {
    
    return 0;
}


// ----------------------- Module Initialization and Exit -------------------
static int __init init_ex_list(void) 
{
	pr_info("Initializing\n");
	return 0;
}

static void __exit exit_ex_list(void)
{
	pr_info("Exiting module\n");
}

module_init(init_ex_list);
module_exit(exit_ex_list);
