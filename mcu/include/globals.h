#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <stdint.h>


#include "channel_codec/channel_codec_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


void SET_LED(int ledstatus);
void delay_ms(uint32_t sleep_ms);

#ifdef __cplusplus
}
#endif

typedef struct{
	uint8_t serialno[5];
} t_glob;

t_glob glob;

extern uint8_t TARGETEMP_C;

extern channel_codec_instance_t cc_instances[channel_codec_comport_COUNT];

#define TRUE 1
#define FALSE 0

#endif /*GLOBALS_H_*/
