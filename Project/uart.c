#include "uart.h"
#include "MK70F12.h"

void uart_init()
{
	//Uart init
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;  // Enable PORTE clock 
	SIM_SCGC4 |= SIM_SCGC4_UART2_MASK; //Enable the clock signal
	
	UART2_BDL = 27; //select baud rate
	
	UART2_C2 |= UART_C2_TE_MASK; //TE enables the UART transmitter
	UART2_C2 |= UART_C2_RE_MASK; //RE enables the UART receiver.

	//UART2_C2 |= UART_C2_RIE_MASK;//
	// Configure the UART pins to be 
	PORTE_PCR17 = PORT_PCR_MUX(3);//receiver
	PORTE_PCR16 = PORT_PCR_MUX(3);//transmission	


}

void uart_send(char data)
{	
	while(!(UART2_S1 & UART_S1_TDRE_MASK )){}          // wait for Transmit Data Register Empty Flag
	UART2_D = data;
}

char uart_read()
{
	while(!(UART2_S1 & UART_S1_RDRF_MASK)){}           // wait for Receive Data Register Full Flag
	return UART2_D;
}

int uart_new_data()
{
	return (UART2_S1 & UART_S1_RDRF_MASK);           // return Receive Data Register Full Flag
}
