#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#pragma RPC prefix RPC_TRANSMISSION_
	

typedef struct{
  uint16_t ain0;
  uint16_t ain1;
  uint16_t ain2;
  uint16_t ain3;
  uint16_t ain4;
  uint16_t ain5;
} rpc_analog_values_t;


typedef struct{
	uint32_t githash;
	uint32_t gitDate_unix;

	uint32_t serialnumber;
	uint16_t deviceID;
	uint8_t guid[12];
	uint8_t boardRevision;

	char name[12];
	char version[8];
} device_descriptor_v1_t;



device_descriptor_v1_t get_device_descriptor(void);
void set_digital_direction(uint8_t pin_number, uint8_t output_direction);
void set_digital_value(uint8_t pin_number, uint8_t value);
uint8_t get_digital_value(uint8_t pin_number);
rpc_analog_values_t get_analog_values(void);
void activate_rpc_watchdog(uint32_t timeout, uint8_t pin_number, uint8_t pin_value);

#ifdef __cplusplus
}
#endif /* __cplusplus */
