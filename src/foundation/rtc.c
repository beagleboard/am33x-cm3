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

#include <device_am335x.h>
#include <prcm.h>
#include <prmam335x.h>
#include <system_am335.h>

int rtc_enable_check()
{
	if(__raw_readl(AM335X_CM_RTC_CLKSTCTRL == 0x2))
		return 0;
	else
		while(1)
		;
}

void rtc_reg_write(int reg, int val)
{
	__raw_writel(val, reg);
}

int rtc_reg_read(int reg)
{
	return __raw_readl(reg);
}
