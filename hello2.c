#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("hello2: calls print_hello N times");
MODULE_LICENSE("Dual BSD/GPL");

static uint howmany = 1;
module_param(howmany, uint, S_IRUGO);
MODULE_PARM_DESC(howmany, "Number of times to call print_hello (uint)");

static int __init hello2_init(void)
{
    unsigned int i;
    int ret;

    if (howmany > 10) {
        pr_err("hello2: howmany=%u > 10 -> refusing to load\n", howmany);
        return -EINVAL;
    }

    if (howmany == 0 || (howmany >= 5 && howmany <= 10)) {
        pr_warn("hello2: howmany=%u is special (0 or 5..10)\n", howmany);
    }

    for (i = 0; i < howmany; i++) {
        ret = print_hello();
        if (ret) {
            pr_err("hello2: print_hello failed on iteration %u (err=%d)\n", i, ret);
            return ret;
        }
    }

    pr_info("hello2: loaded, called print_hello %u times\n", howmany);
    return 0;
}

static void __exit hello2_exit(void)
{
    pr_info("hello2: unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
