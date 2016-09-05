#include <stm32f10x.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_dma.h>

#include <stddef.h>

#include "adc.h"

#include "hardware.h"

static ADC_TypeDef *adclist[] = {ADC1, ADC2, ADC3};
/*
// FROM DRIVER
//ADC_Mode
#define ADC_Mode_Independent                       ((uint32_t)0x00000000)
#define ADC_Mode_RegInjecSimult                    ((uint32_t)0x00010000)
#define ADC_Mode_RegSimult_AlterTrig               ((uint32_t)0x00020000)
#define ADC_Mode_InjecSimult_FastInterl            ((uint32_t)0x00030000)
#define ADC_Mode_InjecSimult_SlowInterl            ((uint32_t)0x00040000)
#define ADC_Mode_InjecSimult                       ((uint32_t)0x00050000)
#define ADC_Mode_RegSimult                         ((uint32_t)0x00060000)
#define ADC_Mode_FastInterl                        ((uint32_t)0x00070000)
#define ADC_Mode_SlowInterl                        ((uint32_t)0x00080000)
#define ADC_Mode_AlterTrig                         ((uint32_t)0x00090000)

//ADC_ExternalTrigConv
#define ADC_ExternalTrigConv_T1_CC1                ((uint32_t)0x00000000) //< For ADC1 and ADC2
#define ADC_ExternalTrigConv_T1_CC2                ((uint32_t)0x00020000) //< For ADC1 and ADC2
#define ADC_ExternalTrigConv_T2_CC2                ((uint32_t)0x00060000) //< For ADC1 and ADC2
#define ADC_ExternalTrigConv_T3_TRGO               ((uint32_t)0x00080000) //< For ADC1 and ADC2
#define ADC_ExternalTrigConv_T4_CC4                ((uint32_t)0x000A0000) //< For ADC1 and ADC2
#define ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO    ((uint32_t)0x000C0000) //< For ADC1 and ADC2
#define ADC_ExternalTrigConv_T1_CC3                ((uint32_t)0x00040000) //< For ADC1, ADC2 and ADC3
#define ADC_ExternalTrigConv_None                  ((uint32_t)0x000E0000) //< For ADC1, ADC2 and ADC3
#define ADC_ExternalTrigConv_T3_CC1                ((uint32_t)0x00000000) //< For ADC3 only
#define ADC_ExternalTrigConv_T2_CC3                ((uint32_t)0x00020000) //< For ADC3 only
#define ADC_ExternalTrigConv_T8_CC1                ((uint32_t)0x00060000) //< For ADC3 only
#define ADC_ExternalTrigConv_T8_TRGO               ((uint32_t)0x00080000) //< For ADC3 only
#define ADC_ExternalTrigConv_T5_CC1                ((uint32_t)0x000A0000) //< For ADC3 only
#define ADC_ExternalTrigConv_T5_CC3                ((uint32_t)0x000C0000) //< For ADC3 only

//ADC_DataAlign
#define ADC_DataAlign_Right                        ((uint32_t)0x00000000)
#define ADC_DataAlign_Left                         ((uint32_t)0x00000800)

#define ADC_Channel_0                               ((uint8_t)0x00)
#define ADC_Channel_1                               ((uint8_t)0x01)
#define ADC_Channel_2                               ((uint8_t)0x02)
#define ADC_Channel_3                               ((uint8_t)0x03)
#define ADC_Channel_4                               ((uint8_t)0x04)
#define ADC_Channel_5                               ((uint8_t)0x05)
#define ADC_Channel_6                               ((uint8_t)0x06)
#define ADC_Channel_7                               ((uint8_t)0x07)
#define ADC_Channel_8                               ((uint8_t)0x08)
#define ADC_Channel_9                               ((uint8_t)0x09)
#define ADC_Channel_10                              ((uint8_t)0x0A)
#define ADC_Channel_11                              ((uint8_t)0x0B)
#define ADC_Channel_12                              ((uint8_t)0x0C)
#define ADC_Channel_13                              ((uint8_t)0x0D)
#define ADC_Channel_14                              ((uint8_t)0x0E)
#define ADC_Channel_15                              ((uint8_t)0x0F)
#define ADC_Channel_16                              ((uint8_t)0x10)
#define ADC_Channel_17                              ((uint8_t)0x11)

*/

void adc_calibrate(uint8_t adc)
{
    if (adc < 1 || adc > 3)
        return;

    ADC_ResetCalibration(adclist[adc - 1]);
    while(ADC_GetResetCalibrationStatus(adclist[adc - 1]));
    ADC_StartCalibration(adclist[adc - 1]);
    while(ADC_GetCalibrationStatus(adclist[adc - 1]));
}

//global configuration for all DMAs
//let's keep it like this for now
void adc_config_all()
{
}

void adc_config_single(uint8_t adc, uint8_t channel, pin_t pin)
{
    if (adc < 1 || adc > 3 || channel > 17)
        return;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 << (adc - 1),ENABLE);
    RCC_APB2PeriphClockCmd(1 << pin.port, ENABLE);

    GPIO_InitTypeDef gpiodef;
    GPIO_StructInit(&gpiodef);
    gpiodef.GPIO_Pin = 1 << pin.pin;
    gpiodef.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(stm32f10x_gpio_ports[pin.port], &gpiodef);

    ADC_InitTypeDef def;
    ADC_StructInit(&def);

    def.ADC_Mode = ADC_Mode_Independent;
    def.ADC_ScanConvMode = DISABLE;
    def.ADC_ContinuousConvMode = DISABLE;
    def.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    def.ADC_DataAlign = ADC_DataAlign_Right;
    def.ADC_NbrOfChannel = 1;
//    def.ADC_Resolution = ADC_Resolution_12b;
//    def.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//    def.ADC_NbrOfConversion = 1;
    ADC_Init(adclist[adc - 1], &def);

    ADC_RegularChannelConfig(adclist[adc - 1], channel, 1, ADC_SampleTime_41Cycles5);

    ADC_Cmd(adclist[adc - 1], ENABLE);

    adc_calibrate(adc);
}

void adc_config_continuous(uint8_t adc, uint8_t *channel, pin_t *pin, uint16_t *dest, uint8_t nb)
{
    if (adc < 1 || adc > 3 || adc == 2) //looks like adc2 has no DMA capability
        return;
    int i;
    for (i = 0 ; i < nb ; i++)
        if (channel[i] < 1 || channel[i] > 17)
            return;

    for (i = 0 ; i < nb ; i++)
    {
        RCC_APB2PeriphClockCmd(1 << pin[i].port, ENABLE);

        GPIO_InitTypeDef gpiodef;
        GPIO_StructInit(&gpiodef);
        gpiodef.GPIO_Pin = 1 << pin[i].pin;
        gpiodef.GPIO_Mode = GPIO_Mode_AIN;
        GPIO_Init(stm32f10x_gpio_ports[pin[i].port], &gpiodef);
    }

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /*
        ADC1: DMA1 Channel 1
        ADC2: ?
        ADC3: DMA2 Channel 5 “only in high-density and XL-density devices”
    */
    DMA_Channel_TypeDef *dmachannels[] = {DMA1_Channel1, NULL, DMA2_Channel5};

    DMA_InitTypeDef dmadef;
    DMA_StructInit(&dmadef);
    DMA_DeInit(dmachannels[adc - 1]);
    dmadef.DMA_PeripheralBaseAddr = (uint32_t)&(adclist[adc - 1])->DR;
    dmadef.DMA_MemoryBaseAddr = (uint32_t) &dest[0];
    dmadef.DMA_DIR = DMA_DIR_PeripheralSRC;//DMA_DIR_PeripheralToMemory;
    dmadef.DMA_BufferSize = nb;
    dmadef.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmadef.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmadef.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dmadef.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dmadef.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;
    dmadef.DMA_Priority = DMA_Priority_High;
    dmadef.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(dmachannels[adc - 1], &dmadef);
    DMA_Cmd(dmachannels[adc - 1], ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 << (adc - 1),ENABLE);

    ADC_InitTypeDef def;
    ADC_StructInit(&def);
    def.ADC_Mode = ADC_Mode_Independent;
    def.ADC_ScanConvMode = ENABLE;
    def.ADC_ContinuousConvMode = ENABLE;
    def.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    def.ADC_DataAlign = ADC_DataAlign_Right;
    def.ADC_NbrOfChannel = nb;
    ADC_Init(adclist[adc - 1], &def);

    for (i = 0 ; i < nb ; i++)
        ADC_RegularChannelConfig(adclist[adc - 1], channel[i], (i + 1), ADC_SampleTime_41Cycles5);

//    ADC_DMARequestAfterLastTransferCmd(adclist[adc - 1], ENABLE);

    ADC_DMACmd(adclist[adc - 1], ENABLE);

    ADC_Cmd(adclist[adc - 1], ENABLE);

    adc_calibrate(adc);

    ADC_SoftwareStartConvCmd(adclist[adc - 1], ENABLE);
}

uint16_t adc_get(uint8_t adc)
{
    if (adc < 1 || adc > 3)
        return 0;

    ADC_SoftwareStartConvCmd(adclist[adc - 1], ENABLE);
    while(!ADC_GetFlagStatus(adclist[adc - 1], ADC_FLAG_EOC)){}
    uint16_t res = ADC_GetConversionValue(adclist[adc - 1]);
    ADC_ClearFlag(adclist[adc - 1], ADC_FLAG_EOC);
    return res;
}


