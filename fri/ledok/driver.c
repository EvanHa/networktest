#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <asm/page.h>
#include <asm/hardware.h>
#include <linux/delay.h>
#include <asm/arch/regs-gpio.h>

 


#if 0
#define S3C_ADDR_BASE      0xF6000000
#define S3C_ADDR(x)        (S3C_ADDR_BASE + (x))

#define S3C2410_PA_GPIO    (0x56000000)
#define S3C2410_PA_UART    (0x50000000)
#define S3C_VA_UART        S3C_ADDR(0x01000000)

#define S3C24XX_PA_GPIO    S3C2410_PA_GPIO
#define S3C24XX_PA_UART    S3C2410_PA_UART
#define S3C24XX_VA_UART    S3C_VA_UART

#define S3C24XX_VA_GPIO    ((S3C24XX_PA_GPIO - S3C24XX_PA_UART) + S3C24XX_VA_UART)
#define S3C2410_GPIOREG(x) ((x) + S3C24XX_VA_GPIO)

#define S3C2410_GPFCON     S3C2410_GPIOREG(0x50)
#define S3C2410_GPFUP      S3C2410_GPIOREG(0x58)
#define S3C2410_GPFDAT     S3C2410_GPIOREG(0x54)
#endif

#define GPFCON            (*(volatile unsigned long *)S3C2410_GPFCON)
#define GPFDAT            (*(volatile unsigned long *)S3C2410_GPFDAT)
#define GPFUP             (*(volatile unsigned long *)S3C2410_GPFUP)

#define DEVICE_NAME       "led"
#define LED_MAJOR          240

static int     led_open(struct inode *, struct file *);
static int     led_release(struct inode *, struct file *);
static ssize_t led_read(struct file *, char *, size_t, loff_t *);
static ssize_t led_write(struct file *, const char *, size_t, loff_t *);
static void    led_display(int data);

static int led_open(struct inode *inode, struct file *file)
{
	return 0;
} 

static int led_release(struct inode *inode, struct file *file)
{
	return 0;
} 

static ssize_t led_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	return 0;
} 

static ssize_t led_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char *k_buf;

	k_buf = kmalloc(count, GFP_KERNEL);
	if(copy_from_user(k_buf, buf, count))
	{
		printk("led_write() fail!\n");
		return -EFAULT;
	}
	led_display(k_buf[0]);
	kfree(k_buf);

	return 0;
}

void led_display(int data)
{
	GPFDAT = (GPFDAT & ~(0xf << 4)) | ((~data & 0xf) << 4);
}

static struct file_operations led_fops = 
{
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release, 
	.read = led_read,
	.write = led_write,
};

static int led_init(void)
{
	int result;
	
	result = register_chrdev(LED_MAJOR, DEVICE_NAME, &led_fops);
	if(result < 0)
	{
		printk("%d character device driver can't be registered.\n", LED_MAJOR);
		return result;
	}

	GPFCON = 0x55aa;
	GPFUP = 0xff;
	led_display(0x0);

	printk("LED driver registered!\n");
	return 0;
}

static void led_exit(void)
{
	unregister_chrdev(LED_MAJOR, DEVICE_NAME);
	printk("LED driver unregistered!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
