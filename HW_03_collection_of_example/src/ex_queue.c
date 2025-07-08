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
MODULE_DESCRIPTION("queue demo");
MODULE_VERSION("0.1");

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define QUEUE_SIZE 256
static DEFINE_KFIFO(fifo, int, QUEUE_SIZE);

// --------------------------- INPUT ---------------------------------------
static int input_set(const char *val, const struct kernel_param *kp) 
{
    int new_data;
    int result = kstrtoint(val, 0, &new_data);
    if (result) {
        pr_warn("Error %d parsing input param\n", result);
        return result;
    }

    if (!kfifo_avail(&fifo)) {
        pr_info("Queue is full. Value %d can't be written\n", new_data);
        return -ENOSPC;
    }

    kfifo_in(&fifo, &new_data, 1);
    pr_info("kfifo_param_module: pushed %d (size=%u)\n", new_data, kfifo_len(&fifo));
    return 0;
}
static const struct kernel_param_ops input_ops = {
    .set = input_set,
    .get = NULL,
};
module_param_cb(input, &input_ops, NULL, 0200);
MODULE_PARM_DESC(input, "Input for the queue");

// --------------------------- OUTPUT ---------------------------------------
static int output_get(char *buffer, const struct kernel_param *kp) 
{
    int out;
    if (!kfifo_out(&fifo, &out, 1)) {
        pr_info("Getting element %d from queue\n", out);
        return scnprintf(buffer, PAGE_SIZE, "(empty)");
    }
    pr_info("Getting element %d from queue\n", out);
    return scnprintf(buffer, PAGE_SIZE, "%d\n", out);
}
static const struct kernel_param_ops output_ops = {
    .set = NULL,
    .get = output_get,
};
module_param_cb(output, &output_ops, NULL, 0400);
MODULE_PARM_DESC(output, "Output from the queue");

// ---------------------------- SIZE ----------------------------------------
static int get_size(char *buffer, const struct kernel_param *kp)
{
    return scnprintf(buffer, PAGE_SIZE, "%d\n", kfifo_len(&fifo));
}
static const struct kernel_param_ops size_ops = {
    .set = NULL,
    .get = get_size,
};
module_param_cb(size, &size_ops, NULL, 0400);
MODULE_PARM_DESC(size, "Size of queue");

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
