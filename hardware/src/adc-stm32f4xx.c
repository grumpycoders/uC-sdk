#include <stm32f4xx.h>
#include <stm32f4xx_adc.h>

#include <adc.h>

ADC_TypeDef *adclist[] = {ADC1, ADC2, ADC3};
extern GPIO_TypeDef *ports[];

//global configuration for all DMAs
//let's keep it like this for now
void adc_config_all()
{
    ADC_CommonInitTypeDef commondef;
    commondef.ADC_Mode = ADC_Mode_Independent;				//Each ADC is independent of others
    commondef.ADC_Prescaler = ADC_Prescaler_Div2;			//smallest prescale
    commondef.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//NO DMA, ADC_DMAAccessMode_1 if we want a sequence of 16b values
    commondef.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//smallest delay
    ADC_CommonInit(&commondef);
}

void adc_config_single(uint8_t adc, uint8_t channel, pin_t pin)
{
    if (adc < 1 || adc > 3 || channel < 1 || channel > 18)
        return;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 << (adc - 1),ENABLE);
    RCC_AHB1PeriphClockCmd(1 << get_port(pin), ENABLE);

    GPIO_InitTypeDef gpiodef;
    GPIO_StructInit(&gpiodef);
    gpiodef.GPIO_Pin = 1 << get_pin(pin);
    gpiodef.GPIO_Mode = GPIO_Mode_AN;
    gpiodef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(ports[get_port(pin)], &gpiodef);

    ADC_InitTypeDef def;
    ADC_StructInit(&def);
    def.ADC_DataAlign = ADC_DataAlign_Right;
    def.ADC_Resolution = ADC_Resolution_12b;
    def.ADC_ContinuousConvMode = DISABLE;
    def.ADC_ExternalTrigConv = 0;
    def.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    def.ADC_NbrOfConversion = 1;
    def.ADC_ScanConvMode = DISABLE;
    ADC_Init(adclist[adc - 1], &def);

    ADC_RegularChannelConfig(adclist[adc - 1], channel, 1, ADC_SampleTime_144Cycles);

    ADC_Cmd(adclist[adc - 1],ENABLE);
}

void adc_config_continuous(uint8_t adc, uint8_t *channel, pin_t *pin, uint16_t *dest, uint8_t nb)
{
    if (adc < 1 || adc > 3)
        return;
    int i;    
    for (i = 0 ; i < nb ; i++)
        if (channel[i] < 1 || channel[i] > 18)
            return;

    for (i = 0 ; i < nb ; i++)
    {
        RCC_AHB1PeriphClockCmd(1 << get_port(pin[i]), ENABLE);

        GPIO_InitTypeDef gpiodef;
        GPIO_StructInit(&gpiodef);
        gpiodef.GPIO_Pin = 1 << get_pin(pin[i]);
        gpiodef.GPIO_Mode = GPIO_Mode_AN;
        gpiodef.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(ports[get_port(pin[i])], &gpiodef);
    }
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

    DMA_InitTypeDef dmadef;
    DMA_StructInit(&dmadef);
    DMA_DeInit(DMA2_Stream4);
    dmadef.DMA_Channel = DMA_Channel_0;
    dmadef.DMA_PeripheralBaseAddr = (uint32_t)&(adclist[adc - 1])->DR;
    dmadef.DMA_Memory0BaseAddr = (uint32_t) &dest[0];
    dmadef.DMA_DIR = DMA_DIR_PeripheralToMemory;
    dmadef.DMA_BufferSize = nb;
    dmadef.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmadef.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmadef.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dmadef.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dmadef.DMA_Mode = DMA_Mode_Circular;
    dmadef.DMA_Priority = DMA_Priority_High;
    dmadef.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dmadef.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    dmadef.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    dmadef.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream4, &dmadef);
    DMA_Cmd(DMA2_Stream4, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 << (adc - 1),ENABLE);

    ADC_InitTypeDef def;
    ADC_StructInit(&def);
    def.ADC_DataAlign = ADC_DataAlign_Right;
    def.ADC_Resolution = ADC_Resolution_12b;
    def.ADC_ContinuousConvMode = ENABLE;
    def.ADC_ExternalTrigConv = 0;
    def.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    def.ADC_NbrOfConversion = nb;
    def.ADC_ScanConvMode = ENABLE;
    ADC_Init(adclist[adc - 1], &def);

    for (i = 0 ; i < nb ; i++)
        ADC_RegularChannelConfig(adclist[adc - 1], channel[i], (i + 1), ADC_SampleTime_144Cycles);

    ADC_DMARequestAfterLastTransferCmd(adclist[adc - 1], ENABLE);

    ADC_DMACmd(adclist[adc - 1], ENABLE);

    ADC_Cmd(adclist[adc - 1],ENABLE);

    ADC_SoftwareStartConv(ADC1);
}

void adc_calibrate(uint8_t adc)
{/*
    if (adc < 1 || adc > 3)
        return;

    ADC_ResetCalibration(adclist[adc - 1]);
    while(ADC_GetResetCalibrationStatus(adclist[adc - 1]));
    ADC_StartCalibration(adclist[adc - 1]);
    while(ADC_GetCalibrationStatus(adclist[adc - 1]));*/
}

uint16_t adc_get(uint8_t adc)
{
    if (adc < 1 || adc > 3)
        return 0;

    ADC_SoftwareStartConv(adclist[adc - 1]);
    while(!ADC_GetFlagStatus(adclist[adc - 1], ADC_FLAG_EOC)){}
    uint16_t res = ADC_GetConversionValue(adclist[adc - 1]);
    ADC_ClearFlag(adclist[adc - 1], ADC_FLAG_EOC);
    return res;
}


