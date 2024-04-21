
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include "mybutton.h"
#include "myisr.h"

void init_button(MyButton *b) {
    
    b->signals[0] = 1;
    b->signals[1] = 1;

    b->falling_ts[0] = 1;
    b->falling_ts[1] = 1;
    b->rising_ts[0] = 1;
    b->rising_ts[1] = 1;
    
    b->single_click_tmps[0] = 0;
    b->single_click_tmps[1] = 0;
    b->double_click_tmps[0] = 0;
    b->double_click_tmps[1] = 0;
    b->long_press_tmps[0] = 0;
    b->long_press_tmps[1] = 0;
    
    b->single_clicked_flag = 0;
    b->double_clicked_flag = 0;
    b->long_press_flag = 0;
};




bool __is_single_click(MyButton *b, uint32_t now)
{
    bool condition1 = (b->rising_ts[1] > b->falling_ts[1]); // last edge is rising
    bool condition2 = (now - b->rising_ts[1] > 200); // timeout
    bool condition3 = (b->rising_ts[1] - b->rising_ts[0] > 500); // 2 rising edge out of window
    bool condition4 = (now - b->falling_ts[1] < 4000);
    
    if (condition1 && condition2 && condition3 && condition4){
        b->single_click_tmps[0] = b->single_click_tmps[1];
        b->single_click_tmps[1] = 1;
    }else{
        b->single_click_tmps[0] = b->single_click_tmps[1];
        b->single_click_tmps[1] = 0;
    }
    
    if(0 == b->single_click_tmps[0] && 1 == b->single_click_tmps[1]){
        return true;
    }
    return false;
}


bool __is_double_click(MyButton *b, uint32_t now)
{
    bool condition1 = (b->rising_ts[1] > b->falling_ts[1]); // last edge is rising
    bool condition2 = (now - b->rising_ts[1] > 200); // timeout
    bool condition3 = (b->rising_ts[1] - b->rising_ts[0] < 500); // 2 rising edge out of window    
    
    if (condition1 && condition2 && condition3){
        b->double_click_tmps[0] = b->double_click_tmps[1];
        b->double_click_tmps[1] = 1;
    }else{
        b->double_click_tmps[0] = b->double_click_tmps[1];
        b->double_click_tmps[1] = 0;
    }
    
    if(0 == b->double_click_tmps[0] && 1 == b->double_click_tmps[1]){
        return true;
    }
    return false;
}


bool __is_long_press(MyButton *b, uint32_t now){
    bool condition1 = (b->falling_ts[1] > b->rising_ts[1]); // last edge is press
    bool condition2 = (now - b->falling_ts[1] > 4000); // long press
    
    if(condition1 && condition2){
        b->long_press_tmps[0] = b->long_press_tmps[1];
        b->long_press_tmps[1] = 1;
    }else{
        b->long_press_tmps[0] = b->long_press_tmps[1];
        b->long_press_tmps[1] = 0;
    }
    
    if(0 == b->long_press_tmps[0] && 1 == b->long_press_tmps[1]){
        return true;
    }
    return false;
}



void update_mybutton(MyButton* b, bool signal, uint32_t now)
{
    b->signals[0] = b->signals[1];
    b->signals[1] = signal;
    
    if( 1 == b->signals[0] && 0 == b->signals[1] ){
        b->falling_ts[0] = b->falling_ts[1];
        b->falling_ts[1] = now;
    }

    if( 0 == b->signals[0] && 1 == b->signals[1] ){
        b->rising_ts[0] = b->rising_ts[1];
        b->rising_ts[1] = now;
    }
    
    if( __is_single_click(b, now) ){
        b->single_clicked_flag = 1;
    }
    
    if( __is_double_click(b, now) ){
        b->double_clicked_flag = 1;
    }
    
    if( __is_long_press(b, now) ){
        b->long_press_flag = 1;
    }
    
}

bool is_single_click(MyButton *b){
    return b->single_clicked_flag;
}

bool is_double_click(MyButton *b){
    return b->double_clicked_flag;
}

bool is_long_press(MyButton *b){
    return b->long_press_flag;
}

void reset_single_click_flag(MyButton *b){
    b->single_clicked_flag = 0;
}

void reset_double_click_flag(MyButton *b){
    b->double_clicked_flag = 0;
}

void reset_long_press_flag(MyButton *b){
    b->long_press_flag = 0;
}



bool is_it_time_up(uint32_t *prev_tick, uint32_t ms){
    uint32_t now = get_ticks();
    int32_t diff = (int32_t)(now - (*prev_tick));
    if ( diff >= ms){
        *prev_tick = now;
        return true;
    };
    return false;
}

bool is_it_time_over(uint32_t ms) {
    uint32_t now = get_ticks_with_lock();
    if (now >= ms){
        return true;
    }
    return false;
}


