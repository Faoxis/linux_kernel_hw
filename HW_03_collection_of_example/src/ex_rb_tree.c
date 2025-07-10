#include "linux/printk.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Samoilov Sergei");
MODULE_DESCRIPTION("rb_tree_demo");
MODULE_VERSION("0.1");

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

// ------------------ rb tree difinition -----------------
struct rb_node_data {
    int value;
    struct rb_node node;
};

static struct rb_root my_tree = RB_ROOT;
// static int last_search_result = 0;

// ---------------------------- Insertion -----------------------------------
static int add_node(const char *val, const struct kernel_param *kp)
{
    int value;
    if (kstrtoint(val, 0, &value)) {
        pr_warn("error parsing inpute value");
        return -EINVAL;
    }

    struct rb_node **new = &my_tree.rb_node;
    struct rb_node *parent = NULL;
    while (*new) {
        struct rb_node_data *this = container_of(*new, struct rb_node_data, node);

        parent = *new;
        if (value < this->value) new = &((*new)->rb_left);
        else if (value > this->value) new = &((*new)->rb_right);
        else return 0;
    }

    struct rb_node_data *entry = kmalloc(sizeof(*entry), GFP_KERNEL);
    if (!entry) return -ENOMEM;
    
    entry->value = value;
    rb_link_node(&entry->node, parent, new);
    rb_insert_color(&entry->node, &my_tree);

    pr_info("Inserted value: %d\n", value);
    return 0;
}
static struct kernel_param_ops add_node_ops = {
    .set = add_node,
    .get = NULL,
};
module_param_cb(add_node, &add_node_ops, NULL, 0200);
MODULE_PARM_DESC(add_node, "Adding new node");

// ------------------------ Showing Tree ------------------------------------
static int show_tree(char *buffer, const struct kernel_param *kp)
{
    int len = 0;
    struct rb_node *node;

    for (node = rb_first(&my_tree); node; node = rb_next(node)) {
        struct rb_node_data *entry = container_of(node, struct rb_node_data, node);
        len += scnprintf(buffer + len, PAGE_SIZE - len, "%d,", entry->value);
    }

    if (len > 0) buffer[len - 1] = '\n';
    else len += scnprintf(buffer + len, PAGE_SIZE - len, "\n");

    pr_info("Showing tree\n");
    return len;
}
static struct kernel_param_ops show_tree_ops = {
    .set = NULL,
    .get = show_tree,
};
module_param_cb(show_tree, &show_tree_ops, NULL, 0444);
MODULE_PARM_DESC(show_tree, "Showing tree");

// ----------------------- Searching in tree --------------------------------
static bool found_node = false;
static int search_data(const char *val, const struct kernel_param *kp) 
{
    int searched;
    if (kstrtoint(val, 0, &searched)) {
        pr_warn("error parsing inpute value");
        return -EINVAL;
    }
    
    struct rb_node *node = my_tree.rb_node;
    found_node = false;
    
    while (node) {
        struct rb_node_data *entry = container_of(node, struct rb_node_data, node);
        if (searched < entry->value) node = node->rb_left;
        else if (searched > entry->value) node = node->rb_right;
        else {
            found_node = true;
            break;
        }

    }
    pr_info("Searching data: %d\n", searched);
    return 0;
}
static struct kernel_param_ops search_data_ops = {
    .set = search_data,
    .get = NULL,
};
module_param_cb(search_data, &search_data_ops, NULL, 0200);
MODULE_PARM_DESC(search_data, "Searching data in the tree");

static int search_data_result(char *buffer, const struct kernel_param *kp)
{
    char *result = found_node ? "found" : "not found";
    pr_info("Getting result of searching: %s\n", result);
    return scnprintf(buffer, PAGE_SIZE, "%s\n", result);
}
static struct kernel_param_ops search_data_result_ops = {
    .set = NULL,
    .get = search_data_result,
};
module_param_cb(search_data_result, &search_data_result_ops, NULL, 0444);
MODULE_PARM_DESC(search_data_result, "Searching result");

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

