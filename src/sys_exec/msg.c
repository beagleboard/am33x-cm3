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
#include <low_power.h>
#include <system_am335.h>

short valid_cmd_id[] = {
	0x1,	/* RTC */
	0x2, 	/* RTC_FAST */
	0x3,	/* DS0 */
	0x5,	/* DS1 */
	0x7,	/* DS2 */
	0x9,	/* Standalone app */
	0xb,	/* Standby */
	0xe,	/* Reset State Machine */
	0xf,	/* Version */
	0x10,	/* CPUIdle MPU Clock gating*/
};

/* Clear out the IPC regs */
void msg_init(void)
{
	/* TODO: Global data related to msg also? */
	a8_m3_data_r.reg1 = 0;
	a8_m3_data_r.reg2 = 0;
	a8_m3_data_r.reg3 = 0;
	a8_m3_data_r.reg4 = 0;
	a8_m3_data_r.reg5 = 0;
	a8_m3_data_r.reg6 = 0;
	a8_m3_data_r.reg7 = 0;
	a8_m3_data_r.reg8 = 0;

	a8_m3_data_w.reg1 = 0;
	a8_m3_data_w.reg2 = 0;
	a8_m3_data_w.reg3 = 0;
	a8_m3_data_w.reg4 = 0;
	a8_m3_data_w.reg5 = 0;
	a8_m3_data_w.reg6 = 0;
	a8_m3_data_w.reg7 = 0;
	a8_m3_data_w.reg8 = 0;

	ipc_reg_r = 0;
	ipc_reg_w = 0;
}

/* Read all the IPC registers in one-shot */
void msg_read_all(void)
{
	a8_m3_data_r.reg1 = __raw_readl(IPC_MSG_REG1);
	a8_m3_data_r.reg2 = __raw_readl(IPC_MSG_REG2);
	a8_m3_data_r.reg3 = __raw_readl(IPC_MSG_REG3);
	a8_m3_data_r.reg4 = __raw_readl(IPC_MSG_REG4);
	a8_m3_data_r.reg5 = __raw_readl(IPC_MSG_REG5);
	a8_m3_data_r.reg6 = __raw_readl(IPC_MSG_REG6);
	a8_m3_data_r.reg7 = __raw_readl(IPC_MSG_REG7);
	a8_m3_data_r.reg8 = __raw_readl(IPC_MSG_REG8);
}

/* Read one specific IPC register */
void msg_read(char reg)
{
	ipc_reg_r = __raw_readl(IPC_MSG_REG1 + (0x4*reg));
	__raw_writel(ipc_reg_r, (int)(&a8_m3_data_r) + (0x4*reg));
}

/* Write all the IPC registers in one-shot */
void msg_write_all(void)
{
	__raw_writel(a8_m3_data_w.reg1, IPC_MSG_REG1);
	__raw_writel(a8_m3_data_w.reg2, IPC_MSG_REG2);
	__raw_writel(a8_m3_data_w.reg3, IPC_MSG_REG3);
	__raw_writel(a8_m3_data_w.reg4, IPC_MSG_REG4);
	__raw_writel(a8_m3_data_w.reg5, IPC_MSG_REG5);
	__raw_writel(a8_m3_data_w.reg6, IPC_MSG_REG6);
	__raw_writel(a8_m3_data_w.reg7, IPC_MSG_REG7);
	/* TODO: Has to the removed since this is reserved */
	__raw_writel(a8_m3_data_w.reg8, IPC_MSG_REG8);
}

/*
 * Write to one specific IPC register
 * Before calling this fn, make sure ipc_reg_w has the correct val
 * TODO: Should check for the reg no. as some are reserved?
 */
void msg_write(char reg)
{
	__raw_writel(ipc_reg_w, IPC_MSG_REG1 + (0x4*reg));
	__raw_writel(ipc_reg_w, (int)(&a8_m3_data_w) + (0x4*reg));
}

/*
 * Check if the cmd_id is valid or not
 * return 1 on success, 0 on failure
 */
int msg_cmd_is_valid(void)
{
	int cmd_cnt = 0;

	msg_read(STAT_ID_REG);

	/* Extract the CMD_ID field of 16 bits */
	cmd_id = ipc_reg_r & 0xffff;

	for(; cmd_cnt < sizeof(valid_cmd_id)/sizeof(short); cmd_cnt++) {
		if(valid_cmd_id[cmd_cnt] == cmd_id)
			return 1;
	}

	return 0;
}

/* Read all the IPC regs and pass it along to the appropriate handler */
void msg_cmd_dispatcher()
{
	char use_default_val = 0;

	msg_read_all();

	if ((a8_m3_data_r.reg3 == 0xffffffff) &&
		(a8_m3_data_r.reg4 == 0xffffffff))
		use_default_val = 1;

	a8_m3_ds_data.reg1 = a8_m3_data_r.reg3;
	a8_m3_ds_data.reg2 = a8_m3_data_r.reg4;

	cmd_global_data.data = &a8_m3_ds_data;
	cmd_global_data.cmd_id = cmd_id;

	switch(cmd_id) {
	case 0x1:
		a8_lp_cmd1_handler(&cmd_global_data, use_default_val);	/* RTC */
		break;
	case 0x2:
		a8_lp_cmd2_handler(&cmd_global_data, use_default_val);	/* RTC_fast */
		break;
	case 0x3:
		a8_lp_cmd3_handler(&cmd_global_data, use_default_val);	/* DS0 */
		break;
	case 0x5:
		a8_lp_cmd5_handler(&cmd_global_data, use_default_val);	/* DS1 */
		break;
	case 0x7:
		a8_lp_cmd7_handler(&cmd_global_data, use_default_val);	/* DS2 */
		break;
	case 0x9:
		a8_standalone_handler(&cmd_global_data);
		break;
	case 0xb:
		a8_standby_handler(&cmd_global_data, use_default_val);	/* Standby */
		break;
	case 0xe:
		init_m3_state_machine();	/* Reset M3 state machine */
		break;
	case 0x10:
		a8_cpuidle_handler(&cmd_global_data, use_default_val);	/* cpuidle MPU Clock gating */
		break;
	case 0xf:
	default:
		m3_firmware_version();
	}
}

void m3_firmware_version(void)
{
	msg_read(PARAM1_REG);
	ipc_reg_r &= 0xffff0000;
	ipc_reg_w = ipc_reg_r | CM3_VERSION;
	msg_write(PARAM1_REG);
}

void msg_cmd_stat_update(int cmd_stat_value)
{
	msg_read(STAT_ID_REG);
	ipc_reg_r &= 0x0000ffff;
	ipc_reg_w = ipc_reg_r | (cmd_stat_value << 16);
	msg_write(STAT_ID_REG);
}

/*
 * Check whether command needs a trigger or not
 * returns 1 if trigger is needed
 * returns 0 if trigger is not needed (eg: checking the version)
 */
int msg_cmd_needs_trigger(void)
{
	msg_read(STAT_ID_REG);

	/* Version and state machine reset commands do not need a trigger */
	if ((cmd_id == 0xf || cmd_id == 0xe))
		return 0;
	else
		return 1;
}
