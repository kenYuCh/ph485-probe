#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include "configuration.h"
#include "i2c_slave.h"
#include "tmr0.h"
#include "my_helpers.h"


#define ENABLE_RC3_UART_TX()  (RC3PPS = 0x0F)
#define DISABLE_RC3_UART_TX() (RC3PPS = 0x00)
#define ENABLE_RC5_UART_TX()  (RC5PPS = 0x0F)
#define DISABLE_RC5_UART_TX() (RC5PPS = 0x00)

#define set_RA5_as_UART_TX()              (RA5PPS = 0x0F)
#define reset_RA5_peripheral_pin_select() (RA5PPS = 0x00)

#define LED_D2_ON()           IO_RC2_SetLow()
#define LED_D2_OFF()          IO_RC2_SetHigh()
#define LED_D2_TOGGLE()       IO_RC2_Toggle()


void print_buffer(uint8_t *buf, uint8_t length){

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


uint8_t tmp;
#define RX_BUFFER_SIZE 32
volatile uint8_t  rxbuffer[RX_BUFFER_SIZE];
volatile uint8_t  rxbuffer_index = 0;
volatile uint32_t uart_rx_ticks = 0;
volatile uint8_t  uart_rx_package_event = 0;

typedef enum {IDLE, BUSY} uart_rx_states_t;
volatile uart_rx_states_t uart_rx_states = 0;


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
}


void putch(uint8_t data)
{
    while(0 == PIR3bits.TX1IF);
    TX1REG = data;
}

void EUSART1_Write(uint8_t txData)
{
    while(0 == PIR3bits.TX1IF);
    TX1REG = txData;    // Write the data byte to the USART.
}


void set_device_serial_number_to_i2c_slave_memorymap(){
    for (uint8_t i = 0; i < 5; i++) {
        char buffer[3];
        buffer[0] = DEVICE_SERIAL[i * 2];
        buffer[1] = DEVICE_SERIAL[i * 2 + 1];
        buffer[2] = '\0';
        //printf("%s\r\n", buffer);
        
        uint8_t value = (uint8_t)strtol(buffer, NULL, 16);
        i2c_slave_set_register_with_lock(57u + i, value);
    }    
}


volatile uint32_t ticks = 0;
uint32_t get_ticks(){
    return ticks;
}
uint32_t get_ticks_with_lock(){
    GIE = 0;
    uint32_t result = ticks;
    GIE = 1;
    return result;
}


uint32_t calculate_diff(uint32_t now, uint32_t prev_ticks) {
    uint32_t diff;
    if (now >= prev_ticks) {
        diff = now - prev_ticks;
    } else {
        // Handling overflow situation
        diff = (UINT32_MAX - prev_ticks) + now + 1;
    }
    return diff;
}


void __interrupt() INTERRUPT_InterruptManager() {

    // Interrupt
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        
        ticks++;
        
        
        if((uart_rx_states == BUSY) 
            && 
           (ticks - uart_rx_ticks > 100))
        {
            uart_rx_states = IDLE; 
            uart_rx_package_event = 1;
        }
        
    }

    // I2C slave interrupt
    if (PIR3bits.SSP1IF) {
        PIR3bits.SSP1IF = 0;

        i2c_slave_interrupt();
    }
    
    
    //if (PIE3bits.RC1IE == 1 && PIR3bits.RC1IF == 1){
    if (PIR3bits.RC1IF){
        
        //IO_RC2_Toggle();
        
        uart_rx_ticks = get_ticks();
        uart_rx_states = BUSY; 
        rxbuffer[rxbuffer_index] = RC1REG;
        rxbuffer_index++;
        rxbuffer_index = rxbuffer_index % RX_BUFFER_SIZE;
    }     
}



void main() {

    SYSTEM_Initialize();
    tmr0_init();
    usart_init();
    i2c_slave_init();


    DISABLE_RC3_UART_TX();
    set_RA5_as_UART_TX();
    printf("PH Probe with RS485\r\n");
    printf("sn: %s\r\n", DEVICE_SERIAL);
    __delay_ms(10);
    ENABLE_RC3_UART_TX();
    reset_RA5_peripheral_pin_select();
    
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;


    while (1) {

        uint32_t now = get_ticks_with_lock();
        static uint32_t prev_ticks1 = 0;
        if(calculate_diff(now, prev_ticks1) >= (4*1000LU)){
            prev_ticks1 = now;
            LED_D2_ON();

            DISABLE_RC3_UART_TX();
            set_RA5_as_UART_TX();
            printf("[%lu] Request\r\n", now);
            __delay_ms(10);
            ENABLE_RC3_UART_TX();
            reset_RA5_peripheral_pin_select();
            
            // request 
            char txbuf[32] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x04, 0x15, 0xC9};
            for(uint8_t i=0; i<8; i++){
                EUSART1_Write(txbuf[i]);
            };
            LED_D2_OFF();
        }
        
        
        
        if(uart_rx_package_event){
            uart_rx_package_event = 0;
            LED_D2_ON();

            /*DISABLE_RC3_UART_TX();
            set_RA5_as_UART_TX();
            print_buffer(rxbuffer, rxbuffer_index);
            __delay_ms(10);
            ENABLE_RC3_UART_TX();
            reset_RA5_peripheral_pin_select();
            */

            rxbuffer_index = 0;

            // arrange bytes oders.
            float ph = 0;
            uint32_t payload = ((uint32_t)rxbuffer[5] << 24) | 
                               ((uint32_t)rxbuffer[6]) << 16 | 
                               ((uint32_t)rxbuffer[3]) << 8 | 
                               ((uint32_t)rxbuffer[4] << 0);
            memcpy(&ph, &payload, sizeof(float));
            uint16_t ph2 = (uint16_t)(ph * 100);
            printf("PH: %u\r\n", ph2);
             
            float temp = 0;
            uint32_t payload_temp = ((uint32_t)rxbuffer[9] << 24) | 
                               ((uint32_t)rxbuffer[10]) << 16 | 
                               ((uint32_t)rxbuffer[7]) << 8 | 
                               ((uint32_t)rxbuffer[8] << 0);
            memcpy(&temp, &payload_temp, sizeof(float));
            uint16_t temp2 = (uint16_t)(temp * 100);
            printf("TEMP: %u\r\n", temp2);


            DISABLE_RC3_UART_TX();
            set_RA5_as_UART_TX();
            //printf("PH: %f\r\n", ph);
//            printf("PH: %u\r\n", ph2);
            __delay_ms(10);
            ENABLE_RC3_UART_TX();
            reset_RA5_peripheral_pin_select();
            
            

            GIE = 0;
            i2c_slave_set_register(0x00, (uint8_t)(ph2 >> 8) & 0xFF);
            i2c_slave_set_register(0x01, (uint8_t)(ph2 >> 0) & 0xFF);
            i2c_slave_set_register(0x02, (uint8_t)(temp2 >> 8) & 0xFF);
            i2c_slave_set_register(0x03, (uint8_t)(temp2 >> 0) & 0xFF);
            GIE = 1;

            LED_D2_OFF();

            DISABLE_RC3_UART_TX();
            set_RA5_as_UART_TX();
            dump_memory_map();
            printf("[%lu] PH: %u (0.01*unit)\r\n", get_ticks(), ph2);
            __delay_ms(10);
            ENABLE_RC3_UART_TX();
            reset_RA5_peripheral_pin_select();

        }
        
        now = get_ticks_with_lock();
        static uint32_t prev_ticks2 = 0;
        if(calculate_diff(now, prev_ticks2) >= 2000){
            prev_ticks2 = now;
            
            set_device_serial_number_to_i2c_slave_memorymap();
        }
        
    }
}


/* Handy note
 * 2023-08-29:
 *      use fixed-point to replace print float output for recude flash usage.
 *      use xc8 v1.45 compiler does NOT work.
 *      use xc8 v2.40 compiler work ! WHY ?
 * 2024-02-02
 *      - copy from 2023-08-31-do-probe-v2.X
 *      - hardware: UNIPCB, hardware version v2.6
 * 2024-02-04
 *      - change UART output from RC5 to RA5.
 * 2024-02-05
 *      - fix printf() issue by leave 10ms window 
 *        when switching between RC3/RA5 pin.
 *      - modify LED behavior. 
 * 
 * 
 */
