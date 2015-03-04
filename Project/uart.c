#include "uart.h"
#include "MK70F12.h"

void uart_init()
{
	//Uart init
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;  // Enable PORTE clock 
	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK; //Enable the clock signal
	
	UART2_BDL = 27; //select baud rate
	
	UART2_C2 |= UART_C2_TE_MASK; //Enable transmission

	// Configure the UART pins to be 
	PORTE_PCR17 = PORT_PCR_MUX(3);//receiver
	PORTE_PCR16 = PORT_PCR_MUX(3);//transmission	
}

void uart_send(char data)
{
	UART2_D = data;
}
