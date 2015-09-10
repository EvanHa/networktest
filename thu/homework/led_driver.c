/* 
   el2440_led.c
   EL2440 LED Test Driver 
*/

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

#define GPFCON *(volatile unsigned long *)S3C2410_GPFCON
#define GPFDAT *(volatile unsigned long *)S3C2410_GPFDAT
#define GPFUP *(volatile unsigned long *)S3C2410_GPFUP

#define DEVICE_NAME   "led"
#define LED_MAJOR  240

static int led_open(struct inode *, struct file *);
static int led_release(struct inode *, struct file *);
static ssize_t led_read(struct file *, char *, size_t, loff_t *);
static ssize_t led_write(struct file *, const char *, size_t, loff_t *);
void Led_Display(int data);


static int led_open(struct inode *inode, struct file *file)
{
  printk("led opened !!\n");
  return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
  printk("led released !!\n");
  return 0;
}

static ssize_t led_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
  printk("led_read is invoked\n");
  return (0);
}

static ssize_t led_write(struct file *filp,const char __user *buf, size_t count,
                            loff_t *f_pos)
{
  int i;
  char *k_buf;
  k_buf = kmalloc(count, GFP_KERNEL);
  if(copy_from_user(k_buf, buf, count)) {
	  return -EFAULT;
  }

  char number = *k_buf;
  int mode = 0;
  switch(number) {
	  case '1':
		  mode = 1;
		  break;
	  case '2':
		  mode = 2;
		  break;
	  case '3':
		  mode = 3;
		  break;
	  case '4':
		  mode = 4;
		  break;
	  default:
		  break;
  }
  Led_Display(mode);
  return (0);
}

void Led_Display(int data)
{
 // Active is low.(LED On)
 // GPF7    GPF6   GPF5   GPF4
 // nLED_8 nLED4 nLED_2 nLED_1
 
  GPFDAT = (GPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4);   
  printk("Led_Display : 0x%02x\n",data);   
}


static struct file_operations led_fops = {
	.owner   = THIS_MODULE,
   	.read	 = led_read,
   	.write   = led_write,
	.open    = led_open,
	.release = led_release,
};

static int led_init(void)
{
  int result = register_chrdev(LED_MAJOR,DEVICE_NAME,&led_fops);
  if(result < 0) {
     printk("led: %d character device driver can't be registered\n", LED_MAJOR);
     return result;
  }
  GPFCON = 0x55aa;
  GPFUP  = 0xff;     // The pull up function is disabled GPF[7:0]

  printk("led registered!!\n");
  return 0;
}

static void led_exit(void)
{
  unregister_chrdev(LED_MAJOR, DEVICE_NAME);
  printk("led unregistered !!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");

