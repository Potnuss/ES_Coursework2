#ifndef tsi_H__
#define tsi_H__


void tsi_init();
void tsi_calibrate_tresholds();
void tsi_scan();
int tsi_update_last_active_button();
int tsi_get_value_from_button(int);

#endif
