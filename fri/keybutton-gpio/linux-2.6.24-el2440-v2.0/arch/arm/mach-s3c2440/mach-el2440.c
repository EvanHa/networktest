/* linux/arch/arm/mach-s3c2440/mach-el2440.c
 *
 * Copyright (c) 2004,2005 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * http://www.fluff.org/ben/smdk2440/
 *
 * Thanks to Dimity Andric and TomTom for the loan of an SMDK2440.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <asm/plat-s3c/regs-serial.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/regs-lcd.h>

#include <asm/arch/idle.h>
#include <asm/arch/fb.h>
#include <asm/arch/ts.h> // added

#include <asm/plat-s3c24xx/s3c2410.h>
#include <asm/plat-s3c24xx/s3c2440.h>
#include <asm/plat-s3c24xx/clock.h>
#include <asm/plat-s3c24xx/devs.h>
#include <asm/plat-s3c24xx/cpu.h>

#include <asm/plat-s3c24xx/common-el2440.h> // for el2440 board

static struct map_desc el2440_iodesc[] __initdata = {
	/* ISA IO Space map (memory space selected by A24) */

	{      /*      CS8900A Mapping   */
		.virtual	= (u32)S3C24XX_VA_ISA_WORD,
		.pfn		= __phys_to_pfn(S3C2410_CS3 +(1<<24)),
		.length       = SZ_1M,
		.type		= MT_DEVICE ,
	},

/*
	{
		.virtual	= (u32)S3C24XX_VA_ISA_WORD,
		.pfn		= __phys_to_pfn(S3C2410_CS2),
		.length		= 0x10000,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_WORD + 0x10000,
		.pfn		= __phys_to_pfn(S3C2410_CS2 + (1<<24)),
		.length		= SZ_4M,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_BYTE,
		.pfn		= __phys_to_pfn(S3C2410_CS2),
		.length		= 0x10000,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_BYTE + 0x10000,
		.pfn		= __phys_to_pfn(S3C2410_CS2 + (1<<24)),
		.length		= SZ_4M,
		.type		= MT_DEVICE,
	} */
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg el2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	/* IR port */
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x43,
		.ufcon	     = 0x51,
	}
};

/* LCD driver info */

static struct s3c2410fb_display el2440_lcd_cfg __initdata = {

	.lcdcon5	= S3C2410_LCDCON5_FRM565 |
			  S3C2410_LCDCON5_INVVLINE |
			  S3C2410_LCDCON5_INVVFRAME |
			  S3C2410_LCDCON5_PWREN |
			  S3C2410_LCDCON5_HWSWP,

	.type		= S3C2410_LCDCON1_TFT,

//	.width		= 240,
//	.height	= 320,
	.width		= 320,
	.height	= 240,

	.pixclock	= 166667, /* HCLK 60 MHz, divisor 10 */
//	.xres		= 240,
//	.yres		= 320,
	.xres		= 320,
	.yres		= 240,
	.bpp		= 16,
	.left_margin	= 20,
	.right_margin	= 8,
	.hsync_len	= 4,
	.upper_margin	= 8,
	.lower_margin	= 7,
	.vsync_len	= 4,
};

static struct s3c2410fb_mach_info el2440_fb_info __initdata = {
	.displays	= &el2440_lcd_cfg,
	.num_displays	= 1,
	.default_display = 0,

    .gpccon     =   0xaaaaaaaa,
    .gpccon_mask=   0xffffffff,
    .gpcup      =   0x0000ffff,
    .gpcup_mask =   0xffffffff,
    .gpdcon     =   0xaaaaaaaa,
    .gpdcon_mask=   0xffffffff,
    .gpdup      =   0x0000ffff,
    .gpdup_mask =   0xffffffff,

    .lpcsel=    0xe0,
};

// Touch screen setup
static struct s3c2410_ts_mach_info elayer_s24xx_ts_cfg = {
	.delay = 10000,
	.presc = 49, 
	.oversampling_shift = 2,
};

static struct platform_device *el2440_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c,
	&s3c_device_iis,
	&s3c_device_ts, // added
	&el2440_device_keybtn,
};

static void __init el2440_map_io(void)
{
	s3c24xx_init_io(el2440_iodesc, ARRAY_SIZE(el2440_iodesc));
	s3c24xx_init_clocks(16934400);
	s3c24xx_init_uarts(el2440_uartcfgs, ARRAY_SIZE(el2440_uartcfgs));
	set_s3c2410ts_info(&elayer_s24xx_ts_cfg);

}

static void __init el2440_machine_init(void)
{
	s3c24xx_fb_set_platdata(&el2440_fb_info);

	platform_add_devices(el2440_devices, ARRAY_SIZE(el2440_devices));

	elayer_machine_init();	// arch/arm/plat-s3c24xx/common-el2440.c	
	//smdk_machine_init();
}

MACHINE_START(EL2440, "EL2440(eLayer System)")
	/* Maintainer: Ben Dooks <ben@fluff.org> */
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,

	.init_irq	= s3c24xx_init_irq,
	.map_io		= el2440_map_io,
	.init_machine	= el2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
