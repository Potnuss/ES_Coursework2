/* periodic interrupt timer delivers interrupts and can be used to poll for TSI */
#include "pit.h"
#include "MK70F12.h"

void pit_init(int time)
{


	//Enable PIT interrupt in Nested Vectored Interrupt Controller 
	NVICICPR2 |= 1 << (68 % 32); 
	//NVIC_ICPRn[31:0] are the clear-pending bits for interrupts (31+(32*n)) - (32*n). = 95 - 64

	NVICISER2 |= 1 << (68 % 32);
	//NVIC_ISERn[31:0] are the set-enable bits for interrupts (31+(32*n)) - (32*n). = 95 - 64

	//NVICIP68 |= 3;
	//NVICIP17 |= NVIC_IP_PRI2(0);
	//NVICIP17 = 0x00000003u; //pit prio 3
	NVICIP17 = 0x00000000u; //pit 0

	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //PIT clock gate control

	PIT_MCR &= ~PIT_MCR_MDIS_MASK; //enable the module clock. This bit must be enabled before any other setup is done.

	


	PIT_LDVAL0 = time; //Timer Start Value BitsThese bits set the timer start value

	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enables the timer | Interrupt will be requested whenever TIF is set.

	//No need for Timer Flag Register (PIT_TFLGn)?
}
