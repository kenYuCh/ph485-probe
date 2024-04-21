#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include "configuration.h"
#include "i2c_slave.h"
#include "tmr0.h"
#include "my_helpers.h"
#include "uart.h"
#include "myisr.h"
#include "led.h"
#include "set_device_i2c_sn.h"
#include "mybutton.h"
#include "monitor_sync.h"
#include "measurement.h"
#include "calibration.h"



bool is_time_ups(uint32_t prev_ticks, uint32_t now, uint16_t tms) {
    uint32_t diff = calculate_diff(now, prev_ticks);
    if(diff >= tms){
        return true;
    }else {
        return false;
    };
};


void main() {
    SYSTEM_Initialize();
    tmr0_init();
    usart_init();
    i2c_slave_init();

    // ------------------------------------ Button 
    ANSELAbits.ANSA4 = 0; // digital input
    TRISAbits.TRISA4 = 1; // mode input
    ANSELAbits.ANSA5 = 0; // digital input
    TRISAbits.TRISA5 = 1; // mode input
    
    MyButton b1;
    MyButton b2;
    
    init_button(&b1);
    init_button(&b2);
    
    States state = STATE_MEASURE;
//    States state = STATE_CALIBRATE;
    uint8_t b1_clicked_count = 0;
    uint8_t b2_clicked_count = 0;
    // ------------------------------------
    SensorValue sensor_value;
    // ------------------------------------ 
    
    enable_uart_printf();
    printf("PH Probe with RS485\r\n");
    printf("sn: %s\r\n", DEVICE_SERIAL);
    __delay_ms(10);
    enable_tx_transmit();
    
    // ------------------------------------ Interrupt
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    
    while (1) {
        uint32_t now = get_ticks_with_lock();
        static uint32_t prev_ticks_btn = 0;
        
//        sensor_value.ph_value = 70;
        
//        // button polling timebase
        if(calculate_diff(now, prev_ticks_btn) >= 20){
            prev_ticks_btn = now;
            update_mybutton(&b1, PORTAbits.RA4, get_ticks_with_lock());
            update_mybutton(&b2, PORTAbits.RA5, get_ticks_with_lock());
        };
        
        switch(state) {
            static uint32_t prev_ticks = 0;
            static uint32_t prev_ticks_monitor = 0;
            static uint32_t prev_ticks_btn= 0;
            static uint32_t prev_ticks_calibrate = 0;
            now = get_ticks_with_lock();
            
//            case STATE_MONITORING_POWER_FOR_SYNC:
//                monitoring_sync(&state, &b1, &b2, &b1_clicked_count, &b2_clicked_count);
//                break;
       
            case STATE_MEASURE:
                if(calculate_diff(now, prev_ticks) >= 5000){
                    prev_ticks = now;
                    measurement(&state, &sensor_value);
//                  if(uart_rx_package_event){
                    if(1){
                        state = STATE_OUTPUT_VALUE;
                    };
                };
                __delay_ms(10);
                // This function is: if you press the button during printing, it has no effect.
                enable_uart_printf();
                monitoring_sync(&state, &b1, &b2, &b1_clicked_count, &b2_clicked_count);
                __delay_ms(10);
                enable_tx_transmit();
                break;
                
            case STATE_CALIBRATE:
                
                b1_clicked_count = 0;
                b2_clicked_count = 0;
//                
                if(calculate_diff(now, prev_ticks_calibrate) >= 5000){
                    prev_ticks_calibrate = now;
                    measured_adc();
                    __delay_ms(10);
//                    if(uart_rx_package_event){
                    if(1){
                        state =  STATE_OUTPUT_ADC;
                    };
                };
                
                monitoring_sync(&state, &b1, &b2, &b1_clicked_count, &b2_clicked_count);
                __delay_ms(10);
                dispatch(&state, &b1, &b2, &b1_clicked_count, &b2_clicked_count);
                
                break;
                
            case STATE_DISPATCH:
//                dispatch(&state, &b1, &b2, &b1_clicked_count, &b2_clicked_count);
                break;
                
            case STATE_SAVE_TOP_CALIBRATION_POINT:
                save_top_adc_point(&state, &sensor_value);
                state = STATE_CALIBRATE;
                break;
                
            case STATE_SAVE_BOTTOM_CALIBRATION_POINT:
                save_bottom_adc_point(&state, &sensor_value);
                state = STATE_CALIBRATE;
                break;
           
            case STATE_OUTPUT_ADC:
                // ---- uart_rx_buffer
                uart_rx_package_event = 0;
                rxbuffer_index = 0;
                get_measured_adc_value(&sensor_value);    
                __delay_ms(10);
                // ------------------------------------ Print
                enable_uart_printf();
                dump_memory_map();
                printf("[RX-BUFFER] ADC_value from pH485_probe: \r\n"
                        "ph ADC_value_List: %u \r\n"
                        "pH Top_Point ADC:     %u \r\n"
                        "pH Bottom_Point ADC:  %u \r\n",
                        sensor_value.ph_ADC_value,
                        sensor_value.ph_top_point_adc_value,
                        sensor_value.ph_bottom_point_adc_value
                );
                // ------------------------------------ LED Flash
                printf("[Calibration] --- \r\n--You have 5 seconds to press the calibration button-- \r\n");
                LED_flashes_three_times();
                enable_tx_transmit();
                // ----
                state = STATE_CALIBRATE;
            break;
            
            case STATE_OUTPUT_VALUE:
                uart_rx_package_event = 0;
                rxbuffer_index = 0;
                LED_D2_ON();
                get_measured_value(&sensor_value); // memory big
                // ------------------------------------
                Enable_Global_Interrupt();
       
                i2c_slave_set_register(0x00, (uint8_t)(sensor_value.ph_value >> 8) & 0xFF);
                i2c_slave_set_register(0x01, (uint8_t)(sensor_value.ph_value >> 0) & 0xFF);
                i2c_slave_set_register(0x02, (uint8_t)(sensor_value.temp_value >> 8) & 0xFF);
                i2c_slave_set_register(0x03, (uint8_t)(sensor_value.temp_value >> 0) & 0xFF);
                Disable_Global_Interrupt();
                LED_D2_OFF();

                // ------------------------------------
                enable_uart_printf();
                dump_memory_map();
                printf("[RX-BUFFER] value from pH485_probe: \r\n"
                        "pH  : %u \r\n"
                        "TEMP: %u \r\n", 
                        sensor_value.ph_value,
                        sensor_value.temp_value
                        );
                printf("---------------------------- \r\n");
               
                enable_tx_transmit();
                
                state = STATE_MEASURE;
//                
                break;
            default:
                break;
        };

        
        now = get_ticks_with_lock();
        static uint32_t prev_ticks2 = 0;
        if(calculate_diff(now, prev_ticks2) >= 2000){
            prev_ticks2 = now;
            set_device_serial_number_to_i2c_slave_memorymap();
        };
        
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
 * 2024-04-15
 *      - organize code
 *      - add temp value
 *      - add calibration
 *      - add LED blink from calibration
 *      - add state machine
 *      
 *      
 */
