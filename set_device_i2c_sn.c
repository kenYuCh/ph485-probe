#include <stdlib.h>
#include <stdint.h>
#include "configuration.h"
#include "set_device_i2c_sn.h"
#include "i2c_slave.h"



void set_device_serial_number_to_i2c_slave_memorymap(){
    for (uint8_t i = 0; i < 5; i++) {
        char buffer[3];
        buffer[0] = DEVICE_SERIAL[i * 2];
        buffer[1] = DEVICE_SERIAL[i * 2 + 1];
        buffer[2] = '\0';
        //printf("%s\r\n", buffer);
        uint8_t value = (uint8_t)strtol(buffer, NULL, 16);
        i2c_slave_set_register_with_lock(57u + i, value);
    }    
}