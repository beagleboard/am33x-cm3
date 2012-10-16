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

int am335_init(void)
{
	int i;

	/*
	 * Each interrupt has a priority register associated with it
	 * 8 bits... only 7:6:5:4 are available for SA
	 * out of the 16 levels here... using a priority grouping
	 * these 4 bits can be further split into preempt priority
	 * and subpriority fields
	 */
	scr_enable_sleepdeep();
	scr_enable_sleeponexit();

	/* Disable all the external interrupts */
	for (i=0; i<AM335X_NUM_EXT_INTERRUPTS; i++)
		nvic_disable_irq(i);

	msg_init();

	trace_init();

	pm_init();

	/* Enable only the MBX IRQ */
	nvic_enable_irq(AM335X_IRQ_MBINT0);

	/* TODO: Enable PRCM_INT2 with a dummy handler */

	/* Notify A8 of init completion */
	a8_notify(CMD_STAT_PASS);

	/* Ok we are done here */

	return 0;
}

int main(void)
{
	system_init();	/* Setup Systick and global freq val */

	/*
	 * need to configure bit 2 of System control register for sleepdeep
	 * bit 1 of SCR will enable sleeponexit and this needs to be explored
	 * bit 0 is reserved
	 */

	am335_init();	/* configure SCR */

	/* For some reason, the compiler does not like the WFI wrapper here :\ */
	while(1)
		__asm("wfi");

	return 0;
}
