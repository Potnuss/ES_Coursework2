#ifndef __main_h_
#define __main_h_

#include <stdint.h>
extern void uart_handler(void);
extern void pit_handler(void);
void led_update(int);
#endif /* __main_h_ */

