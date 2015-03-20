/*
 * ES Coursework Part 2, created by Pontus Petersson, March 2015
 * Embedded Systems, University of Edinburgh
 */
#include "pit.h"
#include "MK70F12.h"

/*
 * Function:  pit_init
 * --------------------
 * Initializes the PIT  
 * The input value "time" will generate a interrupt every ("time")/("Clock freq") seconds
 * NOTE: Clear potentially pending PIT interrupts
 */
void pit_init(int time)
{
	/*Enable PIT interrupt in Nested Vectored Interrupt Controller 
	PIT: IRQ68 and Vector84*/

	//Clear potentially pending PIT interrupts
	NVICICPR2 |= 1 << (68 % 32); 
	
	//Enable PIT interrupts
	NVICISER2 |= 1 << (68 % 32);

	//Set Priority for PIT interrupts
	//Group Priority 1 (with settings:bits7-4 Group, bits 3-0 subgroup)
	NVICIP68 = 0x10; 

	//PIT clock gate control
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; 

	//Enable the module clock. This bit must be enabled before any other setup is done.
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; 

	//Timer Start Value
	PIT_LDVAL0 = time; 

	//TEN: Enables the timer
	//TIE: Interrupt will be requested whenever TIF is set.
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; 

}
