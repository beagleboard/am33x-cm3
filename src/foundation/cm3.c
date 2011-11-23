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

#include <stdint.h>

#include <cm3.h>
#include <device_am335x.h>
#include <system_am335.h>

void nvic_enable_irq(int irq_no)
{
	int reg_offset = irq_no/32;

	int irq_reg;

	irq_reg = __raw_readl(NVIC_IRQ_SET_EN1 + (reg_offset*0x4));
	irq_reg |= (1 << (irq_no%32));

	__raw_writel(irq_reg, (NVIC_IRQ_SET_EN1 + (reg_offset*0x4)));

}

void nvic_disable_irq(int irq_no)
{
	int reg_offset = irq_no/32;

	int irq_reg;

	irq_reg = __raw_readl(NVIC_IRQ_CLR_EN1 + (reg_offset*0x4));
	irq_reg |= (1 << (irq_no%32));

	__raw_writel(irq_reg, (NVIC_IRQ_CLR_EN1 + (reg_offset*0x4)));

}

void nvic_clear_irq(int irq_no)
{
	int reg_offset = irq_no/32;

	int irq_reg;

	irq_reg = __raw_readl(NVIC_IRQ_CLR_PEND1 + (reg_offset*0x4));
	irq_reg |= (1 << (irq_no%32));

	__raw_writel(irq_reg, (NVIC_IRQ_CLR_PEND1 + (reg_offset*0x4)));

}

void scr_enable_sleepdeep()
{
	int scr_reg;

	scr_reg = __raw_readl(SYS_SCR);
	scr_reg |= (1<<SYS_SCR_SD_OFFSET);

	__raw_writel(scr_reg, SYS_SCR);
}

void scr_enable_sleeponexit()
{
	int scr_reg;

	scr_reg = __raw_readl(SYS_SCR);
	scr_reg |= (1<<SYS_SCR_SOE_OFFSET);

	__raw_writel(scr_reg, SYS_SCR);
}
