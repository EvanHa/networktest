/* linux/include/asm-arm/arch-s3c2410/regs-spi.h
 *
 * Copyright (c) 2004 Fetron GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * S3C2410 SPI register definition
 *
 *  Changelog:
 *    20-04-2004     KF      Created file
 *    04-10-2004     BJD     Removed VA address (no longer mapped)
 *			     tidied file for submission
 *    03-04-2005     LCVR    Added S3C2400_SPPIN_nCS definition
 */

#ifndef __ASM_ARCH_REGS_SPI_H
#define __ASM_ARCH_REGS_SPI_H

#define S3C2410_SPIREG(x)	((x) + S3C24XX_VA_SPI)

#define S3C2410_SPCON		S3C2410_SPIREG(0x00)
#define S3C2410_SPCON0		S3C2410_SPCON
#define S3C2410_SPCON1		S3C2410_SPIREG(0x20)

#define S3C2410_SPCON_SMOD_DMA		(2<<5)	/* DMA mode */
#define S3C2410_SPCON_SMOD_INT		(1<<5)	/* interrupt mode */
#define S3C2410_SPCON_SMOD_POLL		(0<<5)	/* polling mode */
#define S3C2410_SPCON_ENSCK			(1<<4)	/* Enable SCK */
#define S3C2410_SPCON_ENSCK_DIS		(0<<4)	/* Disale SCK */
#define S3C2410_SPCON_MSTR			(1<<3)	/* Master 1 */
#define S3C2410_SPCON_SLAVE			(0<<3)	/* Slave 0 */
#define S3C2410_SPCON_CPOL_HIGH		(1<<2)	/* Clock polarity select */
#define S3C2410_SPCON_CPOL_LOW		(0<<2)	/* Clock polarity select */
#define S3C2410_SPCON_CPHA_FMTB		(1<<1)	/* Clock Phase Select */
#define S3C2410_SPCON_CPHA_FMTA		(0<<1)	/* Clock Phase Select */
#define S3C2410_SPCON_TAGD			(1<<0)	/* Tx auto garbage data mode */
#define S3C2410_SPCON_TAGD_NORM		(0)		/* TAGD normal mode */

#define S3C2410_SPSTA		S3C2410_SPIREG(0x04)
#define S3C2410_SPSTA0		S3C2410_SPSTA
#define S3C2410_SPSTA1		S3C2410_SPIREG(0x24)

#define S3C2410_SPSTA_DCOL			(1<<2)	/* Data Collision Error */
#define S3C2410_SPSTA_MULD			(1<<1)	/* Multi Master Error */
#define S3C2410_SPSTA_READY			(1<<0)	/* Data Tx/Rx ready */


#define S3C2410_SPPIN		S3C2410_SPIREG(0x08)
#define S3C2410_SPPIN0		S3C2410_SPPIN
#define S3C2410_SPPIN1		S3C2410_SPIREG(0x28)

#define S3C2410_SPPIN_ENMUL	  		(1<<2)	/* Multi Master Error detect */
#define S3C2410_SPPIN_ENMUL_DIS		(0<<2)	/* ENMUL disable */
#define S3C2410_SPPIN_RESERVED		(1<<1)
#define S3C2400_SPPIN_nCS     	  	(1<<1)	/* SPI Card Select */
#define S3C2410_SPPIN_KEEP	  		(1)		/* Master Out keep */
#define S3C2410_SPPIN_KEEP_RELS		(0)		/* release */


#define S3C2410_SPPRE		S3C2410_SPIREG(0x0C)
#define S3C2410_SPPRE0		S3C2410_SPPRE
#define S3C2410_SPPRE1		S3C2410_SPIREG(0x2c)


#define S3C2410_SPTDAT		S3C2410_SPIREG(0x10)
#define S3C2410_SPTDAT0		S3C2410_SPTDAT
#define S3C2410_SPTDAT1		S3C2410_SPIREG(0x30)


#define S3C2410_SPRDAT		S3C2410_SPIREG(0x14)
#define S3C2410_SPRDAT0		S3C2410_SPRDAT
#define S3C2410_SPRDAT1		S3C2410_SPIREG(0x34)


#endif /* __ASM_ARCH_REGS_SPI_H */
