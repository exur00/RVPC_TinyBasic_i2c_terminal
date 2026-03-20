#include  "i2c_own.h"

void own_I2C_Init() {
    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitTStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    // SCL pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // set alternate function, open drain
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    // SDA pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    I2C_InitTStructure.I2C_ClockSpeed = 100000;
    I2C_InitTStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitTStructure );

    I2C_Cmd( I2C1, ENABLE );

    I2C_AcknowledgeConfig( I2C1, ENABLE );
}


void I2C_SendByte(uint8_t address, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);

    I2C_GenerateSTART(I2C1, ENABLE);

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Transmitter);

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);

    I2C_SendData(I2C1, data);

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_SendBytes(uint8_t address, uint8_t *data, size_t length)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);

    I2C_GenerateSTART(I2C1, ENABLE);

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Transmitter);

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    for (size_t i = 0; i < length; i++) {

        while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);

        I2C_SendData(I2C1, data[i]);

    }

    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}
