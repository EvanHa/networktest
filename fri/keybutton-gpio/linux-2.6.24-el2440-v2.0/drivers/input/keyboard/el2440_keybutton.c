/********************************************************************
 *
 * el2440_keybutton.c
 *
 * EL2440 key button device driver
 *
 *    EINT0  - KEY 1
 *    EINT2  - KEY 2
 *    EINT8  - KEY 3
 *    EINT19 - KEY 4
 *
 * 2008-11-05 : eLayer System G.B.H.
 *
 ********************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/device.h>

#include <asm/arch/regs-gpio.h>


static irqreturn_t key1_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	printk("KEY1 pressed.. irq[%d]\n",irq);

	return IRQ_HANDLED;
}


static int __init el2440_keybutton_probe(struct device *dev)
{

	int ret;

	printk("EL2440 external key button probe!!\n");

	ret = request_irq(IRQ_EINT0,
			  (void *)key1_isr,
			  IRQF_DISABLED | IRQF_TRIGGER_FALLING,
			  "KEY1", NULL);

	if (ret) {
		printk("EL2440 keybutton: request IRQ failed!!\n");
		return ret;
	}

	return 0;
}


static int el2440_keybutton_remove(struct device *dev)
{
	printk("EL2440 external key button remove!!\n");
	free_irq(IRQ_EINT0, NULL);
	return 0;
}

static struct device_driver el2440keybtn_driver = {
	.name		= "el2440-keybutton",
	.owner		= THIS_MODULE,
	.bus		= &platform_bus_type,
	.probe		= el2440_keybutton_probe,
	.remove	= el2440_keybutton_remove,
};


int __init el2440_keybutton_init(void)
{
	return driver_register(&el2440keybtn_driver);
}

void __exit el2440_keybutton_exit(void)
{
	driver_unregister(&el2440keybtn_driver);
}

module_init(el2440_keybutton_init);
module_exit(el2440_keybutton_exit);

MODULE_LICENSE("GPL");
