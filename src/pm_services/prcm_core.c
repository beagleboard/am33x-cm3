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
#include <prcm.h>
#include <prmam335x.h>
#include <system_am335.h>

/* Explicit 0s imply don't care */
struct deep_sleep_data ds0_data =  {
	.mosc_state 			= MOSC_OFF,
	.deepsleep_count 		= DS_COUNT_DEFAULT,
	.vdd_mpu_val 			= 0,

	.pd_mpu_state 			= PD_OFF,
	.pd_mpu_ram_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_mpu_l1_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_mpu_l2_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_mpu_ram_on_state 		= 0,

	.pd_per_state 			= PD_RET,
	.pd_per_icss_mem_ret_state 	= MEM_BANK_RET_ST_OFF,
	.pd_per_mem_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_per_ocmc_ret_state 		= MEM_BANK_RET_ST_RET,
	.pd_per_icss_mem_on_state 	= 0,
	.pd_per_mem_on_state 		= 0,
	.pd_per_ocmc_on_state 		= 0,

	.wake_sources 			= WAKE_ALL,
	.reserved 			= 0
};

struct deep_sleep_data ds1_data =  {
	.mosc_state 			= MOSC_OFF,
	.deepsleep_count 		= DS_COUNT_DEFAULT,
	.vdd_mpu_val 			= 0,

	.pd_mpu_state 			= PD_OFF,
	.pd_mpu_ram_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_mpu_l1_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_mpu_l2_ret_state 		= MEM_BANK_RET_ST_OFF,
	.pd_mpu_ram_on_state 		= 0,

	.pd_per_state 			= PD_ON,
	.pd_per_icss_mem_ret_state 	= 0,
	.pd_per_mem_ret_state 		= 0,
	.pd_per_ocmc_ret_state 		= 0,
	.pd_per_icss_mem_on_state 	= MEM_BANK_ON_ST_ON,
	.pd_per_mem_on_state 		= MEM_BANK_ON_ST_ON,
	.pd_per_ocmc_on_state 		= MEM_BANK_ON_ST_ON,

	.wake_sources 			= WAKE_ALL,
	.reserved 			= 0
};

struct deep_sleep_data ds2_data =  {
	.mosc_state 			= MOSC_OFF,
	.deepsleep_count 		= DS_COUNT_DEFAULT,
	.vdd_mpu_val 			= 0,

	.pd_mpu_state 			= PD_ON,
	.pd_mpu_ram_ret_state 		= 0,
	.pd_mpu_l1_ret_state 		= 0,
	.pd_mpu_l2_ret_state 		= 0,
	.pd_mpu_ram_on_state 		= MEM_BANK_ON_ST_ON,

	.pd_per_state 			= PD_ON,
	.pd_per_icss_mem_ret_state 	= 0,
	.pd_per_mem_ret_state 		= 0,
	.pd_per_ocmc_ret_state 		= 0,
	.pd_per_icss_mem_on_state 	= MEM_BANK_ON_ST_ON,
	.pd_per_mem_on_state 		= MEM_BANK_ON_ST_ON,
	.pd_per_ocmc_on_state 		= MEM_BANK_ON_ST_ON,

	.wake_sources 			= WAKE_ALL,
	.reserved 			= 0
};

/* Clear out the global variables here */
void pm_init(void)
{
	cmd_id 			= 0;
	cmd_stat 		= 0;

	cmd_global_data.cmd_id 	= 0;
	cmd_global_data.data 	= 0;

	pd_mpu_stctrl_next_val 	= 0;
	pd_per_stctrl_next_val 	= 0;
	pd_mpu_pwrstst_next_val	= 0;
	pd_per_pwrstst_next_val	= 0;
	pd_mpu_stctrl_prev_val	= 0;
	pd_per_stctrl_prev_val	= 0;
	pd_mpu_pwrstst_prev_val	= 0;
	pd_per_pwrstst_prev_val	= 0;
}

void setup_am335x_soc_revision(void)
{
	am335x_soc_rev = (__raw_readl(DEVICE_ID) >> 0x1c) & 0xff;
}

unsigned int get_am335x_soc_rev(void)
{
	return am335x_soc_rev;
}

int var_mod(int var, int mask, int bit_val)
{
	int v;

	v = var;
	v &= ~mask;
	v |= bit_val;

	return v;
}

#define DEFAULT_IDLEST_SHIFT		16
#define DEFAULT_IDLEST_MASK		(3 << DEFAULT_IDLEST_SHIFT)
#define DEFAULT_IDLEST_IDLE_VAL		3
#define DEFAULT_IDLEST_ACTIVE_VAL 	0

/* TODO: Add a timeout and bail out */
static void _module_enable(int reg)
{
	__raw_writel(MODULE_ENABLE, reg);

	while ((__raw_readl(reg) & DEFAULT_IDLEST_MASK)>>DEFAULT_IDLEST_SHIFT !=
		DEFAULT_IDLEST_ACTIVE_VAL);
}

static void _module_disable(int reg)
{
	__raw_writel(MODULE_DISABLE, reg);

	while ((__raw_readl(reg) & DEFAULT_IDLEST_MASK)>>DEFAULT_IDLEST_SHIFT !=
		DEFAULT_IDLEST_IDLE_VAL);
}

int module_state_change(int state, int reg)
{
	if (state == MODULE_DISABLE)
		_module_disable(reg);
	else
		_module_enable(reg);

	return 0;
}

#define DEFAULT_CLKTRCTRL_SHIFT		0
#define DEFAULT_CLKTRCTRL_MASK		(3 << DEFAULT_CLKTRCTRL_SHIFT)
#define DEFAULT_CLKTRCTRL_WAKE		0x2
#define DEFAULT_CLKTRCTRL_SLEEP		0x1

#define CLKDM_SLEEP	0x1
#define CLKDM_WAKE	0x2

static void _clkdm_sleep(int reg)
{
	int var = 0;

	var = __raw_readl(reg);
	var = var_mod(var, DEFAULT_CLKTRCTRL_MASK, DEFAULT_CLKTRCTRL_SLEEP);
	__raw_writel(var, reg);
}

static void _clkdm_wakeup(int reg)
{
	int var = 0;

	var = __raw_readl(reg);
	var = var_mod(var, DEFAULT_CLKTRCTRL_MASK, DEFAULT_CLKTRCTRL_WAKE);
	__raw_writel(var, reg);
}

int clkdm_state_change(int state, int reg)
{
	if (state == CLKDM_SLEEP)
		_clkdm_sleep(reg);
	else
		_clkdm_wakeup(reg);

	return 0;
}

/*
 * Looks like we'll have to ensure that we disable some modules when going down
 * ideally this list should have 0 entries but we need to check
 * what are the things that are really really necessary here
 */
int essential_modules_disable(void)
{
	/* Disable only the bare essential modules */
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_CLKDIV32K_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_IEEE5000_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_EMIF_FW_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_GPMC_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_OCMCRAM_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_EMIF_CLKCTRL);

	module_state_change(MODULE_DISABLE, AM335X_CM_PER_MAILBOX0_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_UART0_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_TIMER2_CLKCTRL);
	return 0;
}

int essential_modules_enable(void)
{
	/* Enable only the bare essential modules */
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_CLKDIV32K_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_IEEE5000_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_EMIF_FW_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_GPMC_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_OCMCRAM_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_EMIF_CLKCTRL);

	module_state_change(MODULE_ENABLE, AM335X_CM_PER_MAILBOX0_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_UART0_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_TIMER2_CLKCTRL);
	return 0;
}

int interconnect_modules_disable(void)
{
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_EMIF_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_EMIF_FW_CLKCTRL);

	module_state_change(MODULE_DISABLE, AM335X_CM_PER_L4LS_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_L4HS_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_L4FW_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_L3_INSTR_CLKCTRL);
	module_state_change(MODULE_DISABLE, AM335X_CM_PER_L3_CLKCTRL);

	return 0;
}

int interconnect_modules_enable(void)
{
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_L3_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_L3_INSTR_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_L4FW_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_L4HS_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_L4LS_CLKCTRL);

	module_state_change(MODULE_ENABLE, AM335X_CM_PER_EMIF_FW_CLKCTRL);
	module_state_change(MODULE_ENABLE, AM335X_CM_PER_EMIF_CLKCTRL);

	return 0;
}

void mpu_disable(void)
{
	module_state_change(MODULE_DISABLE, AM335X_CM_MPU_MPU_CLKCTRL);
}

void mpu_enable(void)
{
	module_state_change(MODULE_ENABLE, AM335X_CM_MPU_MPU_CLKCTRL);
}

/* CLKDM related */
void clkdm_sleep(void)
{
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_OCPWP_L3_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_ICSS_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_CPSW_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_LCDC_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_CLK_24MHZ_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_L4LS_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_L4HS_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_L4FW_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_L3S_CLKSTCTRL);
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_PER_L3_CLKSTCTRL);
}

void clkdm_wake(void)
{
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_L3_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_L3S_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_L4FW_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_L4HS_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_L4LS_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_CLK_24MHZ_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_LCDC_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_CPSW_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_ICSS_CLKSTCTRL);
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_PER_OCPWP_L3_CLKSTCTRL);
}

void mpu_clkdm_sleep(void)
{
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_MPU_CLKSTCTRL);
}

void mpu_clkdm_wake(void)
{
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_MPU_CLKSTCTRL);
}

void wkup_clkdm_sleep(void)
{
	clkdm_state_change(CLKDM_SLEEP, AM335X_CM_WKUP_CLKSTCTRL);
}

void wkup_clkdm_wake(void)
{
	clkdm_state_change(CLKDM_WAKE, AM335X_CM_WKUP_CLKSTCTRL);
}

/* PD related */
int pd_state_change(int val, int pd)
{
	if (pd == PD_MPU) {
		pd_mpu_stctrl_next_val	= val;
		pd_mpu_pwrstst_prev_val = __raw_readl(AM335X_PM_MPU_PWRSTST);
		__raw_writel(val, AM335X_PM_MPU_PWRSTCTRL);
	} else if (pd == PD_PER) {
		pd_per_stctrl_next_val = val;
		pd_per_pwrstst_prev_val = __raw_readl(AM335X_PM_PER_PWRSTST);
		__raw_writel(val, AM335X_PM_PER_PWRSTCTRL);
	}

	return 0;
}

int mpu_ram_ret_state_change(int val, int var)
{
	var = var_mod(var, MPU_RAM_RETSTATE_MASK,
				 (val << MPU_RAM_RETSTATE_SHIFT));

	return var;
}

int mpu_l1_ret_state_change(int val, int var)
{
	var = var_mod(var, MPU_L1_RETSTATE_MASK,
				 (val << MPU_L1_RETSTATE_SHIFT));

	return var;
}

int mpu_l2_ret_state_change(int val, int var)
{
	var = var_mod(var, MPU_L2_RETSTATE_MASK,
				(val << MPU_L2_RETSTATE_SHIFT));

	return var;
}

int icss_mem_ret_state_change(int val, int var)
{
	var = var_mod(var, PER_ICSS_MEM_RETSTATE_MASK,
				 (val << PER_ICSS_MEM_RETSTATE_SHIFT));

	return var;
}

int per_mem_ret_state_change(int val, int var)
{
	var = var_mod(var, PER_MEM_RETSTATE_MASK,
				(val << PER_MEM_RETSTATE_SHIFT));

	return var;
}

int ocmc_mem_ret_state_change(int val, int var)
{
	var = var_mod(var, PER_RAM_MEM_RETSTATE_MASK,
				(val << PER_RAM_MEM_RETSTATE_SHIFT));

	return var;
}

int mpu_ram_on_state_change(int val, int var)
{
	/* Currently don't do anything */
	return var;
}

int icss_mem_on_state_change(int val, int var)
{
	var = var_mod(var, PER_ICSS_MEM_ONSTATE_MASK,
				(val << PER_ICSS_MEM_ONSTATE_SHIFT));

	return var;
}

int per_mem_on_state_change(int val, int var)
{
	var = var_mod(var, PER_MEM_ONSTATE_MASK,
				(val << PER_MEM_ONSTATE_SHIFT));

	return var;
}

int ocmc_mem_on_state_change(int val, int var)
{
	var = var_mod(var, PER_RAM_MEM_ONSTATE_MASK,
				(val << PER_RAM_MEM_ONSTATE_SHIFT));

	return var;
}

int per_powerst_change(int val, int var)
{
	var = var_mod(var, PER_POWERSTATE_MASK,
				(val << PER_POWERSTATE_SHIFT));

	return var;
}

int mpu_powerst_change(int val, int var)
{
	var = var_mod(var, MPU_POWERSTATE_MASK,
				(val << MPU_POWERSTATE_SHIFT));

	return var;
}

static int _next_pd_per_stctrl_val(state)
{
	int v = 0;

	if (state == 0) {
		v = per_powerst_change(ds0_data.pd_per_state, v);
		v = icss_mem_ret_state_change(ds0_data.pd_per_icss_mem_ret_state, v);
		v = per_mem_ret_state_change(ds0_data.pd_per_mem_ret_state, v);
		v = ocmc_mem_ret_state_change(ds0_data.pd_per_ocmc_ret_state, v);
	} else if (state == 1) {
		v = per_powerst_change(ds1_data.pd_per_state, v);
		v = icss_mem_on_state_change(ds1_data.pd_per_icss_mem_on_state, v);
		v = per_mem_on_state_change(ds1_data.pd_per_mem_on_state, v);
		v = ocmc_mem_on_state_change(ds1_data.pd_per_ocmc_on_state, v);
	} else if (state == 2) {
		v = per_powerst_change(ds2_data.pd_per_state, v);
		v = icss_mem_on_state_change(ds2_data.pd_per_icss_mem_on_state, v);
		v = per_mem_on_state_change(ds2_data.pd_per_mem_on_state, v);
		v = ocmc_mem_on_state_change(ds2_data.pd_per_ocmc_on_state, v);
	}

	return v;
}

int get_pd_per_stctrl_val(int state)
{
	/* Backup the current value for restoration */
	pd_per_stctrl_prev_val = __raw_readl(AM335X_PM_PER_PWRSTCTRL);

	return _next_pd_per_stctrl_val(state);
}

static int _next_pd_mpu_stctrl_val(state)
{
	int v = 0;

	if (state == 0) {
		v = mpu_powerst_change(ds0_data.pd_mpu_state, v);
		v = mpu_ram_ret_state_change(ds0_data.pd_mpu_ram_ret_state, v);
		v = mpu_l1_ret_state_change(ds0_data.pd_mpu_l1_ret_state, v);
		v = mpu_l2_ret_state_change(ds0_data.pd_mpu_l2_ret_state, v);
	} else if (state == 1) {
		v = mpu_powerst_change(ds1_data.pd_mpu_state, v);
		v = mpu_ram_ret_state_change(ds1_data.pd_mpu_ram_ret_state, v);
		v = mpu_l1_ret_state_change(ds1_data.pd_mpu_l1_ret_state, v);
		v = mpu_l2_ret_state_change(ds1_data.pd_mpu_l2_ret_state, v);
	} else if (state == 2) {
		v = mpu_powerst_change(ds2_data.pd_mpu_state, v);
		v = mpu_ram_on_state_change(ds2_data.pd_mpu_ram_on_state, v);
	}

	return v;
}

int get_pd_mpu_stctrl_val(int state)
{
	/* Backup the current value for restoration */
	pd_mpu_stctrl_prev_val = __raw_readl(AM335X_PM_MPU_PWRSTCTRL);

	return _next_pd_mpu_stctrl_val(state);
}

/* DeepSleep related */
int disable_master_oscillator(void)
{
	int v = __raw_readl(DEEPSLEEP_CTRL);

	v = var_mod(v, DS_ENABLE_MASK, (1 << DS_ENABLE_SHIFT));

	__raw_writel(v, DEEPSLEEP_CTRL);

	return 0;
}

int enable_master_oscillator()
{
	int v = __raw_readl(DEEPSLEEP_CTRL);

	v = var_mod(v, DS_ENABLE_MASK, (0 << DS_ENABLE_SHIFT));

	__raw_writel(v, DEEPSLEEP_CTRL);

	return 0;
}

void configure_deepsleep_count(int ds_count)
{
	int v = __raw_readl(DEEPSLEEP_CTRL);

	v = var_mod(v, DS_COUNT_MASK, (ds_count << DS_COUNT_SHIFT));

	__raw_writel(v, DEEPSLEEP_CTRL);
}

/* 1ms timer */
static int check_timer1(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x10);

	if ((v & 0x14) != 0x14) {
		__raw_writel(0x214, base + 0x10);
		return 1;
	}

	return 0;
}

static int check_i2c(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x10);

	if ((v & 0x1C) != 0x1C) {
		__raw_writel(0x1C, base + 0x10);
		return 1;
	}

	return 0;
}

static int check_adtsc(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x10);

	if ((v & 0xC) != 0xC) {
		__raw_writel(0xCc, base + 0x10);
		return 1;
	}

	return 0;
}

static int check_uart(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x54);

	if ((v & 0x1C) != 0x1C) {
		__raw_writel(0x1C, base + 0x54);
		return 1;
	}

	return 0;
}

static int check_gpio(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x10);

	if ((v & 0x1C) != 0x1C) {
		__raw_writel(0x1C, base + 0x10);
		return 1;
	}

	return 0;
}

static int check_rtc(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x78);

	if ((v & 0x3) != 0x3) {
		__raw_writel(0x3, base + 0x78);
		return 1;
	}

	return 0;
}

static int check_wdt(int base)
{
	int v = 0;

	v = __raw_readl(base + 0x10);

	if ((v & 0x18) != 0x18) {
		__raw_writel(0x18, base + 0x10);
		return 1;
	}

	return 0;
}

/*
 * A8 is expected to have left the module in a state where it will
 * cause a wakeup event. Ideally, this function should just enable
 * the NVIC interrupt
 */
void configure_wake_sources(int wake_sources, int mod_check)
{
	int v = 0;

	if (wake_sources != 0 || (mod_check == 0))
		cmd_wake_sources = wake_sources;
	else
		cmd_wake_sources = WAKE_ALL;

	/*
	 * This flag is for checking the SYSCONFIG of the module
	 * and then gating the clock. The clock needs to be gated
	 * for the module to generate swake
	 */
	mod_check = 0;

	/* Enable wakeup interrupts from required wake sources */
	if (BB_USB_WAKE) {
		/* A8 driver needs to configure SYSCONFIG. No M3 access */
		module_state_change(MODULE_DISABLE, AM335X_CM_PER_USB0_CLKCTRL);
		nvic_enable_irq(AM335X_IRQ_USBWAKEUP);
	}

	if(BB_I2C0_WAKE) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_I2C0_CLKCTRL);
			v = check_i2c(I2C0_BASE);
			module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_I2C0_CLKCTRL);
		}
		nvic_enable_irq(AM335X_IRQ_I2C0_WAKE);
	}

	if(BB_ADTSC_WAKE) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_ADC_TSC_CLKCTRL);
			v = check_adtsc(ADC_TSC_BASE);
			module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_ADC_TSC_CLKCTRL);
		}
		nvic_enable_irq(AM335X_IRQ_ADC_TSC_WAKE);
	}

	if(BB_UART0_WAKE) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_UART0_CLKCTRL);
			v = check_uart(UART0_BASE);
			module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_UART0_CLKCTRL);
		}
		nvic_enable_irq(AM335X_IRQ_UART0_WAKE);
	}

	if(BB_GPIO0_WAKE0) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
			v = check_gpio(GPIO_BASE);
			module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
		}
		nvic_enable_irq(AM335X_IRQ_GPIO0_WAKE0);
	}

	if(BB_GPIO0_WAKE1) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
			v = check_gpio(GPIO_BASE);
			module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
		}
		nvic_enable_irq(AM335X_IRQ_GPIO0_WAKE1);
	}

	if(BB_RTC_ALARM_WAKE) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_RTC_RTC_CLKCTRL);
			v = check_rtc(RTCSS_BASE);
		}
		nvic_enable_irq(AM335X_IRQ_RTC_ALARM_WAKE);
	}

	if(BB_TIMER1_WAKE) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_TIMER1_CLKCTRL);
			v = check_timer1(DMTIMER1_BASE);
		}
		nvic_enable_irq(AM335X_IRQ_TIMER1_WAKE);
	}

	if(BB_WDT1_WAKE) {
		if(mod_check) {
			module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_WDT1_CLKCTRL);
			v = check_wdt(WDT1_BASE);
		}
		nvic_enable_irq(AM335X_IRQ_WDT1_WAKE);
	}

#if 0
	/* Not recommended */
	if(BB_RTC_TIMER_WAKE) {
		module_state_change(MODULE_ENABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
		module_wakeup_enable(AM335X_CM_PER_USB0_SYSCONFIG);
		nvic_enable_irq(AM335X_IRQ_RTC_TIMER_WAKE);
	}

	if(BB_MPU_WAKE) {
		module_wakeup_enable(AM335X_CM_PER_USB0_SYSCONFIG);
		nvic_enable_irq(AM335X_IRQ_MPU_WAKE);
	}

	if(BB_TIMER0_WAKE) {
		module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
		module_wakeup_enable(AM335X_CM_PER_USB0_SYSCONFIG);
		nvic_enable_irq(AM335X_IRQ_TIMER0_WAKE);
	}

	if(BB_WDT0_WAKE) {
		module_state_change(MODULE_DISABLE, AM335X_CM_WKUP_GPIO0_CLKCTRL);
		module_wakeup_enable(AM335X_CM_PER_USB0_SYSCONFIG);
		nvic_enable_irq(AM335X_IRQ_WDT0_WAKE);
	}
#endif

	if(BB_USBWOUT0)
		nvic_enable_irq(AM335X_IRQ_USB0WOUT);

	if(BB_USBWOUT1)
		nvic_enable_irq(AM335X_IRQ_USB1WOUT);
}

void clear_wake_sources(void)
{
	/*
	 * Clear the global variable
	 * and then disable all wake interrupts
	 */

	cmd_wake_sources = 0x0;	/* All disabled */

	/* Disable the wake interrupts */
	nvic_disable_irq(AM335X_IRQ_USBWAKEUP);
	nvic_disable_irq(AM335X_IRQ_I2C0_WAKE);
	nvic_disable_irq(AM335X_IRQ_RTC_TIMER_WAKE);
	nvic_disable_irq(AM335X_IRQ_RTC_ALARM_WAKE);
	nvic_disable_irq(AM335X_IRQ_TIMER0_WAKE);
	nvic_disable_irq(AM335X_IRQ_TIMER1_WAKE);
	nvic_disable_irq(AM335X_IRQ_UART0_WAKE);
	nvic_disable_irq(AM335X_IRQ_GPIO0_WAKE0);
	nvic_disable_irq(AM335X_IRQ_GPIO0_WAKE1);
	nvic_disable_irq(AM335X_IRQ_MPU_WAKE);
	nvic_disable_irq(AM335X_IRQ_WDT0_WAKE);
	nvic_disable_irq(AM335X_IRQ_WDT1_WAKE);
	nvic_disable_irq(AM335X_IRQ_ADC_TSC_WAKE);
	nvic_disable_irq(AM335X_IRQ_USB0WOUT);
	nvic_disable_irq(AM335X_IRQ_USB1WOUT);

	/* TODO: Clear all the pending interrupts */
}

void pd_state_restore(void)
{
	__raw_writel(pd_per_stctrl_prev_val, AM335X_PM_PER_PWRSTCTRL);
	__raw_writel(pd_mpu_stctrl_prev_val, AM335X_PM_MPU_PWRSTCTRL);
}

/* Checking only the stst bits for now */
int verify_pd_transitions(void)
{
	int mpu_ctrl, mpu_stst, per_ctrl, per_stst;

	mpu_ctrl = __raw_readl(AM335X_PM_MPU_PWRSTCTRL);
	per_ctrl = __raw_readl(AM335X_PM_PER_PWRSTCTRL);

	mpu_stst = __raw_readl(AM335X_PM_MPU_PWRSTST);
	per_stst = __raw_readl(AM335X_PM_PER_PWRSTST);

	if (((mpu_ctrl & 0x3) == (mpu_stst & 0x3)) &&
		((per_ctrl & 0x3) == (per_stst & 0x3)))
		return CMD_STAT_PASS;
	else
		return CMD_STAT_FAIL;
}

/* New Power-down Sequence PG 2.0 */
void dpll_power_down(unsigned int dpll)
{
	int dpll_reg, dpll_reg_val, dpll_pwr_sw_status_reg;
	unsigned int sw_ctrl_dpll_bit, isoscan_bit, ret_bit, reset_bit, iso_bit;
	unsigned int pgoodin_bit, ponin_bit;
	unsigned int pgoodout_status_bit, ponout_status_bit;

	switch (dpll) {
	case DPLL_PER:
		dpll_reg		= DPLL_PWR_SW_CTRL;
		sw_ctrl_dpll_bit	= SW_CTRL_PER_DPLL;
		isoscan_bit		= ISOSCAN_PER;
		ret_bit			= RET_PER;
		reset_bit		= RESET_PER;
		iso_bit			= ISO_PER;
		pgoodin_bit		= PGOODIN_PER;
		ponin_bit		= PONIN_PER;
		dpll_pwr_sw_status_reg	= DPLL_PWR_SW_STATUS;
		pgoodout_status_bit	= PGOODOUT_PER_STATUS;
		ponout_status_bit	= PONOUT_PER_STATUS;
		break;
	case DPLL_DISP:
		dpll_reg		= DPLL_PWR_SW_CTRL;
		sw_ctrl_dpll_bit	= SW_CTRL_DISP_DPLL;
		isoscan_bit		= ISOSCAN_DISP;
		ret_bit			= RET_DISP;
		reset_bit		= RESET_DISP;
		iso_bit			= ISO_DISP;
		pgoodin_bit		= PGOODIN_DISP;
		ponin_bit		= PONIN_DISP;
		dpll_pwr_sw_status_reg	= DPLL_PWR_SW_STATUS;
		pgoodout_status_bit	= PGOODOUT_DISP_STATUS;
		ponout_status_bit	= PONOUT_DISP_STATUS;
		break;
	case DPLL_DDR:
		dpll_reg		= DPLL_PWR_SW_CTRL;
		sw_ctrl_dpll_bit	= SW_CTRL_DDR_DPLL;
		isoscan_bit		= ISOSCAN_DDR;
		ret_bit			= RET_DDR;
		reset_bit		= RESET_DDR;
		iso_bit			= ISO_DDR;
		pgoodin_bit		= PGOODIN_DDR;
		ponin_bit		= PONIN_DDR;
		dpll_pwr_sw_status_reg	= DPLL_PWR_SW_STATUS;
		pgoodout_status_bit	= PGOODOUT_DDR_STATUS;
		ponout_status_bit	= PONOUT_DDR_STATUS;
		break;
	default:
		return;
	}

	/* Configure bit to select Control module selection for DPLL */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val |= sw_ctrl_dpll_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* Assert ISO bit high */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val |= iso_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* ISO_SCAN, RET should be asserted high */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val |= (isoscan_bit | ret_bit);
	__raw_writel(dpll_reg_val, dpll_reg);

	/* Assert DPLL reset to 1 */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val |= reset_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* PGOODIN signal is de-asserted low */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val &= ~pgoodin_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* PONIN signal is de-asserted low */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val &= ~ponin_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* Poll for PONOUT and PGOODOUT signal status as 0 */
	while (__raw_readl(dpll_pwr_sw_status_reg) &
			(pgoodout_status_bit | ponout_status_bit)) {
	}
}

/* New Power-up Sequence PG 2.0 */
void dpll_power_up(unsigned int dpll)
{
	int dpll_reg, dpll_reg_val, dpll_pwr_sw_status_reg;
	unsigned int sw_ctrl_dpll_bit, isoscan_bit, ret_bit, reset_bit, iso_bit;
	unsigned int pgoodin_bit, ponin_bit;
	unsigned int pgoodout_status_bit, ponout_status_bit;

	switch (dpll) {
	case DPLL_PER:
		dpll_reg		= DPLL_PWR_SW_CTRL;
		sw_ctrl_dpll_bit	= SW_CTRL_PER_DPLL;
		isoscan_bit		= ISOSCAN_PER;
		ret_bit			= RET_PER;
		reset_bit		= RESET_PER;
		iso_bit			= ISO_PER;
		pgoodin_bit		= PGOODIN_PER;
		ponin_bit		= PONIN_PER;
		dpll_pwr_sw_status_reg	= DPLL_PWR_SW_STATUS;
		pgoodout_status_bit	= PGOODOUT_PER_STATUS;
		ponout_status_bit	= PONOUT_PER_STATUS;
		break;
	case DPLL_DISP:
		dpll_reg		= DPLL_PWR_SW_CTRL;
		sw_ctrl_dpll_bit	= SW_CTRL_DISP_DPLL;
		isoscan_bit		= ISOSCAN_DISP;
		ret_bit			= RET_DISP;
		reset_bit		= RESET_DISP;
		iso_bit			= ISO_DISP;
		pgoodin_bit		= PGOODIN_DISP;
		ponin_bit		= PONIN_DISP;
		dpll_pwr_sw_status_reg	= DPLL_PWR_SW_STATUS;
		pgoodout_status_bit	= PGOODOUT_DISP_STATUS;
		ponout_status_bit	= PONOUT_DISP_STATUS;
		break;
	case DPLL_DDR:
		dpll_reg		= DPLL_PWR_SW_CTRL;
		sw_ctrl_dpll_bit	= SW_CTRL_DDR_DPLL;
		isoscan_bit		= ISOSCAN_DDR;
		ret_bit			= RET_DDR;
		reset_bit		= RESET_DDR;
		iso_bit			= ISO_DDR;
		pgoodin_bit		= PGOODIN_DDR;
		ponin_bit		= PONIN_DDR;
		dpll_pwr_sw_status_reg	= DPLL_PWR_SW_STATUS;
		pgoodout_status_bit	= PGOODOUT_DDR_STATUS;
		ponout_status_bit	= PONOUT_DDR_STATUS;
		break;
	default:
		return;
	}

	/* PONIN is asserted high */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val |= ponin_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* Poll for PONOUT to become high  */
	while (!(__raw_readl(dpll_pwr_sw_status_reg) & ponout_status_bit)) {
	}

	/* PGOODIN is asserted high */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val |= pgoodin_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* Poll for PGOODOUT to become high */
	while (!(__raw_readl(dpll_pwr_sw_status_reg) & pgoodout_status_bit)) {
	}

	/* De-assert DPLL RESET to 0 */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val &= ~reset_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* ISO_SCAN, RET should be de-asserted low */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val &= ~(isoscan_bit | ret_bit);
	__raw_writel(dpll_reg_val, dpll_reg);

	/* De-assert ISO signal */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val &= ~iso_bit;
	__raw_writel(dpll_reg_val, dpll_reg);

	/* Re-Configure bit to select PRCM selection for DPLL */
	dpll_reg_val = __raw_readl(dpll_reg);
	dpll_reg_val &= ~sw_ctrl_dpll_bit;
	__raw_writel(dpll_reg_val, dpll_reg);
}

void core_ldo_power_down(void)
{
	unsigned int core_ldo;

	/* Configure RETMODE_ENABLE for CORE LDO */
	core_ldo = __raw_readl(AM335X_PRM_LDO_SRAM_CORE_CTRL);
	core_ldo |= RETMODE_ENABLE;
	__raw_writel(core_ldo, AM335X_PRM_LDO_SRAM_CORE_CTRL);

	/* Poll for LDO Status to be in retention (SRAMLDO_STATUS) */
	while (!(__raw_readl(AM335X_PRM_LDO_SRAM_CORE_CTRL) & SRAMLDO_STATUS)) {
	}
}

void core_ldo_power_up(void)
{
	unsigned int core_ldo;

	/* Disable RETMODE for CORE LDO */
	core_ldo = __raw_readl(AM335X_PRM_LDO_SRAM_CORE_CTRL);
	core_ldo &= ~RETMODE_ENABLE;
	__raw_writel(core_ldo, AM335X_PRM_LDO_SRAM_CORE_CTRL);

	/* Poll for LDO status to be out of retention (SRAMLDO_STATUS) */
	while (__raw_readl(AM335X_PRM_LDO_SRAM_CORE_CTRL) & SRAMLDO_STATUS) {
	}
}
