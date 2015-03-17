#include "MK70F12.h"
#include "tsi.h"
#include "led.h"

#define NUMBER_OF_BUTTONS 4

//Stores the treshold value (for beeing touched) for each button
int button_thresholds[4] = {0,0,0,0};

//Stores the last pushed button
int last_active_button = 0;


void tsi_init()
{
	// Enable the clock signal for TSI unit
	// Enable clock gating to the correct pin ports (A and B)
	SIM_SCGC5 |= SIM_SCGC5_TSI_MASK | SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	//Multiplexing and Pin Assignments
	PORTA_PCR4 = PORT_PCR_MUX(0); //E1Touch Alt0 for TSI0_CH5
	PORTB_PCR3 = PORT_PCR_MUX(0); //E2Touch Alt0 for TSI0_CH8
	PORTB_PCR2 = PORT_PCR_MUX(0); //E3Touch Alt0 for TSI0_CH7
	PORTB_PCR16 = PORT_PCR_MUX(0); //E4Touch Alt0 for TSI0_CH9

	//Touch Sensing Input Module Disable - during setup 
	TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;

	//General Control and Status Register TSI0 (page 2164)
	// Note that: Software trigger scan by default (STM=0)
	//PS: Electrode Oscillator Frequency divided by 2
	//NSCN: Number of Consecutive Scans per electrode - 16 times
	//EOSF: End-of-Scan flag reset (just for sure)
	TSI0_GENCS |= TSI_GENCS_PS(1) | TSI_GENCS_NSCN(15) | TSI_GENCS_EOSF_MASK;

	//SCAN Control Register
	//EXTCHRG: External OSC Charge Current 12 μA
	//REFCHRG: Ref OSC Charge Current 24 μA
	//AMPSC: Active Mode Prescaler Input Clock Source divided by 4.
	//AMCLKS: Active Mode Clock Source - External Oscillator 0
	TSI0_SCANC |= TSI_SCANC_EXTCHRG(5) | TSI_SCANC_REFCHRG(11) | TSI_SCANC_AMPSC(2) | TSI_SCANC_AMCLKS(2); 

	// Pin Enable Register 5,7,8,9 The corresponding pins is used by TSI.
	TSI0_PEN |= TSI_PEN_PEN5_MASK | TSI_PEN_PEN7_MASK | TSI_PEN_PEN8_MASK | TSI_PEN_PEN9_MASK;

	//Touch Sensing Input Module Enable
	TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;
}

void tsi_calibrate_tresholds() 
{
	int i;

	//Scan to get the initial scan values
	//NOTE: Do not touch buttons during this time!
	tsi_scan();


	for(i = 0; i < NUMBER_OF_BUTTONS; i++) {
		button_thresholds[i] = tsi_get_value_from_button(i) + tsi_get_value_from_button(i)/50;
	}
}


void tsi_scan()
{
	TSI0_GENCS |= TSI_GENCS_SWTS_MASK; // Software Trigger Start, Start scan
	while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK)); // wait for End of Scan Flag.
	TSI0_GENCS |= TSI_GENCS_EOSF_MASK; //End of Scan Flag. Writing "1" to this bit will clear the flag to 0.	
}

int tsi_update_last_active_button()
{
	int i;
	tsi_scan();
	for(i = 0; i < NUMBER_OF_BUTTONS; i++) {
		if(tsi_get_value_from_button(i) > button_thresholds[i]) 
			last_active_button = i;
	}		
	return last_active_button;		
}

int tsi_get_value_from_button(int button) {
    switch (button) {
        case 0: return ((TSI0_CNTR5 & TSI_CNTR5_CTN_MASK) >> (TSI_CNTR5_CTN_SHIFT));
        case 1: return ((TSI0_CNTR9 & TSI_CNTR9_CTN1_MASK) >> (TSI_CNTR9_CTN1_SHIFT));
        case 2: return ((TSI0_CNTR7 & TSI_CNTR7_CTN_MASK) >> (TSI_CNTR7_CTN_SHIFT));
        case 3: return ((TSI0_CNTR9 & TSI_CNTR9_CTN_MASK) >> (TSI_CNTR9_CTN_SHIFT));
    }
//    return -1;
}