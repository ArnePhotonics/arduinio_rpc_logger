

#include "Arduino.h"

#include "main.h"
#include "globals.h"
#include "vc.h"
#include "errorlogger/generic_eeprom_errorlogger.h"
#include "channel_codec/channel_codec.h"
#include "rpc_transmission/client/generated_app/RPC_TRANSMISSION_mcu2qt.h"

#define LEDPIN 13 // LEDPIN is a constant

#define CHANNEL_CODEC_TX_BUFFER_SIZE 64
#define CHANNEL_CODEC_RX_BUFFER_SIZE 64

channel_codec_instance_t cc_instances[channel_codec_comport_COUNT];

static char cc_rxBuffers[channel_codec_comport_COUNT][CHANNEL_CODEC_RX_BUFFER_SIZE];
static char cc_txBuffers[channel_codec_comport_COUNT][CHANNEL_CODEC_TX_BUFFER_SIZE];
volatile rpc_watchdog_t rpc_watchdog_data;
volatile static uint32_t rpc_watchdog_timer;



bool xSerialCharAvailable() {
    if (Serial.available()) {
        return true;
    } else {
        return false;
    }
}

bool xSerialGetChar(char *data) {
    if (Serial.available()) {
        *data = Serial.read();
        return true;
    } else {
        return false;
    }
}

#ifdef __cplusplus
extern "C" {
#endif

bool is_watchdog_triggered(){
    return rpc_watchdog_data.is_triggered;
}

void toggleLED() {
    boolean ledstate = digitalRead(LEDPIN); // get LED state
    ledstate ^= 1;                          // toggle LED state using xor
    digitalWrite(LEDPIN, ledstate);         // write inversed state back
}

void trigger_watchdog_timer(void) {
    rpc_watchdog_timer = rpc_watchdog_data.timeout;
 //  rpc_watchdog_timer = 100000; //ca 2sec
}

void xSerialToRPC(void) {
    while (xSerialCharAvailable()) {
        // read the incoming byte:
        char inByte = 0;
        xSerialGetChar(&inByte);
        trigger_watchdog_timer();
        channel_push_byte_to_RPC(&cc_instances[channel_codec_comport_transmission], inByte);
    }
    if (rpc_watchdog_timer) {
        rpc_watchdog_timer = rpc_watchdog_timer-1;
        toggleLED();
        if (rpc_watchdog_timer == 0) {
            uint16_t pin_mask = rpc_watchdog_data.pin_mask;
            for (int i=0; i< 13;i++){
                uint8_t val = pin_mask & 1;
                pin_mask >>= 1;
                rpc_watchdog_data.is_triggered = true;
                digitalWrite(i, val); // timeout happened. so lets set the watchdog pin as wanted
            }
        }
    }
}

void xSerialPutChar(uint8_t data) {
    Serial.write(data);
}

void SET_LED(int ledstatus) {
    digitalWrite(LEDPIN, ledstatus); // write inversed state back
    if (ledstatus == 30) {
        Serial.print("Answer\n");
    }
}

void delay_ms(uint32_t sleep_ms) {
    delay(sleep_ms);
}

#ifdef __cplusplus
}
#endif

void ChannelCodec_errorHandler(channel_codec_instance_t *instance, channelCodecErrorNum_t ErrNum) {
    (void)ErrNum;
    (void)instance;
}

void setup() {
    // start serial port at 9600 bps:
    Serial.begin(115200);
    RPC_TRANSMISSION_mutex_init();

    cc_instances[channel_codec_comport_transmission].aux.port = channel_codec_comport_transmission;

    channel_init_instance(&cc_instances[channel_codec_comport_transmission], cc_rxBuffers[channel_codec_comport_transmission],
                          CHANNEL_CODEC_RX_BUFFER_SIZE, cc_txBuffers[channel_codec_comport_transmission], CHANNEL_CODEC_TX_BUFFER_SIZE);

    rpc_watchdog_data.timeout = 0;
#if 1
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
#endif
    rpc_watchdog_data.is_triggered = false;

    pinMode(LEDPIN, OUTPUT); // LED init
    digitalWrite(LEDPIN, 1); // write inversed state back
}

void loop() {
    while (1) {
        xSerialToRPC();
    }
}
