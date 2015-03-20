/*
 * ES Coursework Part 2, created by Pontus Petersson, March 2015
 * Embedded Systems, University of Edinburgh
 */
#include "uart.h"
#include "MK70F12.h"

/*
 * Function:  uart_init
 * --------------------
 * Initializes UART2, makes transmit and recieve work
 *
 * Sets Baud rate to 115200
 * UART baud rate = (50*10^6) / (16 × 27) = 115740 --> 0.5% error
 * UART baud rate = UART module clock / (16 × (SBR[12:0] + BRFD))
 *
 * NOTE: Needs the UART module clock to run in 50Mhz
 * NOTE: UART2_BDH and UART2_BRFD has to be zero(they are as default after reset)
 */
void uart_init()
{
	/*Enable UART2 interrupt in Nested Vectored Interrupt Controller 
	UART2: IRQ49 and Vector65*/

	//Clear potentially pending PIT interrupts
	NVICICPR1 |= 1 << (49 % 32); 

	//Enable UART2 interrupts, Interrupt Set-Enable Registers
	NVICISER1 |= 1 << (49 % 32);

	//Set Priority for UART2 interrupts
	//Group Priority 0 (with settings:bits7-4 Group, bits 3-0 subgroup)
	NVICIP49 = 0x00; 


	// Enable clock gating to the correct pin port (E)
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	//Enable the clock signal for UART2
	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK; 
	
	/*Select baud rate 115200
	 * UART baud rate = (50*10^6) / (16 × 27) = 115740 --> 0.5% error
	 * UART baud rate = UART module clock / (16 × (SBR[12:0] + BRFD))
	 * where BDL is part of SBR 
	 */
	UART2_BDL = 27; 
	
	//Enables: Generate interrupt requests when new data is recieved 
	UART2_C2 |= UART_C2_RIE_MASK;

	//Enables the UART transmitter and receiver
	UART2_C2 |= UART_C2_TE_MASK; 
	UART2_C2 |= UART_C2_RE_MASK;

	// Configure the UART pins to be 
	PORTE_PCR17 = PORT_PCR_MUX(3);//receiver
	PORTE_PCR16 = PORT_PCR_MUX(3);//transmission	
}
/*
 * Function:  uart_send
 * --------------------
 * Sends one byte over UART2
 */
void uart_send(char data)
{	
	//Wait for Transmit Data Register Empty Flag
	while(!(UART2_S1 & UART_S1_TDRE_MASK )){}

	//Send data over UART2     
	UART2_D = data;
}

/*
 * Function:  uart_read
 * --------------------
 * Read one byte from UART2
 * NOTE: if UART2_S1 recently has been read, followed by a read of UART2_D, 
 *	this will result in clearing the uart interrupt (RDRF)
 *  returns: the recieved byte 
 */
char uart_read()
{	
	//Read data from UART2
	return UART2_D;
}
