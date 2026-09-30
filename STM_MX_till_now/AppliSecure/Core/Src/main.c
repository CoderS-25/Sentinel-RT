/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cacheaxi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <tk/tkernel.h>
#include <stdio.h>
#include "../../Sentinel/sentinel_rt.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define VECT_TAB_NS_OFFSET  0x00400
#define VTOR_TABLE_NS_START_ADDR (SRAM2_AXI_BASE_NS|VECT_TAB_NS_OFFSET)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void NonSecure_Init(void);
static void SystemIsolation_Config(void);
/* USER CODE BEGIN PFP */
extern void knl_start_mtkernel(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  extern UART_HandleTypeDef huart3;
  
  MX_GPIO_Init();
  
  MX_CACHEAXI_Init();
  
  MX_USART3_UART_Init();
  HAL_UART_Transmit(&huart3, (uint8_t*)"1. UART Init Done\r\n", 19, 100);
  
  SystemIsolation_Config();
  HAL_UART_Transmit(&huart3, (uint8_t*)"2. System Isolation Done\r\n", 26, 100);
  
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart3, (uint8_t*)"3. Starting Hardware LED Test...\r\n", 34, 100);

  /* --- HARDWARE SANITY CHECK: BLINK GREEN LED --- */
  /* 1. Enable the clock for GPIO Port O */
  __HAL_RCC_GPIOO_CLK_ENABLE();

  /* 2. Configure Port O, Pin 1 as an Output */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // Push-Pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);

  /* 3. Hardware blink test removed! We are booting the RTOS now. */
  /* ---------------------------------------------- */
  
  knl_start_mtkernel();  /* Hand off control to the TRON Kernel */
  
  /* If we reach here, the RTOS crashed and returned! */
  uint8_t msg2[] = "ERROR: RTOS returned!\r\n";
  HAL_UART_Transmit(&huart3, msg2, sizeof(msg2)-1, 100);
  /* USER CODE END 2 */

  /* Secure SysTick should rather be suspended before calling non-secure  */
  /* in order to avoid wake-up from sleep mode entered by non-secure      */
  /* The Secure SysTick shall be resumed on non-secure callable functions */
  HAL_SuspendTick();

  /*************** Setup and jump to non-secure *******************************/

  NonSecure_Init();

  /* Non-secure software does not return, this code is not executed */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief  Non-secure call function
  *         This function is responsible for Non-secure initialization and switch
  *         to non-secure state
  * @retval None
  */
static void NonSecure_Init(void)
{
  funcptr_NS NonSecure_ResetHandler;

  SCB_NS->VTOR = VTOR_TABLE_NS_START_ADDR;

  /* Set non-secure main stack (MSP_NS) */
  __TZ_set_MSP_NS((*(uint32_t *)VTOR_TABLE_NS_START_ADDR));

  /* Get non-secure reset handler */
  NonSecure_ResetHandler = (funcptr_NS)(*((uint32_t *)((VTOR_TABLE_NS_START_ADDR) + 4U)));

  /* Start non-secure state software application */
  NonSecure_ResetHandler();
}

/**
  * @brief RIF Initialization Function
  * @param None
  * @retval None
  */
  static void SystemIsolation_Config(void)
{

/* USER CODE BEGIN RIF_Init 0 */

/* USER CODE END RIF_Init 0 */

  /* set all required IPs as secure privileged */
  __HAL_RCC_RIFSC_CLK_ENABLE();

  /*RISUP configuration*/
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_USART3 , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_NPRIV);
  HAL_RIF_RISC_SetSlaveSecureAttributes(RIF_RISC_PERIPH_INDEX_NPU , RIF_ATTRIBUTE_SEC | RIF_ATTRIBUTE_NPRIV);

  /* RIF-Aware IPs Config */

  /* set up GPIO configuration */
  HAL_GPIO_ConfigPinAttributes(GPIOD,GPIO_PIN_8,GPIO_PIN_SEC|GPIO_PIN_NPRIV);
  HAL_GPIO_ConfigPinAttributes(GPIOD,GPIO_PIN_9,GPIO_PIN_SEC|GPIO_PIN_NPRIV);

/* USER CODE BEGIN RIF_Init 1 */

/* USER CODE END RIF_Init 1 */
/* USER CODE BEGIN RIF_Init 2 */

/* USER CODE END RIF_Init 2 */

}

/* USER CODE BEGIN 4 */
/* Official TRON entry point — Sentinel-RT lives here.
 * This function is called by the uT-Kernel scheduler as the first task.
 * At this point, the RTOS is fully running and it is safe to create tasks. */
void usermain(void)
{
    extern UART_HandleTypeDef huart3;
    HAL_UART_Transmit(&huart3, (uint8_t*)"[Sentinel-RT] Kernel up!\r\n", 26, 200);
    sentinel_rt_init();  /* Starts Profiler, Inference Engine, Decision Engine */
    tk_ext_tsk();        /* Exit this init task so the kernel can take over */
}

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART3 and Loop until the end of transmission */
    extern UART_HandleTypeDef huart3;
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
