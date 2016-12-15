#include "dac.h"

#include <stm32f4xx.h>
#include <stm32f4xx_dac.h>


/*
    TODO: DACs can be triggered by a timer
    Timers TRGO: TIM2, TIM4, TIM5, TIM6, TIM7 and TIM8
          (DAC_Trigger_T2_TRGO, DAC_Trigger_T4_TRGO...)
          The timer TRGO event should be selected using TIM_SelectOutputTrigger()
*/
void dac_config(dac_port_t dac_port)
{
    if (dac_port.dac < 1 || dac_port.dac > 2)
        return;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    gpio_config_analog(dac_port.pin, pin_dir_write, pull_none);

    DAC_InitTypeDef dac;
    dac.DAC_Trigger = DAC_Trigger_None;
    dac.DAC_WaveGeneration = DAC_WaveGeneration_None;
    dac.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
    dac.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

    if (dac_port.dac == dac_1)
    {
        DAC_Init(DAC_Channel_1, &dac);
        DAC_Cmd(DAC_Channel_1, ENABLE);
    }
    else
    {
        DAC_Init(DAC_Channel_2, &dac);
        DAC_Cmd(DAC_Channel_2, ENABLE);
    }
}

//set 8 bits value
void dac_set(dac_t dac, uint8_t value)
{
    if (dac == 1)
        DAC_SetChannel1Data(DAC_Align_8b_R, (uint16_t) value);
    else
        DAC_SetChannel2Data(DAC_Align_8b_R, (uint16_t) value);
}

//set 16 bits value
void dac_set16(dac_t dac, uint16_t value)
{
    if (dac == 1)
        DAC_SetChannel1Data(DAC_Align_12b_R, (uint16_t) value);
    else
        DAC_SetChannel2Data(DAC_Align_12b_R, (uint16_t) value);
}
