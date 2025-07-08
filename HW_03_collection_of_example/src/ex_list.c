#include "linux/gfp_types.h"
#include "linux/printk.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/moduleparam.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Samoilov Sergei");
MODULE_DESCRIPTION("list demo");
MODULE_VERSION("0.1");

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

struct list_struct 
{
	int data;
	struct list_head list;
};

static LIST_HEAD(mylist);

static int new_element = 0;
static int int_set(const char *val, const struct kernel_param *kp) 
{
    int new_el;
    int err = kstrtoint(val, 0, &new_el);
    if (err) return err;

    *(int *)kp->arg = new_el;

    struct list_struct *new_elem = kmalloc(sizeof(*new_elem), GFP_KERNEL);
    new_elem->data = new_el;
    list_add_tail(&new_elem->list, &mylist);
    pr_info("New element %d has been added to tail list\n", new_el);

    return 0;
}
static const struct kernel_param_ops int_kernel_param = {
    .set = int_set,
    .get = NULL,
};
module_param_cb(new_element, &int_kernel_param, &new_element, 0600);
MODULE_PARM_DESC(new_element, "for new element in list");

static char all_elements[1024];
static int elements_get(char *buffer, const struct kernel_param *kp) 
{
    struct list_struct *pos;
	int offset = 0;

	list_for_each_entry(pos, &mylist, list) {
		offset += snprintf(buffer + offset, PAGE_SIZE - offset, "%d,", pos->data);
		if (offset >= PAGE_SIZE)
			break;
	}
	if (offset > 0)
		buffer[offset - 1] = '\n'; 
	else
		buffer[0] = '\0';

	return offset;
}
static const struct kernel_param_ops all_elements_param = {
	.get = elements_get,
	.set = NULL,  
};

// добавляем параметр all_elements, только для чтения: 0444
module_param_cb(all_elements, &all_elements_param, &all_elements, 0444);
MODULE_PARM_DESC(all_elements, "all elements in list");

static int get_last = 0;
static int get_last_element(char *buffer, const struct kernel_param *kp) 
{
	if (list_empty(&mylist)) 
	{
		pr_info("Getting last element from empty list\n");
		return snprintf(buffer, PAGE_SIZE, "EMPTY\n");
	}

	struct list_struct *last = list_last_entry(&mylist, struct list_struct, list);
	pr_info("Getting last element (%d) from list\n", last->data);
	return snprintf(buffer, PAGE_SIZE, "%d\n", last->data);
}
static const struct kernel_param_ops last_element_param = {
    .get = get_last_element,
    .set = NULL,
};

module_param_cb(get_last, &last_element_param, &get_last, 0444);
MODULE_PARM_DESC(get_last, "get last element in list");

static int get_first = 0;
static int get_first_element(char *buffer, const struct kernel_param *kp) 
{
	if (list_empty(&mylist))
	{
		pr_info("Getting first element from empty list\n");
		return snprintf(buffer, PAGE_SIZE, "EMPTY\n");
	}

	struct list_struct *first = list_first_entry(&mylist, struct list_struct, list);
	pr_info("Getting first element (%d) from list\n", first->data);
	return snprintf(buffer, PAGE_SIZE, "%d\n", first->data);
}
static const struct kernel_param_ops first_element_param = {
    .get = get_first_element,
    .set = NULL,
};
module_param_cb(get_first, &first_element_param, &get_first, 0444);
MODULE_PARM_DESC(get_first, "get first element in list");


static int __init init_ex_list(void) 
{
	pr_info("Initializing\n");
	return 0;
}

static void __exit exit_ex_list(void)
{
	struct list_struct *pos, *n;
    list_for_each_entry_safe(pos, n, &mylist, list) {
        list_del(&pos->list);
        kfree(pos);
    }
	pr_info("List has been cleared. Exiting module\n");
}

module_init(init_ex_list);
module_exit(exit_ex_list);
