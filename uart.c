

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include "i2c_slave.h"
#include "uart.h"
#include "myisr.h"

void usart_init() {
    PIE3bits.RC1IE = 0;
    PIE3bits.TX1IE = 0;
    
    // Baud rate 9600
    BAUD1CON = 0x08;
    SP1BRGL = 0x19;
    SP1BRGH = 0x00;
    
    TX1STA = 0x24;
    RC1STA = 0x90;

    PIE3bits.RC1IE = 1;
};


void putch(uint8_t data)
{
    while(0 == PIR3bits.TX1IF);
    TX1REG = data;
}

void EUSART1_Write(uint8_t txData)
{
    while(0 == PIR3bits.TX1IF);
    TX1REG = txData;    // Write the data byte to the USART.
};

// read 1 word
char EUSART1_Read() {
//    while(!PIR3bits.RC1IF); // wait complete
//    return RC1REG;           // return data;
    return rxbuffer;
};

void EUSART1_Write_1(char *txbuf, uint8_t length) {
    for(uint8_t i=0; i<length; i++){
        EUSART1_Write(txbuf[i]);
    };
};

char EUSART1_Read_1(uint8_t length) {
    char data[32]= {0};
    for(uint8_t i=0; i<length; i++){
        data[i] = EUSART1_Read();
    };
    return data;
};


void __interrupt() INTERRUPT_InterruptManager() {

    /* Interrupt : TMR0 is a timer/counter module used to generate interrupt requests at specific time intervals .
     * Used to indicate whether the TMR0 interrupt has triggered, if so, it will be set to 1.
    */
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        ticks++;
//        printf("ticks: %u", ticks);
        
        if((uart_rx_states == BUSY) && 
           (ticks - uart_rx_ticks > 200)) {
            uart_rx_states = IDLE; 
            uart_rx_package_event = 1;
        };
        
//        if(is_printing) {
//            if(is_RA5_button_pressed){
//                
//            };
//        } else {
//            
//        };
        
    }

    // I2C slave interrupt
    if (PIR3bits.SSP1IF) {
        PIR3bits.SSP1IF = 0;
        i2c_slave_interrupt();
    }
    
    /* This is the receive interrupt flag bit for the USART module.
     * The RC1IF bit is used to indicate whether the USART receive buffer has data available for reading, if so it will be set to 1.
     */
    //if (PIE3bits.RC1IE == 1 && PIR3bits.RC1IF == 1){
    // Mean: UART receive new data.
    if (PIR3bits.RC1IF){ // Interrupt flag bit of target TMR0 module
        //IO_RC2_Toggle();
        
        uart_rx_ticks = get_ticks();
        uart_rx_states = BUSY;  // receive-ing.
        rxbuffer[rxbuffer_index] = RC1REG;
        rxbuffer_index++;
        rxbuffer_index = rxbuffer_index % RX_BUFFER_SIZE;
//        PIR3bits.RC1IF = 0; // 
    }     
};


void print_buffer(uint8_t *buf, uint8_t length) {

    DISABLE_RC3_UART_TX();
    set_RA5_as_UART_TX();

    for(uint8_t i=0; i<length; i++){
      printf("%02X ", buf[i]);
    }
    printf("\r\n");
    
    __delay_ms(10);
    ENABLE_RC3_UART_TX();
    reset_RA5_peripheral_pin_select();
}


void enable_uart_printf() {
    DISABLE_RC3_UART_TX();
    set_RA5_as_UART_TX();
};
void enable_tx_transmit() {
    __delay_ms(10);
    ENABLE_RC3_UART_TX();
    reset_RA5_peripheral_pin_select();
};

