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

#ifndef __SYSTEM_AM335_H__
#define __SYSTEM_AM335_H__

#include <stdint.h>

struct ipc_data {
	int reg1;
	int reg2;
	int reg3;
	int reg4;
	int reg5;
	int reg6;
	int reg7;
	int reg8;
};

struct ds_data {
	int reg1;
	int reg2;
};


struct cmd_data {
	short cmd_id;
	void *data;
};

struct cmd_data cmd_global_data;
struct ipc_data a8_m3_data_r;
struct ipc_data	a8_m3_data_w;
struct ds_data	a8_m3_ds_data;

int ipc_reg_r;
int ipc_reg_w;

short cmd_id;
short cmd_stat;

int cmd_wake_sources;
int pd_mpu_stctrl_next_val;
int pd_mpu_stctrl_prev_val;
int pd_per_stctrl_next_val;
int pd_per_stctrl_prev_val;
int pd_mpu_pwrstst_next_val;
int pd_mpu_pwrstst_prev_val;
int pd_per_pwrstst_next_val;
int pd_per_pwrstst_prev_val;

unsigned int am335x_soc_rev;

void pm_init(void);

void system_init(void);
void system_core_clock_update(void);

void msg_init(void);
void msg_read(char);
void msg_read_all(void);
void msg_write(char);
void msg_write_all(void);

int msg_cmd_is_valid(void);
int msg_cmd_needs_trigger(void);
void msg_cmd_dispatcher(void);
void msg_cmd_stat_update(int);

void a8_notify(int);
void a8_m3_low_power_sync(int);

void a8_lp_cmd1_handler(struct cmd_data *, char);
void a8_lp_cmd2_handler(struct cmd_data *, char);
void a8_lp_cmd3_handler(struct cmd_data *, char);
void a8_lp_cmd5_handler(struct cmd_data *, char);
void a8_lp_cmd7_handler(struct cmd_data *, char);
void a8_standalone_handler(struct cmd_data *);
void a8_standby_handler(struct cmd_data *, char);
void a8_cpuidle_handler(struct cmd_data *, char);

void generic_wake_handler(int);
void a8_wake_cmd1_handler(void);
void a8_wake_cmd2_handler(void);
void a8_wake_cmd3_handler(void);
void a8_wake_cmd5_handler(void);
void a8_wake_cmd7_handler(void);
void a8_wake_cmdb_handler(void);
void a8_wake_cmd10_handler(void);

void m3_firmware_version(void);
void init_m3_state_machine(void);

void trace_init(void);
void trace_update(void);
void trace_get_current_pos(void);
void trace_set_current_pos(void);

int rtc_enable_check(void);
int rtc_reg_read(int);
void rtc_reg_write(int, int);

void setup_am335x_soc_revision(void);
unsigned int get_am335x_soc_rev(void);

#define BITBAND_SRAM_REF 	UMEM_ALIAS
#define BITBAND_SRAM_BASE 	0x22000000
#define BITBAND_SRAM(a,b) 	((BITBAND_SRAM_BASE + ((int)(a))*32 + (b*4)))

#define BITBAND_PERI_REF 	DMEM_ALIAS
#define BITBAND_PERI_BASE 	0x42000000
#define BITBAND_PERI(a,b) 	((BITBAND_PERI_BASE + (*(a) - BITBAND_PERI_REF)*32 + (b*4)))

#define BB_USB_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 0)))
#define BB_I2C0_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 1)))
#define BB_RTC_ALARM_WAKE	*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 2)))
#define BB_TIMER1_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 3)))
#define BB_UART0_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 4)))
#define BB_GPIO0_WAKE0		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 5)))
#define BB_GPIO0_WAKE1		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 6)))
#define BB_WDT1_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 7)))
#define BB_ADTSC_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 8)))
/* Not used currently */
#define BB_RTC_TIMER_WAKE	*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 9)))
#define BB_USBWOUT0		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 10)))
#define BB_MPU_WAKE		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 11)))
#define BB_USBWOUT1		*((volatile int *)(BITBAND_SRAM(&cmd_wake_sources, 12)))

#define PD_MPU	0x1
#define PD_PER	0x2

#define MODULE_DISABLE	0x0
#define MODULE_ENABLE	0x2

#define CD_DISABLE	0x0
#define CD_ENABLE	0x2

#define PD_DISABLE	0x0
#define PD_ENABLE	0x2

#define TRACE_REG	0x0
#define STAT_ID_REG	0x1
#define PARAM1_REG	0x2
#define PARAM2_REG	0x3
#define RES1_REG	0x4
#define RES2_REG	0x5
#define RES3_REG	0x6
#define CUST_REG	0x7

#define CMD_STAT_PASS		0x0
#define CMD_STAT_FAIL		0x1
#define CMD_STAT_WAIT4OK	0x2

#define SET_BIT(x)		(1<<x)
#define CLR_BIT(x)		(0<<x)

#define AM335X_REV_ES1_0	0
#define AM335X_REV_ES2_0	1
#define AM335X_REV_ES2_1	2

#endif
