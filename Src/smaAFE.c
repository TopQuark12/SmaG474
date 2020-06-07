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

#define NCoef 2

uint16_t adcDataRaw[2];
float voltageReadingOffset = -0.24;
float voltageReadingGain = 4959;
float currentReadingOffset = -0.01656;
float currentReadingGain = 78919;
float currentSetOffset = 0.0069586;
float currentSetGain = 4987;
float currentSetMin = 0.2;
float currentSetMax = 0.5;
static volatile float currentSet = 0;
float currentReading;
float voltageReading;
float resistanceReading;
float shrinkage;
float shrinkageSetting;

void smaAfeInit(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcDataRaw, 2);
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
}

static volatile uint32_t dacOutRaw = 0;

float cap(float val, float min, float max)
{
    if (val > max)
        return max;
    if (val < min)
        return min;
    return val;
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float iir(float NewSample)
{
    float ACoef[NCoef + 1] = {
        0.06745527606901530200, 0.13491055213803060000, 0.06745527606901530200};

    float BCoef[NCoef + 1] = {1.00000000000000000000,
                              -1.14298050253990090000,
                              0.41280159809618860000};

    static float y[NCoef + 1];  // output samples
    static float x[NCoef + 1];  // input samples
    int n;

    // shift the old samples
    for (n = NCoef; n > 0; n--)
    {
        x[n] = x[n - 1];
        y[n] = y[n - 1];
    }

    // Calculate the new output
    x[0] = NewSample;
    y[0] = ACoef[0] * x[0];
    for (n = 1; n <= NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];

    return y[0];
}

void smaAfeLoop(void) 
{
    voltageReading = adcDataRaw[0] / voltageReadingGain - voltageReadingOffset;
    currentReading = adcDataRaw[1] / currentReadingGain - currentReadingOffset;
    resistanceReading = voltageReading / currentReading;
    shrinkage = map(resistanceReading, 4.64, 4.1, 0, 1);
    currentSet = shrinkage < shrinkageSetting ? currentSetMax : currentSetMin;
    currentSet = cap(currentSet, currentSetMin, currentSetMax);
    dacOutRaw = (currentSet - currentSetOffset) * currentSetGain;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t) dacOutRaw);
}

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
// }
