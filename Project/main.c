/*
 * Bare metal template created by Harry Wagstaff
 * Based on Freescale Codewarrior Bareboard project template
 */

#include "MK70F12.h"
#include "main.h"
#include "led.h"
#include "uart.h"
#include "filter.h"
#include "fpu.h"
#include "pit.h"
#include "tsi.h"

#define EnableInterrupts asm(" CPSIE i");
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
	
	
	uart_init();
	led_init();
	fpu_init();
	pit_init(1000000);
	tsi_init();
}
int timer = 0;
int mode = 0;
void main()
{
	tsi_calibrate_tresholds();
	EnableInterrupts;


	while(1)
	{
		int timer = 10000000;
		while(timer--) ;

		//led_toggle(LED_RED);
	}
}

void uart_handler(void)
{
	uart_send(filter(uart_read(),0));
}

void pit_handler(void)
{
	
	PIT_TFLG0 = PIT_TFLG_TIF_MASK; //Timer Interrupt Flag cleared only by writing it with 1.
	if(timer >= 10) 
	{
		//led_toggle(LED_BLUE);
		timer = 0;
	}
	timer++;
	
	    mode = tsi_update_active_button();
	    led_update(mode);
}

void led_update(int led_number)
{
	int i;
	for(i=0;i< NUMBER_OF_LEDS ;i++){
		led_off(i);
	}
	led_on(led_number);
}