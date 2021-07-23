/*
 * rpc_func_arduino.c
 *
 *  Created on: 15.11.2015
 *      Author: arne
 */

#include "../../../include/rpc_transmission/server/app/qt2mcu.h"
#include "board.h"
#include "globals.h"
#include "vc.h"

#include "Arduino.h"

rpc_analog_values_t get_analog_values(void){
    rpc_analog_values_t result;
    result.ain0 = analogRead(0);
    result.ain1 = analogRead(1);
    result.ain2 = analogRead(2);
    result.ain3 = analogRead(3);
    result.ain4 = analogRead(4);
    result.ain5 = analogRead(5);
    return result;
}

uint8_t get_digital_value(uint8_t pin_number){
    uint8_t result = digitalRead(pin_number) == HIGH;
    return result;
}

void set_digital_value(uint8_t pin_number, uint8_t value){
    if (value){
      digitalWrite(pin_number, HIGH);
    }else{
      digitalWrite(pin_number, LOW);
    }
}

void set_digital_direction(uint8_t pin_number, uint8_t output_direction){
    if (output_direction){
      pinMode(pin_number, OUTPUT);
    }else{
      pinMode(pin_number, INPUT);
    }
}

void activate_rpc_watchdog(uint32_t timeout, uint8_t pin_number, uint8_t pin_value){
    rpc_watchdog_data.timeout = timeout;
    rpc_watchdog_data.pin_number = pin_number;
    rpc_watchdog_data.pin_value = pin_value;
    trigger_watchdog_timer();
}

device_descriptor_v1_t get_device_descriptor(void) {
    device_descriptor_v1_t descriptor = {
        .githash = GITHASH,
        .gitDate_unix = GITUNIX,
        .deviceID = 0,
        .guid = {0},
        .boardRevision = 0,
        .name = "ArduLogger",
        .version = "-",
    };
    return descriptor;
}
