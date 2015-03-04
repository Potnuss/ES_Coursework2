#ifndef UART_H__
#define UART_H__


extern void uart_init();
extern void uart_send(char);
extern char uart_read();
extern int uart_new_data();

#endif /* #ifndef UART_H__ */

