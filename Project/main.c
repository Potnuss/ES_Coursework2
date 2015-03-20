/*
 * ES Coursework Part 2, created by Pontus Petersson, March 2015
 * Embedded Systems, University of Edinburgh
 * Realtime Audio (8khz) filtering (Bandpass) over UART with baudrate baud rate 115200 
 * 
 * Based on Bare metal template created by Harry Wagstaff
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

#define EnableInterrupts asm(" CPSIE i"); //PRIMASK is cleared to 0
#define DisableInterrupts asm(" CPSID i"); //PRIMASK is set to 1
#define WaitForInterrupt asm("WFI"); //Suspend execution and enter a low-power state



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

	// Configure the MCG - set up clock dividers on (50MHz Ethernet board)
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(1);
	MCG_C1 = MCG_C1_CLKS(2);
	
	//Initializations
	uart_init();
	led_init();
	fpu_init();
	pit_init(1000000);
	tsi_init();
}

//Global variables
int mode = 0; //Choosed filter
int pit_occurred = 0; //Flag: PIT-interrupt has occoard

void main()
{
	//Calibrate capacitive buttons
	tsi_calibrate_tresholds();

	//Allows interrupts to happen
	EnableInterrupts; 

	while(1)
	{	
		// If PIT-interrupt has occoard
		if(pit_occurred)
		{	
			//Clear flag PIT-interrupt has been handled
			pit_occurred = 0;

			//Scan touchbuttons and see which one that was last pressed
			mode = tsi_update_last_active_button();

			//Update the leds to show wich filter mode that is active:
			led_update(mode);
		}

		/*Suspend execution and enter a low-power state
		and wait for interrupt.*/
		WaitForInterrupt;
	}
}

/*
 * Function:  uart_handler
 * --------------------
 * This function is called when a UART2 interrupt occurs.
 * Reads new data, produces new filter output and send that byte over uart2.
 * Clears interrupt flag in the end.
 */
void uart_handler(void)
{
	//Get new data, filter it, and send result over uart
	uart_send(filter(uart_read(),mode));

	//This sequence clears the uart interrupt (RDRF)
	UART2_S1; 	//Step 1, Read UART Status Register 1
	UART2_D;	//Step 2, Read UART Data Register
}

/*
 * Function:  pit_handler
 * --------------------
 * This function is called when a PIT interrupt occours.
 * Sets pit_occurred flag
 * Clears interrupt flag in the end.
 */
void pit_handler(void)
{	
	//Set flag PIT-interrupt has occoard
	pit_occurred = 1;
	
	//Clear Timer Interrupt Flag 
	PIT_TFLG0 = PIT_TFLG_TIF_MASK; 
}

/*
 * Function:  led_update
 * --------------------
 * Make sure that the correct led is turned on and the other ones are turned off.
 */
void led_update(int led_number)
{
	int i;

	//Turn of all LEDs
	for(i=0;i< NUMBER_OF_LEDS ;i++){
		led_off(i);
	}

	//Turn on one LED
	led_on(led_number);
}
