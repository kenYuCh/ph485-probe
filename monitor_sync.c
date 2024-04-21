#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "monitor_sync.h"
#include "mybutton.h"
#include "led.h"
#include "tmr0.h"
#include <xc.h>
#include "myisr.h"
#include "calibration.h"

void monitoring_sync(States *state, MyButton *b1, MyButton *b2, 
                     uint8_t *b1_clicked_count, uint8_t *b2_clicked_count) {
    
    if( is_single_click(b1)){
        printf("b1 clicked\r\n");
        *b1_clicked_count = (*b1_clicked_count) + 1;
        LED_flashes_three_times();
        reset_single_click_flag(b1);

    };
    
    if( is_single_click(b2)){
        printf("b2 clicked\r\n");
        *b2_clicked_count = (*b2_clicked_count) + 1;
        LED_flashes_three_times();
        reset_single_click_flag(b2);
//        *b2_clicked_count = 0;
    };
    
    if( is_long_press(b1)){
        printf("b1 long press \r\n");
        *b1_clicked_count = (*b1_clicked_count) + 1;
        //
        LED_flashes_three_times();
        LED_flashes_three_times();
        LED_flashes_three_times();
        reset_long_press_flag(b1);
        *state = STATE_CALIBRATE;
        *b1_clicked_count = 0;
    };
    if( is_long_press(b2)){
        printf("b2 long press \r\n");
        LED_flashes_three_times();
        LED_flashes_three_times();
        LED_flashes_three_times();
        reset_long_press_flag(b2);
        *state = STATE_MEASURE;
        *b1_clicked_count = 0;
     };
//     *state = STATE_MEASURE;
};



void dispatch(States *state,
              MyButton *b1,
              MyButton *b2,
              uint8_t *b1_clicked_count_ptr, 
              uint8_t *b2_clicked_count_ptr)
{
    uint8_t b1_clicked_count = *b1_clicked_count_ptr;
    uint8_t b2_clicked_count = *b2_clicked_count_ptr;
    

//    if( 0 == b1_clicked_count && 0 == b2_clicked_count){
//        *state = STATE_CALIBRATE;
//        return;
//    }  
    
    if( 1 == b1_clicked_count && 0 == b2_clicked_count){
        *state = STATE_SAVE_TOP_CALIBRATION_POINT;
        *b1_clicked_count_ptr = 0; // use 
        return;
    }

    if( 0 == b1_clicked_count && 1 == b2_clicked_count){
        *state = STATE_SAVE_BOTTOM_CALIBRATION_POINT;
        *b2_clicked_count_ptr = 0; // use
        return;
    }
//    *state = STATE_CALIBRATE;
}




void delay_state(States *state) {
    static uint32_t prev_ticks_delay = 0;
    if(calculate_diff(get_ticks(), prev_ticks_delay) >= 1000){
        prev_ticks_delay = get_ticks();
        *state = STATE_MEASURE;
    };
};