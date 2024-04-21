#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
//#include "tmr0.h"
#include "measurement.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "led.h"
#include "uart.h"
#include "monitor_sync.h"
#include <string.h>


void measurement(States *state, SensorValue *value){
    LED_D2_ON();
    // ------------------------------------
    enable_uart_printf();
    printf("[Request] Send data request to pH485_probe - MEASUREMENT MODE ---  \r\n");
    __delay_ms(10);
    enable_tx_transmit();
    // ------------------------------------
    // request ph and temp
    send_measured_command();
    LED_D2_OFF();
//    *state = STATE_MONITORING_POWER_FOR_SYNC;
};

void send_measured_command() {
    char txbuf[32] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x04, 0x15, 0xC9};
    EUSART1_Write_1(txbuf, 8);
};

void get_measured_value(SensorValue *sensor_value) {
    enable_uart_printf();
    printf("[Receive] data from pH485_probe \r\n");
    __delay_ms(10);
    enable_tx_transmit();
//    float ph = 0;
//    uint32_t payload = ((uint32_t)rxbuffer[5] << 24) | 
//                       ((uint32_t)rxbuffer[6]) << 16 | 
//                       ((uint32_t)rxbuffer[3]) << 8 | 
//                       ((uint32_t)rxbuffer[4] << 0);
//    memcpy(&ph, &payload, sizeof(float));
//    sensor_value->ph_value = (uint16_t)(ph * 100);
////    ph = (uint16_t)(ph * 100);
//    printf("PH: %u\r\n", sensor_value->ph_value);
    
    uint16_t ph = 0;
    uint32_t payload_ph = ((uint32_t)rxbuffer[5] << 24) | 
                       ((uint32_t)rxbuffer[6]) << 16 | 
                       ((uint32_t)rxbuffer[3]) << 8 | 
                       ((uint32_t)rxbuffer[4] << 0);
    memcpy(&ph, &payload_ph, sizeof(uint16_t));
    sensor_value->ph_value = payload_ph;
//    printf("PH: %u\r\n", sensor_value->ph_value);
    // ------------------------------------
//    float temp = 0;
//    uint32_t payload_temp = ((uint32_t)rxbuffer[9] << 24) | 
//                       ((uint32_t)rxbuffer[10]) << 16 | 
//                       ((uint32_t)rxbuffer[7]) << 8 | 
//                       ((uint32_t)rxbuffer[8] << 0);
//    memcpy(&temp, &payload_temp, sizeof(float));
////    uint16_t temp2 = (uint16_t)(temp * 100);
//    sensor_value->temp_value = (uint16_t)(temp * 100);
//    printf("TEMP: %u\r\n", sensor_value->temp_value);
    
    uint16_t temp = 0;
    uint32_t payload_temp = ((uint32_t)rxbuffer[9] << 24) | 
                       ((uint32_t)rxbuffer[10]) << 16 | 
                       ((uint32_t)rxbuffer[7]) << 8 | 
                       ((uint32_t)rxbuffer[8] << 0);
    memcpy(&temp, &payload_temp, sizeof(uint16_t));
    sensor_value->temp_value = payload_temp;
//    printf("TEMP: %u\r\n", sensor_value->temp_value);
};


