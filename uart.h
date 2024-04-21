/* 
 * File:   uart.h
 * Author: yuhsienchang
 *
 * Created on April 19, 2024, 10:44 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
    
    
#define ENABLE_RC3_UART_TX()  (RC3PPS = 0x0F)
#define DISABLE_RC3_UART_TX() (RC3PPS = 0x00)
#define ENABLE_RC5_UART_TX()  (RC5PPS = 0x0F)
#define DISABLE_RC5_UART_TX() (RC5PPS = 0x00)

#define set_RA5_as_UART_TX()              (RA5PPS = 0x0F)
#define reset_RA5_peripheral_pin_select() (RA5PPS = 0x00)

    
    
uint8_t tmp;
#define RX_BUFFER_SIZE 32
volatile uint8_t  rxbuffer[RX_BUFFER_SIZE];
volatile uint8_t  rxbuffer_index = 0;
volatile uint32_t uart_rx_ticks = 0;
volatile uint8_t  uart_rx_package_event = 0;
typedef enum {IDLE, BUSY} uart_rx_states_t;
volatile uart_rx_states_t uart_rx_states = 0;

volatile uint8_t is_RA5_button_pressed = 0;
volatile uint8_t is_printing = 0;

    
    
void usart_init();
void putch(uint8_t data);

void EUSART1_Write(uint8_t txData);
char EUSART1_Read();

void EUSART1_Write_1(char *txbuf, uint8_t length);
char EUSART1_Read_1(uint8_t length);



void print_buffer(uint8_t *buf, uint8_t length);

// enable uart printf and disable tx_transmit
void enable_uart_printf();

// enable tx_transmit and disable uart_printf
void enable_tx_transmit();

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

