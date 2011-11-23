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

#include  <stdint.h>

extern unsigned int _end_stack;
extern unsigned int _end_text;
extern unsigned int _start_data;
extern unsigned int _end_data;
extern unsigned int _start_bss;
extern unsigned int _end_bss;

extern int main(void);

#define DUMMY_ATTRIB __atrribute__ ((weak, alias ("dummy_handler")));

void reset_handler(void);
void nmi_handler(void) 		__attribute__ ((weak, alias("dummy_handler")));
void hardfault_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void memmanage_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void busfault_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void usagefault_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void svc_handler(void)		__attribute__ ((weak, alias("dummy_handler")));
void debugmon_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void pendsv_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void systick_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void extint0_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint1_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint2_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint3_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint4_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint5_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint6_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint7_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint8_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint9_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint10_handler(void)	__attribute__ ((weak, alias("dummy_handler")));
void extint11_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint12_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint13_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint14_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint15_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint16_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint17_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint18_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint19_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint20_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint21_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint22_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint23_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint24_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint25_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint26_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint27_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint28_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint29_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint30_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint31_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint32_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint33_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint34_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint35_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint36_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint37_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint38_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint39_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint40_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint41_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint42_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint43_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint44_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint45_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint46_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint47_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint48_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint49_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint50_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));
void extint51_handler(void) 	__attribute__ ((weak, alias("dummy_handler")));

void dummy_handler(void);

/* the vector table */
void *vector_table[] __attribute__ ((section(".vectors"))) = {
	&_end_stack,
	reset_handler,
	nmi_handler,
	hardfault_handler,
	memmanage_handler,
	busfault_handler,
	usagefault_handler,
	0,
	0,
	0,
	0,
	svc_handler,
	debugmon_handler,
	0,
	pendsv_handler,
	systick_handler,

	extint0_handler,
	extint1_handler,
	extint2_handler,
	extint3_handler,
	extint4_handler,
	extint5_handler,
	extint6_handler,
	extint7_handler,
	extint8_handler,
	extint9_handler,
	extint10_handler,
	extint11_handler,
	extint12_handler,
	extint13_handler,
	extint14_handler,
	extint15_handler,
	extint16_handler,
	extint17_handler,
	extint18_handler,
	extint19_handler,
	extint20_handler,
	extint21_handler,
	extint22_handler,
	extint23_handler,
	extint24_handler,
	extint25_handler,
	extint26_handler,
	extint27_handler,
	extint28_handler,
	extint29_handler,
	extint30_handler,
	extint31_handler,
	extint32_handler,
	extint33_handler,
	extint34_handler,
	extint35_handler,
	extint36_handler,
	extint37_handler,
	extint38_handler,
	extint39_handler,
	extint40_handler,
	extint41_handler,
	extint42_handler,
	extint43_handler,
	extint44_handler,
	extint45_handler,
	extint46_handler,
	extint47_handler,
	extint48_handler,
	extint49_handler,
	extint50_handler,
	extint51_handler,
};

/* so it begins... */
void reset_handler(void)
{
	unsigned int *pulsrc, *puldest;

	/*
	 * copy the data segment initializers from flash to sram.
	 */
	pulsrc = &_end_text;
	for(puldest = &_start_data; puldest < &_end_data; )
	{
	    *puldest++ = *pulsrc++;
	}

	/*
	 * zero fill the bss segment.  this is done with inline assembly since this
	 * will clear the value of puldest if it is not kept in a register.
	 */
	__asm("    ldr     r0, =_start_bss\n"
	      "    ldr     r1, =_end_bss\n"
	      "    mov     r2, #0\n"
	      "    .thumb_func\n"
	      "zero_loop:\n"
	      "        cmp     r0, r1\n"
	      "        it      lt\n"
	      "        strlt   r2, [r0], #4\n"
	      "        blt     zero_loop");

	/*
	 * call the application's entry point.
	 */
	main();
}

void dummy_handler(void)
{
	while(1)
	;
}
