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
static int set_slot(const char *val, const struct kernel_param *kp) {
    int slot;
    if (kstrtoint(val, 0, &slot)) {
        pr_warn("Error parsing int value");
        return -EINVAL;
    }
    
    if (slot < 0 || slot >= BITMAP_SIZE) {
        pr_warn("Invalid slot number: %d\n", slot);
    }
    
    if (test_bit(slot, bitmap)) {
        pr_info("Slot %d is already set\n", slot);
        return 0;
    }

    set_bit(slot, bitmap);
    pr_info("Slot %d set to 1\n", slot);

    return 0;
}
static struct kernel_param_ops set_slot_param = {
    .set = set_slot,
    .get = NULL,
};
module_param_cb(set_slot, &set_slot_param, NULL, 0200);
MODULE_PARM_DESC(set_slot, "Setting slot to 1");

// ----------------------- Reset slot ---------------------------------------
static int clear_slot(const char *val, const struct kernel_param *kp) 
{
    int slot;
        if (kstrtoint(val, 0, &slot)) {
        pr_warn("Error parsing int value\n");
        return -EINVAL;
    }
    
    if (slot < 0 || slot >= BITMAP_SIZE) {
        pr_warn("Invalid slot number: %d\n", slot);
    }
    
    if (!test_bit(slot, bitmap)) {
        pr_info("Slot %d is already cleared\n", slot);
        return 0;
    }

    clear_bit(slot, bitmap);
    pr_info("Slot %d set to 0\n", slot);

    return 0;
}
static struct kernel_param_ops clear_slot_param = {
    .set = clear_slot,
    .get = NULL,
};
module_param_cb(clear_slot, &clear_slot_param, NULL, 0200);
MODULE_PARM_DESC(clear_slot, "Setting slot to 0");

// ----------------------- count slots --------------------------------------
static int count_slots(char *buffer, const struct kernel_param *kp) {
    return scnprintf(buffer, PAGE_SIZE, "%d\n", bitmap_weight(bitmap, BITMAP_SIZE));
}
static struct kernel_param_ops count_slots_ops = {
    .set = NULL,
    .get = count_slots,
};
module_param_cb(count_slots, &count_slots_ops, NULL, 0444);
MODULE_PARM_DESC(count_slots, "Counting slots");

// ---------------------- show slots ----------------------------------------
static int show_slots(char *buffer, const struct kernel_param *kp)
{
    int len = 0;
    for (int i = 0; i < BITMAP_SIZE; ++i) {
        len += scnprintf(buffer + len, PAGE_SIZE - 1, "%d", test_bit(BITMAP_SIZE - i - 1, bitmap));
    }
    len += scnprintf(buffer + BITMAP_SIZE, PAGE_SIZE, "\n");
    return len;
}
static struct kernel_param_ops show_slots_ops = {
    .set = NULL,
    .get = show_slots,
};
module_param_cb(show_slots, &show_slots_ops, NULL, 0444);
MODULE_PARM_DESC(show_slots, "Showing slots state");

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
