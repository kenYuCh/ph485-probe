#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "led.h"


void LED_flashes_three_times() {
    for (int i = 0; i < 3; i++) {
        // open led
        LED_D2_ON();
        __delay_ms(50); // 
        // led off
        LED_D2_OFF();
        __delay_ms(50);
        
    }
};
void LED_flashes_one_times() {
    for (int i = 0; i < 1; i++) {
        // open led
        LED_D2_ON();
        __delay_ms(30); // 
        // led off
        LED_D2_OFF();
        __delay_ms(30);
    }
};
