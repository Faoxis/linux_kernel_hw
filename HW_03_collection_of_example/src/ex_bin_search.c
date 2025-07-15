#include "linux/printk.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/sort.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Samoilov Sergei");
MODULE_DESCRIPTION("bin_search_demo");
MODULE_VERSION("0.1");

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
// -----------------------

// ----------------------- Start parameters ---------------------------------
#define MAX_ARRAY_LEN 128
#define MAX_STRING_LEN 1024

static int *array = NULL;
static size_t array_len = 0;

// ----------------------- Getting array ------------------------------------
static int compare_int(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}
static int set_array_param(const char *val, const struct kernel_param *kp) {
  char *input, *token, *p;
  int *tmp = NULL;
  size_t count = 0;
  long n;
  int ret = 0;

  input = kstrdup(val, GFP_KERNEL);
  if (!input)
    return -ENOMEM;

  // Максимум MAX_ARRAY_LEN элементов
  tmp = kmalloc_array(MAX_ARRAY_LEN, sizeof(int), GFP_KERNEL);
  if (!tmp) {
    kfree(input);
    return -ENOMEM;
  }

  p = input;
  while ((token = strsep(&p, ",")) && count < MAX_ARRAY_LEN) {
    ret = kstrtol(token, 10, &n);
    if (ret)
      break;
    tmp[count++] = n;
  }

  if (ret == 0 && count > 0) {
    kfree(array);
    array = tmp;
    array_len = count;

    sort(array, array_len, sizeof(int), compare_int, NULL);

    pr_info("ex_sort_array: set %zu elements\n", array_len);
  } else {
    kfree(tmp);
  }

  kfree(input);
  return ret;
}
static const struct kernel_param_ops array_param_ops = {
    .set = set_array_param,
};
module_param_cb(write_param, &array_param_ops, NULL, 0200); // -w only
MODULE_PARM_DESC(write_param, "Comma-separated int array to be sorted");

// ----------------------- Sorting result -----------------------------------
static int get_array_param(char *buffer, const struct kernel_param *kp) {
  int len = 0;
  size_t i;

  if (!array || array_len == 0)
    return sprintf(buffer, "[]\n");

  for (i = 0; i < array_len; i++) {
    len += scnprintf(buffer + len, PAGE_SIZE - len, "%d%s", array[i],
                     (i == array_len - 1) ? "\n" : ",");
  }

  pr_info("getting sorting result");
  return len;
}
static const struct kernel_param_ops sorted_param_ops = {
    .get = get_array_param,
};
module_param_cb(read_param, &sorted_param_ops, NULL, 0400); // -r only
MODULE_PARM_DESC(read_param, "Sorted array as comma-separated string");

// ----------------------- Module Initialization and Exit -------------------
static int __init init_ex_sort(void) {
  pr_info("Initializing\n");
  return 0;
}

static void __exit exit_ex_sort(void) { pr_info("Exiting module\n"); }

module_init(init_ex_sort);
module_exit(exit_ex_sort);
