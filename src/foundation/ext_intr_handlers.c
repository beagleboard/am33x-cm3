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
#include <low_power.h>
#include <system_am335.h>

/* PRCM_M3_IRQ1: Triggered for events like PLL needs recalibration, domain transition completed */
void extint16_handler()
{
	while(1)
	;
}

/* MBINT0: Triggered on a dummy write to Mailbox module */
void extint31_handler()
{
	nvic_disable_irq(AM335X_IRQ_MBINT0);

	/* If command is not valid, need to update the status to FAIL
	 * and enable the mailbox interrupt back */
	if(!msg_cmd_is_valid()) {
		msg_cmd_stat_update(CMD_STAT_FAIL);
		nvic_enable_irq(AM335X_IRQ_MBINT0);
		return;
	}

	/* cmd was valid */
	if(msg_cmd_needs_trigger()) {
		mpu_disable();
		a8_m3_low_power_sync(CMD_STAT_WAIT4OK);
		nvic_enable_irq(AM335X_IRQ_MBINT0);
		return;
	} else {
		/* For Rev and S/M reset */
		msg_cmd_dispatcher();
	}
}

/* USBWAKEUP */
void extint33_handler()
{
	generic_wake_handler(AM335X_IRQ_USBWAKEUP);
}

/* PRCM_M3_IRQ2: Triggered when A8 executes WFI */
void extint34_handler()
{
	int i = 0;

	/* Flush out ALL the NVIC interrupts */
	for(i=0; i<AM335X_NUM_EXT_INTERRUPTS; i++)
	{
		nvic_disable_irq(i);
		nvic_clear_irq(i);
	}

	nvic_disable_irq(AM335X_IRQ_PRCM_M3_IRQ2);
	msg_cmd_dispatcher();
}

/* I2C0_WAKE */
void extint40_handler()
{
	nvic_disable_irq(AM335X_IRQ_I2C0_WAKE);
	generic_wake_handler(AM335X_IRQ_I2C0_WAKE);
}

/* RTC_TIMER_WAKE */
void extint41_handler()
{
	nvic_disable_irq(AM335X_IRQ_RTC_TIMER_WAKE);
	generic_wake_handler(AM335X_IRQ_RTC_TIMER_WAKE);
}

/* RTC_ALARM_WAKE */
void extint42_handler()
{
	nvic_disable_irq(AM335X_IRQ_RTC_ALARM_WAKE);
	generic_wake_handler(AM335X_IRQ_RTC_ALARM_WAKE);
}

/* TIMER0_WAKE */
void extint43_handler()
{
	nvic_disable_irq(AM335X_IRQ_TIMER0_WAKE);
	generic_wake_handler(AM335X_IRQ_TIMER0_WAKE);
}

/* TIMER1_WAKE */
void extint44_handler()
{
	nvic_disable_irq(AM335X_IRQ_TIMER1_WAKE);
	generic_wake_handler(AM335X_IRQ_TIMER1_WAKE);
}

/* UART0_WAKE */
void extint45_handler()
{
	nvic_disable_irq(AM335X_IRQ_UART0_WAKE);
	generic_wake_handler(AM335X_IRQ_UART0_WAKE);
}

/* GPIO0_WAKE0 */
void extint46_handler()
{
	nvic_disable_irq(AM335X_IRQ_GPIO0_WAKE0);
	generic_wake_handler(AM335X_IRQ_GPIO0_WAKE0);
}

/* GPIO0_WAKE1 */
void extint47_handler()
{
	nvic_disable_irq(AM335X_IRQ_GPIO0_WAKE1);
	generic_wake_handler(AM335X_IRQ_GPIO0_WAKE1);
}

/* MPU_WAKE */
void extint48_handler()
{
	while(1);
}

/* WDT0_WAKE */
void extint49_handler()
{
	nvic_disable_irq(AM335X_IRQ_WDT0_WAKE);
	generic_wake_handler(AM335X_IRQ_WDT0_WAKE);
}

/* WDT1_WAKE */
void extint50_handler()
{
	nvic_disable_irq(AM335X_IRQ_WDT1_WAKE);
	generic_wake_handler(AM335X_IRQ_WDT1_WAKE);
}

/* ADC_TSC_WAKE */
void extint51_handler()
{
	nvic_disable_irq(AM335X_IRQ_ADC_TSC_WAKE);
	generic_wake_handler(AM335X_IRQ_ADC_TSC_WAKE);
}
