/* 
 * File:   mybutton.h
 * Author: yuhsienchang
 *
 * Created on April 19, 2024, 10:34 PM
 */

#ifndef MYBUTTON_H
#define	MYBUTTON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mybutton.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct mybutton{
    uint8_t signals[2];
    uint32_t falling_ts[2];
    uint32_t rising_ts[2];

    uint8_t single_click_tmps[2];
    uint8_t double_click_tmps[2];
    uint8_t long_press_tmps[2];

    uint8_t single_clicked_flag;
    uint8_t double_clicked_flag;
    uint8_t long_press_flag;
}MyButton;

void init_button(MyButton *b);
void update_mybutton(MyButton *b, bool signal, uint32_t now);
//bool is_single_click(MyButton *b, uint32_t now);
//bool is_double_click(MyButton *b, uint32_t now);
bool is_single_click(MyButton *b);
bool is_double_click(MyButton *b);
bool is_long_press(MyButton *b);

void reset_single_click_flag(MyButton *b);
void reset_double_click_flag(MyButton *b);
void reset_long_press_flag(MyButton *b);


/*void mybutton_task();

bool is_mybutton_clicked();
void mybutton_reset_clicked_flag();

bool is_mybutton_double_clicked();
void mybutton_reset_double_clicked_flag();
*/



bool is_it_time_up(uint32_t *prev_tick, uint32_t ms);
bool is_it_time_over(uint32_t ms);
#ifdef	__cplusplus
}
#endif

#endif	/* MYBUTTON_H */

