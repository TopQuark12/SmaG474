# SmaG474
Created by Wong Tat Hang (thwongaz@connect.ust.hk) for HKUST 2019-2020 ISDN2002 course independent study.     
This project was created to control the shrinkage of two-way shape memory alloy wire in a closed loop manner.  
This is possible as two-way SMA wire shrinks in length and expands in diameter as it heats up, resulting in lower electrical resistence of the wire. The changing electrical resistance can be measured and used as a sensor to measure the wire shrinkage. In this project, a system is created to utilize this material property to create a closed loop controlled SMA actuator without external sensors.
## System architecture
The system is comprised of 3 parts:  
- The SMA wire ebmedded into an actuator mechanism  
- The analog front end circuit board, which  
    - Measures current with shunt resistor and differential amplifier, analog output to MCU ADC
    - Measures voltage across the SMA wire with differential amplifier, analog output to MCU ADC
    - Power the SMA with a linear current regulator, using the current signal, error amplifier and series pass transistor, with input from MCU DAC
- The STmicroelectronics Nucleo-64 STM32G474 development board  
## The software
This repo contains code for the STM32 MCU.  
Compiler: arm-gcc-none-eabi 9-2019-q4-major  
Build tool: GNU make  
MCU init code generation tool: STM32CubeMx
