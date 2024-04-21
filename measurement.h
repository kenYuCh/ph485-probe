/* 
 * File:   measurement.h
 * Author: yuhsienchang
 *
 * Created on April 20, 2024, 12:53 AM
 */

#ifndef MEASUREMENT_H
#define	MEASUREMENT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "monitor_sync.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
    
typedef struct values {
    uint8_t ph_value;
    uint16_t ph_ADC_value;
    uint8_t ph_ADC_High_byte;
    uint8_t ph_ADC_Low_byte;
    uint16_t ph_ADC_value_List[10];
            
    uint16_t ph_top_point_adc_value;
    uint8_t ph_top_point_adc_high_byte;
    uint8_t ph_top_point_adc_low_byte;
    
    uint16_t ph_bottom_point_adc_value;
    uint8_t ph_bottom_point_adc_high_byte;
    uint8_t ph_bottom_point_adc_low_byte;
    
    uint16_t temp_value; 
    uint16_t temp_adc_value; 
} SensorValue;

void measurement(States *state, SensorValue *value);
void send_measured_command();
void get_measured_value(SensorValue *sensor_value);
#ifdef	__cplusplus
}
#endif

#endif	/* MEASUREMENT_H */

