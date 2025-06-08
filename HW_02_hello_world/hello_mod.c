#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

#define MAX_STR_LEN 128
#define DEFAULT_STR "Hello, World!"

void update_my_str(void);

static char my_str[MAX_STR_LEN] = DEFAULT_STR;
module_param_string(my_str, my_str, sizeof(my_str), 0444);

static char ch_val = 'H';
static int idx = 0;

static int idx_set(const char *val, const struct kernel_param *kp) {
    int new_idx;
    
    int err = kstrtoint(val, 10, &new_idx);
    if (err) {
        pr_err("Invalid integer value %s!\n", val);
        return err;
    }

    if (new_idx >= MAX_STR_LEN) {
        pr_err("Invalid idx length %d!", new_idx);
        return -EINVAL;
    }

    idx = new_idx;
    pr_info("New index of idx is %d\n", idx);

    update_my_str();

    return 0;
}

static const struct kernel_param_ops idx_ops = {
    .set = idx_set,
    .get = param_get_int
};
module_param_cb(idx, &idx_ops, &idx, 0644);

static int ch_val_set(const char *val, const struct kernel_param *kp) {

    if (!val || strlen(val) == 0) {
        pr_err("Empty character value\n");
        return -EINVAL;
    }
    
    ch_val = val[0]; 
    pr_info("ch_val set to: '%c'\n", ch_val);

    update_my_str();
    return 0;
}

static const struct kernel_param_ops ch_val_ops = {
    .set = ch_val_set,
    .get = param_get_charp
};
module_param_cb(ch_val, &ch_val_ops, &ch_val, 0644);

static int __init mod_init(void) 
{
    pr_info("init\n");
    return 0;
}

static void __exit mod_exit(void) 
{
    pr_info("exit\n");
}

void update_my_str(void) {
    strcpy(my_str, DEFAULT_STR);
    my_str[idx] = ch_val;
    pr_info("New value of my_str is %s\n", my_str);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Some user2");
MODULE_DESCRIPTION("A simple param-callback module for the Linux kernel");
