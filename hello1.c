#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Danyil Herasymchuk");
MODULE_DESCRIPTION("hello1: manages list and exports print_hello");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_event {
    struct list_head list;
    ktime_t before;
    ktime_t after;
};

static LIST_HEAD(hello_events);

/* called by hello2: perform greeting and record times */
int print_hello(void)
{
    struct hello_event *evt;

    evt = kmalloc(sizeof(*evt), GFP_KERNEL);
    if (!evt)
        return -ENOMEM;

    evt->before = ktime_get();
    pr_info("Hello, world!\n");
    evt->after = ktime_get();

    INIT_LIST_HEAD(&evt->list);
    list_add_tail(&evt->list, &hello_events);

    return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("hello1: module loaded\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct hello_event *pos, *n;

    list_for_each_entry_safe(pos, n, &hello_events, list) {
        ktime_t diff = ktime_sub(pos->after, pos->before);
        pr_info("hello1: duration = %lld ns\n", (long long)ktime_to_ns(diff));
        list_del(&pos->list);
        kfree(pos);
    }
    pr_info("hello1: module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
