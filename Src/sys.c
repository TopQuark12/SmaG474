/**
  ******************************************************************************
  * File Name          : SYS.c
  * Description        : This file provides code for the configuration
  *                      of the SYS instances.
  ******************************************************************************
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* SYS init function */
void MX_SYS_Init(void)
{

  /** Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral 
  */
  LL_PWR_DisableDeadBatteryPD();

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
