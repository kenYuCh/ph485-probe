/* 
 * File:   myisr.h
 * Author: yuhsienchang
 *
 * Created on April 19, 2024, 10:33 PM
 */

#ifndef MYISR_H
#define	MYISR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>

volatile uint32_t ticks = 0;

uint32_t get_ticks();
uint32_t get_ticks_with_lock();
uint32_t calculate_diff(uint32_t now, uint32_t prev_ticks);

void Enable_Global_Interrupt();
void Disable_Global_Interrupt();

#ifdef	__cplusplus
}
#endif

#endif	/* MYISR_H */

