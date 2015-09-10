/*
   Hello, World Module Program
*/

#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void)
{
    printk("Hello,Linux World!!\n");

    return 0;
}

static void hello_exit(void)
{
    printk("Goodbye,Cruel World!!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
