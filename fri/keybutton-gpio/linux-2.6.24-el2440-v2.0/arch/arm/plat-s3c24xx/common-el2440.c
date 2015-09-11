/* linux/arch/arm/plat-s3c24xx/common-elayer.c
 *
 * Copyright (c) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Common code for SMDK2410 and SMDK2440 boards
 *
 * http://www.fluff.org/ben/smdk2440/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/sysdev.h>
#include <linux/platform_device.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/mach-types.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <asm/arch/regs-gpio.h>
#include <asm/arch/leds-gpio.h>

#include <asm/plat-s3c/nand.h>

#include <asm/plat-s3c24xx/common-el2440.h>
#include <asm/plat-s3c24xx/devs.h>
#include <asm/plat-s3c24xx/pm.h>

/* LED devices */

static struct s3c24xx_led_platdata el2440_pdata_led4 = {
	.gpio		= S3C2410_GPF4,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led4",
	.def_trigger	= "timer",
};

static struct s3c24xx_led_platdata el2440_pdata_led5 = {
	.gpio		= S3C2410_GPF5,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led5",
	.def_trigger	= "nand-disk",
};

static struct s3c24xx_led_platdata el2440_pdata_led6 = {
	.gpio		= S3C2410_GPF6,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led6",
};

static struct s3c24xx_led_platdata el2440_pdata_led7 = {
	.gpio		= S3C2410_GPF7,
	.flags		= S3C24XX_LEDF_ACTLOW | S3C24XX_LEDF_TRISTATE,
	.name		= "led7",
};

static struct platform_device el2440_led4 = {
	.name		= "s3c24xx_led",
	.id		= 0,
	.dev		= {
		.platform_data = &el2440_pdata_led4,
	},
};

static struct platform_device el2440_led5 = {
	.name		= "s3c24xx_led",
	.id		= 1,
	.dev		= {
		.platform_data = &el2440_pdata_led5,
	},
};

static struct platform_device el2440_led6 = {
	.name		= "s3c24xx_led",
	.id		= 2,
	.dev		= {
		.platform_data = &el2440_pdata_led6,
	},
};

static struct platform_device el2440_led7 = {
	.name		= "s3c24xx_led",
	.id		= 3,
	.dev		= {
		.platform_data = &el2440_pdata_led7,
	},
};


/***********************************************************
 * NAND Partition
 ***********************************************************/
#define FLASH_BOOTLOADER_SIZE       0x080000    // 512KB
//#define FLASH_KERNEL_SIZE           0x180000    // 1.5MB
#define FLASH_KERNEL_SIZE           0x200000    // 2MB  modified
#define FLASH_FILESYSTEM_SIZE       0x400000    // 4MB

struct mtd_partition el2440_default_nand_part[] = {
    [0] = {
            .name       = "part.0 : Bootloader",
            .offset     = 0x0,
            .size       = FLASH_BOOTLOADER_SIZE,
            .mask_flags = 0},
    [1] = {
            .name       = "part.1 : Kernel",
            .offset     = MTDPART_OFS_APPEND,
            .size       = FLASH_KERNEL_SIZE,
            .mask_flags = 0},
    [2] = {
            .name       = "part.2 : File System",
            .offset     = MTDPART_OFS_APPEND,
            .size       = FLASH_FILESYSTEM_SIZE,
            .mask_flags = 0},
    [3] = {
            .name       = "part.3 : User Area", 
            .offset     = MTDPART_OFS_APPEND,
            .size       = MTDPART_SIZ_FULL,
            .mask_flags = 0},
};

static struct s3c2410_nand_set el2440_nand_sets[] = {
	[0] = {
		.name		= "NAND",
		.nr_chips	= 1,
		.nr_partitions	= ARRAY_SIZE(el2440_default_nand_part),
		.partitions	= el2440_default_nand_part,
	},
};

/* choose a set of timings which should suit most 512Mbit
 * chips and beyond.
*/

static struct s3c2410_platform_nand el2440_nand_info = {
	.tacls		= 20,
	.twrph0		= 60,
	.twrph1		= 20,
	.nr_sets	= ARRAY_SIZE(el2440_nand_sets),
	.sets		= el2440_nand_sets,
};

/* devices we initialise */

static struct platform_device __initdata *el2440_devs[] = {
	&s3c_device_nand,
	&el2440_led4,
	&el2440_led5,
	&el2440_led6,
	&el2440_led7,
};

void __init elayer_machine_init(void)
{
	/* Configure the LEDs (even if we have no LED support)*/

	s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
	s3c2410_gpio_cfgpin(S3C2410_GPF7, S3C2410_GPF7_OUTP);

	s3c2410_gpio_setpin(S3C2410_GPF4, 1);
	s3c2410_gpio_setpin(S3C2410_GPF5, 1);
	s3c2410_gpio_setpin(S3C2410_GPF6, 1);
	s3c2410_gpio_setpin(S3C2410_GPF7, 1);

	if (machine_is_smdk2443())
		el2440_nand_info.twrph0 = 50;

	s3c_device_nand.dev.platform_data = &el2440_nand_info;

	platform_add_devices(el2440_devs, ARRAY_SIZE(el2440_devs));

	s3c2410_pm_init();
}
