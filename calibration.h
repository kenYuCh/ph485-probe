/* 
 * File:   calibration.h
 * Author: yuhsienchang
 *
 * Created on April 20, 2024, 2:17 PM
 */

#ifndef CALIBRATION_H
#define	CALIBRATION_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "measurement.h"
#include <stdint.h>
//    
void calibrate(States *states, SensorValue *sensor);
void save_top_calibration(States *states, SensorValue *sensor);
void save_bottom_calibration(States *states, SensorValue *sensor);

void set_top_calibrate_point();
void set_bottom_calibrate_point();
void measured_adc();
void get_measured_adc_value(SensorValue *sensor_value);

void save_top_adc_point(States *states, SensorValue *sensor_value);
void save_bottom_adc_point(States *states, SensorValue *sensor_value);

uint16_t crc16_modbus(uint8_t *data, uint8_t len);
void calculate_crc_and_fill_array(uint8_t *data, uint8_t data_len);


#ifdef	__cplusplus
}
#endif

#endif	/* CALIBRATION_H */

