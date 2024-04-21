
#include "mcc_generated_files/mcc.h"
#include <stdint.h>
#include "myisr.h"



uint32_t get_ticks() {
    return ticks;
}

uint32_t get_ticks_with_lock() {
    GIE = 0;
    uint32_t result = ticks;
    GIE = 1;
    return result;
}

uint32_t calculate_diff(uint32_t now, uint32_t prev_ticks) {
    uint32_t diff;
    if (now >= prev_ticks) {
        diff = now - prev_ticks;
    } else {
        // Handling overflow situation
        diff = (UINT32_MAX - prev_ticks) + now + 1;
    }
    return diff;
}


void Enable_Global_Interrupt() {
    GIE=0;
};
void Disable_Global_Interrupt(){
    GIE=1;
};
    
