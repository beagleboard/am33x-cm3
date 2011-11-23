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

#ifndef __DEVICE_AM335X_H__
#define __DEVICE_AM335X_H__

#define AM335X_IRQ_PRCM_M3_IRQ1		16
#define AM335X_IRQ_UART0INT		17
#define AM335X_IRQ_TINT0		18
#define AM335X_IRQ_TINT1_1MS		19
#define AM335X_IRQ_WDT0INT		20
#define AM335X_IRQ_WDT1INT		21
#define AM335X_IRQ_I2C0INT		22
#define AM335X_IRQ_GPIOINTA		23
#define AM335X_IRQ_GPIOINTB		24
#define AM335X_IRQ_ADC_TSC_INT1		26
#define AM335X_IRQ_RTCINT1		27
#define AM335X_IRQ_RTCALARMINT		28
#define AM335X_IRQ_SMRFLX_MPU		29
#define AM335X_IRQ_SMRFLX_CORE		30
#define AM335X_IRQ_MBINT0		31
#define AM335X_IRQ_USBWAKEUP		33
#define AM335X_IRQ_PRCM_M3_IRQ2		34
#define AM335X_IRQ_USB0WOUT		35
#define AM335X_IRQ_USB1WOUT		36
#define AM335X_IRQ_DMA_INTR_PIN1	37
#define AM335X_IRQ_DMA_INTR_PIN2	38
#define AM335X_IRQ_I2C0_WAKE		40
#define AM335X_IRQ_RTC_TIMER_WAKE	41
#define AM335X_IRQ_RTC_ALARM_WAKE	42
#define AM335X_IRQ_TIMER0_WAKE		43
#define AM335X_IRQ_TIMER1_WAKE		44
#define AM335X_IRQ_UART0_WAKE		45
#define AM335X_IRQ_GPIO0_WAKE0		46
#define AM335X_IRQ_GPIO0_WAKE1		47
#define AM335X_IRQ_MPU_WAKE		48
#define AM335X_IRQ_WDT0_WAKE		49
#define AM335X_IRQ_WDT1_WAKE		50
#define AM335X_IRQ_ADC_TSC_WAKE		51

#define AM335X_NUM_EXT_INTERRUPTS	52

#define UMEM_BASE	0x00000000
#define DMEM_BASE	0x00080000
#define UMEM_BASE_ALIAS	0x20000000
#define DMEM_BASE_ALIAS	0x20080000
#define L4_WKUP_BASE	0x44C00000

#define UMEM_START	0x0
#define UMEM_ALIAS	0x20000000
#define UMEM_SIZE	0x3FFF		/* 16 KB */

#define DMEM_START	0x80000
#define DMEM_ALIAS	0x20080000
#define DMEM_SIZE	0x1FFF		/* 8 KB */

#define CM3_STACK_SIZE	(0xC00)		/* 3KB */
#define CM3_SP		(DMEM_START + CM3_STACK_SIZE)

#define PRCM_BASE	0x44E00000
#define DMTIMER_BASE	0x44E05000
#define GPIO_BASE	0x44E07000
#define UART0_BASE	0x44E09000
#define I2C0_BASE	0x44E0B000
#define ADC_TSC_BASE	0x44E0D000
#define CONTROL_BASE	0x44E10000
#define DMTIMER1_BASE 	0x44E31000
#define WDT0_BASE	0x44E33000
#define WDT1_BASE	0x44E35000
#define SR0_BASE	0x44E37000
#define SR1_BASE	0x44E39000
#define RTCSS_BASE	0x44E3E000

/* RTC module */

#define RTC_SECONDS_REG		(RTCSS_BASE + 0x00)
#define RTC_MINUTES_REG		(RTCSS_BASE + 0x04)
#define RTC_HOURS_REG		(RTCSS_BASE + 0x08)
#define RTC_DAYS_REG		(RTCSS_BASE + 0x0C)
#define RTC_MONTHS_REG		(RTCSS_BASE + 0x10)
#define RTC_YEARS_REG		(RTCSS_BASE + 0x14)
#define RTC_WEEKS_REG		(RTCSS_BASE + 0x18)

#define RTC_ALARM_SECONDS_REG	(RTCSS_BASE + 0x20)
#define RTC_ALARM_MINUTES_REG	(RTCSS_BASE + 0x24)
#define RTC_ALARM_HOURS_REG	(RTCSS_BASE + 0x28)
#define RTC_ALARM_DAYS_REG	(RTCSS_BASE + 0x2c)
#define RTC_ALARM_MONTHS_REG	(RTCSS_BASE + 0x30)
#define RTC_ALARM_YEARS_REG	(RTCSS_BASE + 0x34)

#define RTC_CTRL_REG		(RTCSS_BASE + 0x40)
#define RTC_STATUS_REG		(RTCSS_BASE + 0x44)
#define RTC_INTERRUPTS_REG	(RTCSS_BASE + 0x48)
#define RTC_COMP_LSB_REG	(RTCSS_BASE + 0x4c)
#define RTC_COMP_MSB_REG	(RTCSS_BASE + 0x50)
#define RTC_OSC_REG		(RTCSS_BASE + 0x54)

#define RTC_KICK0		(RTCSS_BASE + 0x6c)
#define RTC_KICK1		(RTCSS_BASE + 0x70)
#define RTC_SYSCONFIG		(RTCSS_BASE + 0x78)
#define RTC_IRQWAKEEN_0		(RTCSS_BASE + 0x7c)

#define RTC_ALARM2_SECONDS_REG	(RTCSS_BASE + 0x80)
#define RTC_ALARM2_MINUTES_REG	(RTCSS_BASE + 0x84)
#define RTC_ALARM2_HOURS_REG	(RTCSS_BASE + 0x88)
#define RTC_ALARM2_DAYS_REG	(RTCSS_BASE + 0x8c)
#define RTC_ALARM2_MONTHS_REG	(RTCSS_BASE + 0x90)
#define RTC_ALARM2_YEARS_REG	(RTCSS_BASE + 0x94)

#define RTC_PMIC_REG		(RTCSS_BASE + 0x98)
#define RTC_DEBOUNCE_REG	(RTCSS_BASE + 0x9c)

/* RTC_CTRL_REG bit fields: */
#define RTC_CTRL_DISABLE	(1<<6)
#define RTC_CTRL_STOP		(1<<0)

/* RTC_STATUS_REG bit fields: */
#define RTC_STATUS_POWER_UP        (1<<7)
#define RTC_STATUS_ALARM           (1<<6)
#define RTC_STATUS_1D_EVENT        (1<<5)
#define RTC_STATUS_1H_EVENT        (1<<4)
#define RTC_STATUS_1M_EVENT        (1<<3)
#define RTC_STATUS_1S_EVENT        (1<<2)
#define RTC_STATUS_RUN             (1<<1)
#define RTC_STATUS_BUSY            (1<<0)

/* RTC_INTERRUPTS_REG bit fields: */
#define RTC_INTERRUPTS_IT_ALARM    (1<<3)
#define RTC_INTERRUPTS_IT_TIMER    (1<<2)

#define IPC_MSG_REG1	(CONTROL_BASE + 0x1328)
#define IPC_MSG_REG2	(CONTROL_BASE + 0x132c)
#define IPC_MSG_REG3	(CONTROL_BASE + 0x1330)
#define IPC_MSG_REG4	(CONTROL_BASE + 0x1334)
#define IPC_MSG_REG5	(CONTROL_BASE + 0x1338)
#define IPC_MSG_REG6	(CONTROL_BASE + 0x133c)
#define IPC_MSG_REG7	(CONTROL_BASE + 0x1340)
#define IPC_MSG_REG8	(CONTROL_BASE + 0x1344)

#define DEEPSLEEP_CTRL	(CONTROL_BASE + 0x0470)

#define __raw_readl(a)		(*(volatile int *)(a))
#define __raw_writel(v, a)	(*(volatile int *)(a) = v)

#endif
