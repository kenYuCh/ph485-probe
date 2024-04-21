/* 
 * File:   monitor_sync.h
 * Author: yuhsienchang
 *
 * Created on April 20, 2024, 12:09 AM
 */

#ifndef MONITOR_SYNC_H
#define	MONITOR_SYNC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "monitor_sync.h"
#include "mybutton.h"
#include <stdlib.h>
    
typedef enum {
    STATE_MONITORING_POWER_FOR_SYNC,
    STATE_DELAY,
    STATE_MEASURE,
    STATE_DISPATCH,
    STATE_CALIBRATE,
    STATE_OUTPUT_ADC,
    STATE_OUTPUT_VALUE,
    STATE_SAVE_TOP_CALIBRATION_POINT,
    STATE_SAVE_BOTTOM_CALIBRATION_POINT,
    STATE_SAVE_3RD_CALIBRATION_POINT
} States;

void monitoring_sync(States *state, MyButton *b1, MyButton *b2, 
                     uint8_t *b1_clicked_count, uint8_t *b2_clicked_count);

void dispatch(States *state,
              MyButton *b1,
              MyButton *b2,
              uint8_t *b1_clicked_count_ptr, 
              uint8_t *b2_clicked_count_ptr);

void delay_state(States *state) ;
#ifdef	__cplusplus
}
#endif

#endif	/* MONITOR_SYNC_H */

