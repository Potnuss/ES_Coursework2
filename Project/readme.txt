Pontus Petersson, s1477669
ES Coursework Part 2
Embedded Systems, University of Edinburgh

This code project for Freescale Kinetis K70 microcontroller, is used to filter (bandpass) a audiostream (signed 8-bit PCM audio at 8000Hz) over UART2 (baudrate 115200). 

The device goes into low-power mode and waits for interrupts in the end of the main-while-loop.

The device wakes up when (1), PIT-interrupt occours, this will do a scan of the touch buttons.
The device wakes up when (2), New data is recieved over UART2, this will filter the recieved sample and send back the result. 

NOTE: The filtering is done inside UART_handler to make sure that the filtering is done in time, no matter what the user does in the main-while-loop. 

NOTE: The tsi_scan() waits until the scan is done. 
	while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK));
	This takes time, concider this if you wanna add things to this project.
	(Improvement can be done here in next version)

NOTE: When initializing the system, Do not touch the buttons during this time.
	( see tsi_calibrate_tresholds() )

NOTE: If a multipe buttons were touched during scan the 
	button with the highest index will choosen as active.
	( see tsi_update_last_active_button() )

NOTE: Interrupt Prioroties and how many bits that belongs to Group vs. SubGroup can be configured
	in AIRCR.PRIGROUP. 
	AIRCR.PRIGROUP is 000 as default --> 1 bit subgroup, according to Manuals.
	But after investigation it seem to be 011 --> 4 bits of subgroup see ARM manual page 636.

NOTE: In led.h (from template project) the following changes has been done:
	led.h
	//#define LED_BLUE 0
	//#define LED_RED 1
	//#define LED_YELLOW 2
	//#define LED_GREEN 3
	
	#define LED_RED 0
	#define LED_YELLOW 1
	#define LED_GREEN 2
	#define LED_BLUE 3

