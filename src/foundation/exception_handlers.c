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

/* Will be extended in the future as and when required */
void nmi_handler(void)
{
	while(1)
	;
}

void hardfault_handler(void)
{
	while(1)
	;
}

void memmanage_handler(void)
{
	while(1)
	;
}

void busfault_handler(void)
{
	while(1)
	;
}

void usagefault_handler(void)
{
	while(1)
	;
}

void svc_handler(void)
{
	while(1)
	;
}

void debugmon_handler(void)
{
	while(1)
	;
}

void pendsv_handler(void)
{
	while(1)
	;
}

#if 0
void systick_handler(void)
{
	while(1)
	;
}
#endif
