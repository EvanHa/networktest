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
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/page.h>
#include <asm/hardware.h>
#include <linux/delay.h>



#define DEVICE_NAME   "key"
#define KEY_MAJOR  240

static int key_open(struct inode *, struct file *);
static int key_release(struct inode *, struct file *);
static ssize_t key_read(struct file *, char *, size_t, loff_t *);
static ssize_t key_write(struct file *, const char *, size_t, loff_t *);
static pid_t id;

static irqreturn_t key1_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	printk("KEY1 pressed.. irq[%d]\n",irq);
	printk("kernel to user SIG = %d\n",(int)id);
	kill_proc(id, SIGUSR1,1);
	
	return IRQ_HANDLED;
}
static irqreturn_t key2_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	printk("KEY2 pressed.. irq[%d]\n",irq);
	printk("kernel to user SIG = %d\n",(int)id);
	kill_proc(id, SIGUSR1,1);

	return IRQ_HANDLED;
}
static irqreturn_t key3_isr(int irq, void *dev_id, struct pt_regs *regs)
{	

	printk("KEY3 pressed.. irq[%d]\n",irq);
	printk("kernel to user SIG = %d\n",(int)id);
	kill_proc(id, SIGUSR1,1);

	return IRQ_HANDLED;
}
static irqreturn_t key4_isr(int irq, void *dev_id, struct pt_regs *regs)
{
	printk("KEY4 pressed.. irq[%d]\n",irq);
	printk("kernel to user SIG = %d\n",(int)id);
	kill_proc(id, SIGUSR1,1);

	return IRQ_HANDLED;
}

static int __init el2440_keybutton_probe(struct device *dev)
{

	int ret[4];
	int ret_b;

	printk("EL2440 external key button probe!!\n");

	ret[0] = request_irq(IRQ_EINT0,
			  (void *)key1_isr,
			  IRQF_DISABLED | IRQF_TRIGGER_FALLING,
			  "KEY1", NULL);
	ret[1] = request_irq(IRQ_EINT2,
			  (void *)key2_isr,
			  IRQF_DISABLED | IRQF_TRIGGER_FALLING,
			  "KEY2", NULL);
	ret[2] = request_irq(IRQ_EINT8,
			  (void *)key3_isr,
			  IRQF_DISABLED | IRQF_TRIGGER_FALLING,
			  "KEY3", NULL);
	ret[3] = request_irq(IRQ_EINT19,
			  (void *)key4_isr,
			  IRQF_DISABLED | IRQF_TRIGGER_FALLING,
			  "KEY4", NULL);

	if (ret[0] || ret[1] || ret[2] || ret[3]) {
		printk("EL2440 keybutton: request IRQ failed!!\n");
		return ret[0];
	}

	return 0;
}


static int el2440_keybutton_remove(struct device *dev)
{
	printk("EL2440 external key button remove!!\n");
	free_irq(IRQ_EINT0, NULL);
	free_irq(IRQ_EINT2, NULL);
	free_irq(IRQ_EINT8, NULL);
	free_irq(IRQ_EINT19, NULL);
	return 0;
}

static int key_open(struct inode *inode, struct file *file)
{
  printk("key opened !!\n");

  return 0;
}

static int key_release(struct inode *inode, struct file *file)
{
  printk("key released !!\n");
  return 0;
}

static ssize_t key_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
  printk("key_read is invoked\n");
  return (0);

}

static ssize_t key_write(struct file *filp,const char __user *buf, size_t count,
loff_t *f_pos)
{
  get_user(id, (int *)buf);
  printk("key_write is invoked =%d\n", id);

  return (0);
}

static struct device_driver el2440keybtn_driver = {
	.name		= "el2440-keybutton",
	.owner		= THIS_MODULE,
	.bus		= &platform_bus_type,
	.probe		= el2440_keybutton_probe,
	.remove		= el2440_keybutton_remove,
};

static struct file_operations key_fops = {
	.owner   	= THIS_MODULE,
   	.read	= key_read,
    	.write   	= key_write,
	.open    	= key_open,
	.release 	= key_release,
};


int __init el2440_keybutton_init(void)
{
  int result = register_chrdev(KEY_MAJOR,DEVICE_NAME,&key_fops);

  if(result < 0) {
     printk("key: %d character device driver can't be registered\n", KEY_MAJOR);
     return result;
    }
	return driver_register(&el2440keybtn_driver);
}

void __exit el2440_keybutton_exit(void)
{
	unregister_chrdev(KEY_MAJOR, DEVICE_NAME);
	driver_unregister(&el2440keybtn_driver);
}

module_init(el2440_keybutton_init);
module_exit(el2440_keybutton_exit);

MODULE_LICENSE("GPL");
