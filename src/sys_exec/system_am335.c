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
#include <system_am335.h>

/* uint32_t system_core_clock; */

void system_init()
{
	/* Disable SysTick */
	/* Note: AM335 has the calibration value
	 * and hence that is a better option (if used)
	 * SysTick->CTRL = 0;
	 */
}


/* Update the clock source/frequency if we know what to do based on OPP */
void system_core_clock_update()
{

}

/* systick interrupt handler */
void systick_handler(void)
{
	/* We don't necessarily need timer based delays in the firmware
	 * If needed in the future, the way we could do this is
	 * 1. Initialise a global timer_count variable
	 * 2. Switch to the 10ms mode in SysTick module and use calibration value
	 *    as the reload value on timer expiry
	 * 3. Enable timer interrupts
	 * 4. In the IRQ handler we just increment timer_count
	 * Note: Will need to take care of wraparound condition
	 */
}
