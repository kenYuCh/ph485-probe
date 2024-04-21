
//#include "calibration.h"
#include "monitor_sync.h"
#include "myisr.h"
#include "stdio.h"
#include "measurement.h"
#include <stdio.h>
#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include "uart.h"
#include "led.h"
#include <string.h>
//
void calibrate(States *states, SensorValue *sensor) {

};

void save_top_calibration(States *states, SensorValue *sensor) {

};

void save_bottom_calibration(States *states, SensorValue *sensor) {

};


uint16_t crc16_modbus(uint8_t *data, uint8_t len){
    uint16_t crc = 0xFFFF;
    uint16_t poly = 0xA001;
    
    for (int i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= poly;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
};
void calculate_crc_and_fill_array(uint8_t *data, uint8_t data_len) {
    uint16_t crc = crc16_modbus(data, data_len);
    data[data_len] = crc & 0xFF; // low char
    data[data_len+1] = (crc >> 8) & 0xFF; // high char
};
//
void set_top_calibrate_point() {
//    printf("------ set_top_calibrate_point -------");
    char top_zero_point_command[32] = {0x01, 0x06, 0x00, 0x36, 0x01, 0xA8, 0x04};
    EUSART1_Write_1(top_zero_point_command, 8);
};
void set_bottom_calibrate_point() {
//    printf("------ set_bottom_calibrate_point -------");
    char bottom_slop_point_command[32] = {0x01, 0x06, 0x00, 0x38, 0x01, 0xC9, 0xC7}; // write 01 06, read 01 03 
    EUSART1_Write_1(bottom_slop_point_command, 8);
};


void measured_adc() {
    enable_uart_printf();
    printf("[Request] Send ADC_data request to pH485_probe - CALIBRATION MODE ---  \r\n");
    __delay_ms(10);
    enable_tx_transmit();
    __delay_ms(10);
    // ------------------------------------
    // request ph
    char measured_adc_command[8] = {0x01, 0x03, 0x00, 0x66, 0x00, 0x01, 0x64, 0x15};
    LED_D2_ON();
    EUSART1_Write_1(measured_adc_command, 8);
    LED_D2_OFF();

};

uint8_t test_recived_data[8] ={0x01, 0x03, 0x02, 0x2E, 0xE0, 0xA4, 0x6C};
void get_measured_adc_value(SensorValue *sensor_value) {
    enable_uart_printf();
    printf("[Receive] ADC_data from pH485_probe \r\n");
    __delay_ms(10);
    enable_tx_transmit();
    uint16_t ph_adc_value = 0;
//    sensor_value->ph_adc_high_byte = rxbuffer[3];
//    sensor_value->ph_adc_low_byte = rxbuffer[4];
//    uint32_t payload = ((uint32_t)rxbuffer[3]) << 8 | 
//                       ((uint32_t)rxbuffer[4] << 0);
//    memcpy(&ph_adc_value, &payload, sizeof(uint16_t));
//    sensor_value->ph_adc_value = ph_adc_value;
    sensor_value->ph_ADC_High_byte = test_recived_data[3];
    sensor_value->ph_ADC_Low_byte = test_recived_data[4];
    sensor_value->ph_ADC_value = (test_recived_data[3] << 8)  | test_recived_data[4];

};


void save_top_adc_point(States *states, SensorValue *sensor_value) {
    enable_uart_printf();
    printf("STATE_SAVE_TOP_CALIBRATION_POINT Saved \r\n");
    __delay_ms(10);
    enable_tx_transmit();
    //
    uint8_t save_top_adc_command[8] = {0x01, 0x06, 0x00, 0x3E, 0x00, 0xFF, 0xA8, 0x46};
    save_top_adc_command[4] = sensor_value->ph_ADC_High_byte;
    save_top_adc_command[5] = sensor_value->ph_ADC_Low_byte;
    sensor_value->ph_top_point_adc_value = sensor_value->ph_ADC_value;
    
    calculate_crc_and_fill_array(save_top_adc_command, 6);
    
    EUSART1_Write_1(save_top_adc_command, 8);
    __delay_ms(10);
    
    enable_uart_printf();
    __delay_ms(10);
    printf("[RX-BUFFER] pH485-Calibration_Top_Value: ---- \r\n "
            "pH ADC_value: %u \r\n"
            "pH ADC_High Bytes: 0x%02X \r\n"
            "pH ADC_Low  Bytes: 0x%02X \r\n"
            "pH ADC_Low  CRC High Byte: 0x%02X \r\n"
            "pH ADC_Low  CRC High Byte: 0x%02X \r\n",
            sensor_value->ph_ADC_value,
            save_top_adc_command[4],
            save_top_adc_command[5],
            save_top_adc_command[6],
            save_top_adc_command[7]
            );
    __delay_ms(10);
    enable_tx_transmit();
    *states = STATE_CALIBRATE;
};
void save_bottom_adc_point(States *states, SensorValue *sensor_value) {
    enable_uart_printf();
    printf("STATE_SAVE_BOTTOM_CALIBRATION_POINT Saved \r\n");
    __delay_ms(10);
    enable_tx_transmit();
    //
    uint8_t save_bottom_adc_command[8] = {0x01, 0x06, 0x00, 0x3F, 0x00, 0xFF, 0xF9, 0x86};
    save_bottom_adc_command[4] = sensor_value->ph_ADC_High_byte;
    save_bottom_adc_command[5] = sensor_value->ph_ADC_Low_byte;
    sensor_value->ph_bottom_point_adc_value = sensor_value->ph_ADC_value;
    calculate_crc_and_fill_array(save_bottom_adc_command, 6); // 
    
    EUSART1_Write_1(save_bottom_adc_command, 8);
    __delay_ms(10);
    
    enable_uart_printf();
    __delay_ms(10);
    printf("[RX-BUFFER] pH485-Calibration_Bottom_Value: ---- \r\n "
            "pH ADC_value: %u \r\n"
            "pH ADC_High Bytes: 0x%02X \r\n"
            "pH ADC_Low  Bytes: 0x%02X \r\n"
            "pH ADC_Low  CRC High Byte: 0x%02X \r\n"
            "pH ADC_Low  CRC High Byte: 0x%02X \r\n",
            sensor_value->ph_ADC_value,
            save_bottom_adc_command[4],
            save_bottom_adc_command[5],
            save_bottom_adc_command[6],
            save_bottom_adc_command[7]
            );
    __delay_ms(10);
    enable_tx_transmit();
    *states = STATE_CALIBRATE;
};

/*
 * top
 char top_zero_point_command[32] = {0x01, 0x06, 0x00, 0x36, 0x01, 0xA8, 0x04};
 char bottom_slop_point_command[32] = {0x01, 0x06, 0x00, 0x38, 0x01, 0xC9, 0xC7};
 
 * // ? adc ??? ???? 0x3E??
 char measured_adc_command[32] = {0x01, 0x03, 0x00, 0x66, 0x00, 0x01, 0x64, 0x15}; //read adc value
 * 
 char save_top_adc_command[32] = {0x01, 0x06, 0x00, 0x3E, 0x00, 0xFF, 0xA8, 0x46};
 char save_bottom_adc_command[32] = {0x01, 0x06, 0x00, 0x3F, 0x00, 0xFF, 0xF9, 0x86};
 *
 * 
 * ????? ??????? set top/bottom()?
 * ???????ADC??
 * 
 * ???????????TOP???
 * ??TOP??????SAVE_TOP_ADC_POINT()
 * 
 * ???BUTTON?????BUTTON???
 * ??BUTTON??????SAVE_BUTTON_ADC_POINT()
 * 
 */