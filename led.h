/* 
 * File:   led.h
 * Author: yuhsienchang
 *
 * Created on April 19, 2024, 10:34 PM
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
    
#define LED_D2_ON()           IO_RC2_SetLow()
#define LED_D2_OFF()          IO_RC2_SetHigh()
#define LED_D2_TOGGLE()       IO_RC2_Toggle()
    
#define LED_PIN_D2 LATCbits.LATC2
//#define LED_D2_FlASH_THREE_TIMES_t() LED_flashes_three_times()
//  LATCbits.LATC2  -> LED RC2

typedef enum {
    LED_ON,
    LED_OFF,   
} LED_State;

void LED_flashes_three_times();
void LED_flashes_one_times();
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

