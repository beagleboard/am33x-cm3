/*
 * AM33XX-CM3 firmware
 *
 * Cortex-M3 (CM3) firmware for power management on Texas Instruments' AM33XX series of SoCs
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  This software is licensed under the  standard terms and conditions in the Texas Instruments  Incorporated
 *  Technology and Software Publicly Available Software License Agreement , a copy of which is included in the
 *  software download.
*/

#ifndef __CM3_H__
#define __CM3_H__

#define NVIC_BASE		0xE000E100

#define NVIC_IRQ_SET_EN1	(NVIC_BASE + 0x0)
#define NVIC_IRQ_SET_EN2	(NVIC_BASE + 0x4)
#define NVIC_IRQ_SET_EN3	(NVIC_BASE + 0x8)

#define NVIC_IRQ_CLR_EN1	(NVIC_BASE + 0x80)
#define NVIC_IRQ_CLR_EN2	(NVIC_BASE + 0x84)
#define NVIC_IRQ_CLR_EN3	(NVIC_BASE + 0x88)

#define NVIC_IRQ_SET_PEND1	(NVIC_BASE + 0x100)
#define NVIC_IRQ_SET_PEND2	(NVIC_BASE + 0x104)
#define NVIC_IRQ_SET_PEND3	(NVIC_BASE + 0x108)

#define NVIC_IRQ_CLR_PEND1	(NVIC_BASE + 0x180)
#define NVIC_IRQ_CLR_PEND2	(NVIC_BASE + 0x184)
#define NVIC_IRQ_CLR_PEND3	(NVIC_BASE + 0x188)

#define SYS_CONTROL_BASE	0xE000ED00

#define SYS_SCR			(SYS_CONTROL_BASE + 0x10)
#define SYS_SCR_SD_OFFSET	0x2
#define SYS_SCR_SOE_OFFSET	0x1

void nvic_enable_irq(int);
void nvic_disable_irq(int);
void nvic_clear_irq(int);
void scr_enable_sleepdeep();
void scr_enable_sleeponexit();

#endif
