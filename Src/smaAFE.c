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

#define NCoef 4

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
float shrinkageFiltered;
float shrinkageSetting = 0.1;
float kP = 2;
float kI = 0.07;
float kD = 0;
float P;
float I;
float D;
float integral = 0;
float err = 0;
float errLast = 0;
float integralMin = 0;    //setting go down
float integralMax = 5;     //setting go up

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
    float ACoef[NCoef + 1] = {0.00482136451303671110,
                              0.01928545805214684400,
                              0.02892818707822026800,
                              0.01928545805214684400,
                              0.00482136451303671110};

    float BCoef[NCoef + 1] = {1.00000000000000000000,
                              -2.36951300718203720000,
                              2.31398841441587950000,
                              -1.05466540587856720000,
                              0.18737949236818485000};

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
    if (HAL_GetTick() > 250)
    {
        shrinkage = map(resistanceReading, 4.6, 4.1, 0, 1);
        err = shrinkageSetting - shrinkage;
        integral += err;
        // integral = cap(integral, integralMin, integralMin);
        if (integral > integralMax)
            integral = integralMax;
        if (integral < integralMin)
            integral = integralMin;
        // integral *= 0.995;
        D = (err - errLast) * kD;
        I = kI * integral;
        P = kP * err;
        currentSet = P + I + D;
        errLast = err;
    }
    currentSet = cap(currentSet, currentSetMin, currentSetMax);
    dacOutRaw = (currentSet - currentSetOffset) * currentSetGain;
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t) dacOutRaw);
}

// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
// {
//     HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
// }
