#include "i2c.h"

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_i2c.h>

#include <FreeRTOS.h>
#include <task.h>


struct i2cInitDef_t {
    // scl / sda
    I2C_TypeDef * id;
    uint8_t afid;
    GPIO_TypeDef * tdef[2];
    GPIO_InitTypeDef gpiodef[2];
    uint16_t gpiopinsource[2];
    // i2c / gpio
    volatile uint32_t * bridge[2];
    uint32_t peripheral[2];
};

static struct i2cInitDef_t i2cInitDefs[2] = {
    { I2C1, GPIO_AF_I2C1, { GPIOB, GPIOB }, {   // I2C1
        { GPIO_Pin_6, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_UP },        // SCL
        { GPIO_Pin_7, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_UP },        // SDA
    }, {GPIO_PinSource6, GPIO_PinSource7}, {&RCC->APB1ENR, &RCC->AHB1ENR}, {RCC_APB1Periph_I2C1, RCC_AHB1Periph_GPIOB } },
    { I2C2, GPIO_AF_I2C2, { GPIOB, GPIOB }, {   // I2C2
        { GPIO_Pin_10, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_UP },       // SCL
        { GPIO_Pin_11, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_UP },       // SDA
    }, {GPIO_PinSource10, GPIO_PinSource11}, {&RCC->APB1ENR, &RCC->AHB1ENR}, {RCC_APB1Periph_I2C2, RCC_AHB1Periph_GPIOB } },
};


void i2c_init(uint8_t id, uint32_t speed)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct i2cInitDef_t * i2cInitDef = i2cInitDefs + id - 1;

    int i;
    for (i = 0; i < 2; i++)
        *(i2cInitDef->bridge[i]) |= i2cInitDef->peripheral[i];

    for (i = 0; i < 2; i++)
        GPIO_Init(i2cInitDef->tdef[i], &i2cInitDef->gpiodef[i]);

    for (i = 0; i < 2; i++)
        GPIO_PinAFConfig(i2cInitDef->tdef[i], i2cInitDef->gpiopinsource[i], i2cInitDef->afid);

    //Init I2C
    I2C_InitTypeDef i2cdef;
    i2cdef.I2C_Mode = I2C_Mode_I2C;
    i2cdef.I2C_DutyCycle = I2C_DutyCycle_2;
    i2cdef.I2C_OwnAddress1 = 0x00;
    i2cdef.I2C_Ack = I2C_Ack_Enable;
    i2cdef.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2cdef.I2C_ClockSpeed = speed;

    I2C_Init(i2cInitDef->id, &i2cdef); 
    I2C_Cmd(i2cInitDef->id, ENABLE);

#if (defined(I2C1_USE_DMA_TX) || defined(I2C1_USE_DMA_RX) || defined(I2C2_USE_DMA_TX) || defined(I2C2_USE_DMA_RX))
 	i2c_dma_init(id);
#endif 
}

void i2c_dma_init(uint8_t id)
{

}

void i2c_start_read(uint8_t id, uint8_t destination)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct i2cInitDef_t * i2cInitDef = i2cInitDefs + id - 1;

	I2C_GenerateSTART(i2cInitDef->id, ENABLE);
    while (!I2C_CheckEvent(i2cInitDef->id, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(i2cInitDef->id, destination, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(i2cInitDef->id, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

void i2c_start_write(uint8_t id, uint8_t destination)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct i2cInitDef_t * i2cInitDef = i2cInitDefs + id - 1;

    I2C_GenerateSTART(i2cInitDef->id, ENABLE);
    while (!I2C_CheckEvent(i2cInitDef->id, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(i2cInitDef->id, destination, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(i2cInitDef->id, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void i2c_stop(uint8_t id)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct i2cInitDef_t * i2cInitDef = i2cInitDefs + id - 1;

    I2C_GenerateSTOP(i2cInitDef->id, ENABLE);
}


void i2c_read_polling(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct i2cInitDef_t * i2cInitDef = i2cInitDefs + id - 1;
 
    taskENTER_CRITICAL();

    while (nb--)
    {
        if (nb == 0)
            I2C_AcknowledgeConfig(i2cInitDef->id, DISABLE);
      
      while (!I2C_CheckEvent(i2cInitDef->id, I2C_EVENT_MASTER_BYTE_RECEIVED));
      *buffer++ = I2C_ReceiveData(i2cInitDef->id);
    }
    
    I2C_AcknowledgeConfig(i2cInitDef->id, ENABLE);

    taskEXIT_CRITICAL();
}

void i2c_write_polling(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct i2cInitDef_t * i2cInitDef = i2cInitDefs + id - 1;

    taskENTER_CRITICAL();

    while (nb--)
    {
        I2C_SendData(i2cInitDef->id, *buffer++);
        while(!I2C_CheckEvent(i2cInitDef->id, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    taskEXIT_CRITICAL();
}

void i2c_read_dma(uint8_t id, uint8_t *buffer, uint8_t nb)
{

}

void i2c_write_dma(uint8_t id, uint8_t *buffer, uint8_t nb)
{

}

