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

#ifndef __LOW_POWER_H__
#define __LOW_POWER_H__

#include <stdint.h>

#define CM3_VERSION		0x14

#define MOSC_OFF		0x0
#define MOSC_ON			0x1

#define DS_COUNT_DEFAULT	0x6A75
#define DS_COUNT_SHIFT		0
#define DS_COUNT_MASK		(0xffff << DS_COUNT_SHIFT)
#define DS_ENABLE_SHIFT		17
#define DS_ENABLE_MASK		(1 << DS_ENABLE_SHIFT)


#define PD_ON			0x3
#define PD_RET			0x1
#define PD_OFF			0x0

#define MEM_BANK_RET_ST_RET	0x1
#define MEM_BANK_RET_ST_OFF	0x0

#define MEM_BANK_ON_ST_ON	0x3
#define MEM_BANK_ON_ST_RET	0x1
#define MEM_BANK_ON_ST_OFF	0x0

#define WAKE_ALL		0x1fff

#define RTC_TIMEOUT_DEFAULT	0x2
#define RTC_TIMEOUT_MAX		0xf

struct rtc_data {
	int rtc_timeout_val :4;	/* Delay for RTC alarm timeout. Default = 2secs */
};

struct deep_sleep_data {
	int mosc_state :1;			/* MOSC to be kept on (1) or off (0) */
	int deepsleep_count :16;		/* Count of how many OSC clocks needs to be seen \
						before exiting deep sleep mode */

	int vdd_mpu_val :15;			/* If vdd_mpu is to be lowered, vdd_mpu in mV */

	int pd_mpu_state :2;			/* Powerstate of PD_MPU */
	int pd_mpu_ram_ret_state :1;		/* Sabertooth RAM in retention state */
	int pd_mpu_l1_ret_state :1;		/* L1 memory in retention state */
	int pd_mpu_l2_ret_state :1;		/* L2 memory in retention state */
	int pd_mpu_ram_on_state :2;		/* Sabertooth RAM in ON state */

	int pd_per_state :2;	 		/* Powerstate of PD_PER */
	int pd_per_icss_mem_ret_state :1;	/* ICSS memory in retention state */
	int pd_per_mem_ret_state :1; 		/* Other memories in retention state */
	int pd_per_ocmc_ret_state :1; 		/* OCMC memory in retention state */
	int pd_per_icss_mem_on_state :2; 	/* ICSS memory in ON state */
	int pd_per_mem_on_state :2; 		/* Other memories in ON state */
	int pd_per_ocmc_on_state :2; 		/* OCMC memory in ON state */

	int wake_sources :13;			/* Wake sources */
						/* USB, I2C0, RTC_ALARM, TIMER1 \
						   UART0, GPIO0_WAKE0, GPIO0_WAKE1, \
						   WDT1, ADTSC, RTC_TIMER, USBWOUT0, \
						   MPU, USBWOUT1 */
	int reserved :1;			/* Internal use */
};

int reg_mod(int, int, int);
int debugss_keep_enabled(void);

int module_state_change(int, int);
int clkdm_state_change(int state, int reg);
int interconnect_modules_enable(void);
int interconnect_modules_disable(void);
int essential_modules_disable(void);
int essential_modules_enable(void);
void mpu_disable(void);
void mpu_enable(void);
void clkdm_sleep(void);
void clkdm_wake(void);
void mpu_clkdm_sleep(void);
void mpu_clkdm_wake(void);
void wkup_clkdm_sleep(void);
void wkup_clkdm_wake(void);
int pd_state_change(int, int);
void pd_state_restore(void);

int mpu_ram_ret_state_change(int, int);
int mpu_l1_ret_state_change(int, int);
int mpu_l2_ret_state_change(int, int);
int icss_mem_ret_state_change(int, int);
int per_mem_ret_state_change(int, int);
int ocmc_mem_ret_state_change(int, int);

int mpu_ram_on_state_change(int, int);
int icss_mem_on_state_change(int, int);
int per_mem_on_state_change(int, int);
int ocmc_mem_on_state_change(int, int);

int per_powerst_change(int, int);
int mpu_powerst_change(int, int);

int get_pd_per_stctrl_val(int);
int get_pd_mpu_stctrl_val(int);

int verify_pd_transitions(void);

int disable_master_oscillator(void);
int enable_master_oscillator(void);

void configure_deepsleep_count(int ds_count);
void configure_wake_sources(int wake_sources,int mod_check);
void clear_wake_sources(void);
void enable_wake_sources_for_ds2(void);

void dpll_power_down(unsigned int dpll);
void dpll_power_up(unsigned int dpll);

#endif
