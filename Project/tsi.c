#include "MK70F12.h"
#include "tsi.h"
#include "led.h"

#define NUMBER_OF_BUTTONS 4

int button_thresholds[4] = {0,0,0,0};
int last_active_button = 0;

void tsi_init()
{

	SIM_SCGC5 |= SIM_SCGC5_TSI_MASK | SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	//I/O Connectors
	PORTA_PCR4 = PORT_PCR_MUX(0); //E1Touch Alt0
	PORTB_PCR3 = PORT_PCR_MUX(0); //E2Touch Alt0
	PORTB_PCR2 = PORT_PCR_MUX(0); //E3Touch Alt0
	PORTB_PCR16 = PORT_PCR_MUX(0); //E4Touch Alt0

	//Touch Sensing Input Module Disable - while setup
	TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;

	//General Control and Status Register TSI0 p2164
	//Electrode Oscillator Frequency divided by 1 (says 2 in pm)
	//Number of Consecutive Scans per electrode - 16 times 
	//Scan Trigger Mode. 0 Software trigger scan 1 Periodical Scan.
	// End-of-Scan flag Need to reset?
	//TSI0_GENCS |= TSI_GENCS_PS(0) | TSI_GENCS_NSCN(15) | TSI_GENCS_STM_MASK | TSI_GENCS_EOSF_MASK; //Periodical Scan, Work divide by 1
	//TSI0_GENCS |= TSI_GENCS_PS(1) | TSI_GENCS_NSCN(15) | TSI_GENCS_STM_MASK | TSI_GENCS_EOSF_MASK; //Periodical Scan, Works divide by 2
	TSI0_GENCS |= TSI_GENCS_PS(1) | TSI_GENCS_NSCN(15) | TSI_GENCS_EOSF_MASK;//Software trigger scan

	//SCAN Control Register
	//External OSC Charge Current 12 μA
	//Ref OSC Charge Current 24 μA
	//Active Mode Prescaler Input Clock Source divided by 4.
	//Active Mode Clock Source - External Oscillator 0
	TSI0_SCANC |= TSI_SCANC_EXTCHRG(5) | TSI_SCANC_REFCHRG(11) | TSI_SCANC_AMPSC(2) | TSI_SCANC_AMCLKS(2); 

	// Pin Enable Register 5,7,8,9
	TSI0_PEN |= TSI_PEN_PEN5_MASK | TSI_PEN_PEN7_MASK | TSI_PEN_PEN8_MASK | TSI_PEN_PEN9_MASK;

	//Touch Sensing Input Module Enable again
	TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;
}

void tsi_calibrate_tresholds() 
{
	int i;
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

int tsi_update_active_button()
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