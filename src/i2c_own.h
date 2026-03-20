#include <ch32v00x.h>
#include <debug.h>

void own_I2C_Init();
void I2C_SendByte(uint8_t address, uint8_t data);
void I2C_SendBytes(uint8_t address, uint8_t *data, size_t length);
