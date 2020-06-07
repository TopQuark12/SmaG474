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
const float voltageReadingOffset = -0.2694;
const float voltageReadingGain = 4873;
const float currentReadingOffset = -0.01656;
const float currentReadingGain = 78919;
const float currentSetOffset = 0.0069586;
const float currentSetGain = 4987;
static volatile float currentSet = 0;
float currentReading;
float voltageReading;
float resistanceReading;

void smaAfeInit(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcDataRaw, 2);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

static volatile uint32_t dacOutRaw = 0;

void smaAfeLoop(void) 
{
    voltageReading = adcDataRaw[0] / voltageReadingGain - voltageReadingOffset;
    currentReading = adcDataRaw[1] / currentReadingGain - currentReadingOffset;
    resistanceReading = voltageReading / currentReading;
    dacOutRaw = (currentSet - currentSetOffset) * currentSetGain;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t) dacOutRaw);
}

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
// }
