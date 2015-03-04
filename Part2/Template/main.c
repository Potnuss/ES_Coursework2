/*
 * Bare metal template created by Harry Wagstaff
 * Based on Freescale Codewarrior Bareboard project template
 */

#include "MK70F12.h"

#include "main.h"
#include "led.h"

// __init_hardware is called by the Freescale __thumb_startup function (see 
// vectors.c)
void __init_hardware()
{
	// Disable the Watchdog module. This module is designed to reset
	// the board in case it crashes for some reason. We don't need it,
	// so we disable it here.l
	WDOG_UNLOCK = 0xC520;
	WDOG_UNLOCK = 0xD928;
	WDOG_STCTRLH = 0xD2;

	// Configure the MCG - set up clock dividers on 
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(1);
	MCG_C1 = MCG_C1_CLKS(2);

	led_init();
}

void main()
{
	while(1)
	{
		int timer = 10000000;
		while(timer--) ;

		led_toggle(LED_RED);
		led_toggle(LED_BLUE);
	}
}


