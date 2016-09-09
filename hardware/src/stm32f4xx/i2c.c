#include "i2c.h"

#include <hardware.h>

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_i2c.h>

static I2C_TypeDef * const i2cs[] = { (void *) 0, I2C1, I2C2, I2C3 };

void i2c_config(i2c_port_t i2c_port, uint32_t speed)
{
    i2c_t i2c = i2c_port.i2c;
    pin_t scl = i2c_port.scl;
    pin_t sda = i2c_port.sda;

    I2C_TypeDef *id = i2cs[i2c];
    uint8_t af;

    switch (i2c) {
    case i2c_1:
        af = GPIO_AF_I2C1;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        break;
    case i2c_2:
        af = GPIO_AF_I2C2;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
        break;
    case i2c_3:
        af = GPIO_AF_I2C3;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
        break;
    default:
        return;
    }

    uint32_t port_flags = 0;
    port_flags |= 1 << scl.port;
    port_flags |= 1 << sda.port;
    RCC_AHB1PeriphClockCmd(port_flags, ENABLE);


    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin   = 1 << scl.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[scl.port], scl.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[scl.port], &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = 1 << sda.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[sda.port], sda.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[sda.port], &GPIO_InitStructure);


    //Init I2C
    I2C_InitTypeDef i2cdef;
    i2cdef.I2C_Mode = I2C_Mode_I2C;
    i2cdef.I2C_DutyCycle = I2C_DutyCycle_2;
    i2cdef.I2C_OwnAddress1 = 0x00;
    i2cdef.I2C_Ack = I2C_Ack_Enable;
    i2cdef.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    i2cdef.I2C_ClockSpeed = speed;

    I2C_Init(id, &i2cdef); 
    I2C_Cmd(id, ENABLE);

/*
#if (defined(I2C1_USE_DMA_TX) || defined(I2C1_USE_DMA_RX) || defined(I2C2_USE_DMA_TX) || defined(I2C2_USE_DMA_RX))
 	i2c_dma_init(id);
#endif */
}
/*
void i2c_dma_init(uint8_t id)
{

}
*/
void i2c_start_read(i2c_t i2c, uint8_t destination)
{
    I2C_TypeDef * id = i2cs[i2c];

    I2C_GenerateSTART(id, ENABLE);
    while (!I2C_CheckEvent(id, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(id, destination, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(id, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

void i2c_start_write(i2c_t i2c, uint8_t destination)
{
    I2C_TypeDef * id = i2cs[i2c];

    I2C_GenerateSTART(id, ENABLE);
    while (!I2C_CheckEvent(id, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(id, destination, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(id, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void i2c_stop(i2c_t i2c)
{
    I2C_GenerateSTOP(i2cs[i2c], ENABLE);
}


void i2c_read_polling(i2c_t i2c, uint8_t *value, uint8_t nb)
{
    I2C_TypeDef * id = i2cs[i2c];

    while (nb--){
        if (nb == 0)
            I2C_AcknowledgeConfig(id, DISABLE);
      
        while (!I2C_CheckEvent(id, I2C_EVENT_MASTER_BYTE_RECEIVED));
        *value++ = I2C_ReceiveData(id);
    }
    
    I2C_AcknowledgeConfig(id, ENABLE);
}

void i2c_write_polling(i2c_t i2c, uint8_t *value, uint8_t nb)
{
    I2C_TypeDef * id = i2cs[i2c];

    while (nb--){
        I2C_SendData(id, *value++);
        while(!I2C_CheckEvent(id, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
}

/*
void i2c_read_dma(i2c_t i2c, uint8_t *value, uint8_t nb)
{

}

void i2c_write_dma(i2c_t i2c, uint8_t *value, uint8_t nb)
{

}*/

