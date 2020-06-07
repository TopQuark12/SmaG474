/**
 * @file smaAFE.c
 * @author Alex Wong
 * @brief
 * @version 0.1
 * @date 2020-05-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "adc.h"
#include "dac.h"
#include "main.h"
#include "tim.h"

uint16_t adcDataRaw[2];

void smaAfeInit(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcDataRaw, 2);
    // HAL_TIM_Base_Start(&htim2);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

static volatile uint32_t dacOutRaw = 0;

void smaAfeLoop(void) 
{
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t) dacOutRaw);

}

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
// }
