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
#define DisableInterrupts asm(" CPSID i");
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
int timer = 1000000;
int timer2 = 1000000;
int timer3 = 1000000;
int mode = 0;
int data_to_handle = 0;
char data;
void main()
{
	tsi_calibrate_tresholds();
	EnableInterrupts;


	while(1)
	{
		/*if(data_to_handle){
			data_to_handle = 0;
			//uart_send(data);
		}*/
		
		while(timer3--);
		timer3 = 1000000;
		led_toggle(LED_YELLOW);
	}
}

void uart_handler(void)
{		
	//DisableInterrupts; // do i have to do this?
	uart_send(uart_read());
	UART2_S1; 	//this
	UART2_D;	//and this:Clears the RDRF (Verified to work)
	NVICICPR1 |= 1 << (49 % 32);  //make sure to clear the interrupt(not needed)
	while(1) 
	{	
		timer2 = 1000000;
		while(timer2--);
		timer2 = 1000000;
		led_toggle(LED_BLUE);
	}
	//while(timer2--);
	//timer2 = 1000000;
	//led_toggle(LED_BLUE);
	//data_to_handle = 1;
	//data = uart_read();//modified so it doenst clear.
	//uart_send(data); 
	//UART2_S1; 	//this
	//UART2_D;	//and this:Clears the RDRF (Verified to work)
	//EnableInterrupts;
}

void pit_handler(void)
{	
	//PIT_LDVAL0 = 1000000; just for try
	NVICICPR2 |= 1 << (68 % 32); //make sure to clear the interrupt
	PIT_TFLG0 = PIT_TFLG_TIF_MASK; //Timer Interrupt Flag cleared only by writing it with 1. Here or down?
	NVICICPR2 |= 1 << (68 % 32); //make sure to clear the interrupt 
	//timer = 1000000;
	/*while(1) 
	{
		while(timer--);
		timer = 1000000;
		led_toggle(LED_RED);
	}*/
	led_toggle(LED_RED);//will be seen without a timer.. 
}

void led_update(int led_number)
{
	int i;
	for(i=0;i< NUMBER_OF_LEDS ;i++){
		led_off(i);
	}
	led_on(led_number);
}
