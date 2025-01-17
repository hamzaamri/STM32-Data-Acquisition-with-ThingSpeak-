/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include<stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
 uint16_t Val=0;
 volatile int FlagTimeToSendData=0;
 volatile int Timer_count=0;
 uint8_t api_key[] = "DSLLF9CQ2NVSP7FD";
 uint8_t CMD_AT0[]="AT\r\n";
 uint8_t CMD_AT1[]="AT+RST\r\n";
 uint8_t CMD_AT2[]="AT+CWMODE=1\r\n";
 uint8_t CMD_AT3[] = "AT+CWJAP=\"hamza\",\"hamzanote20\"\r\n";
 uint8_t CMD_AT6[] ="AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n";
 uint8_t CMD_AT7[]="AT+CIPSEND=100\r\n";
 uint8_t CMD_AT8[] = "AT+CIPCLOSE\r\n";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void configure_esp() {

    if(HAL_UART_Transmit(&huart2, CMD_AT0, sizeof(CMD_AT0), 10) != HAL_OK) {
        Error_Handler();

    }
    HAL_Delay(500);

    if(HAL_UART_Transmit(&huart2, CMD_AT1, sizeof(CMD_AT1), 10) != HAL_OK) {
        Error_Handler();

    }
    HAL_Delay(500);

    if(HAL_UART_Transmit(&huart2, CMD_AT2, sizeof(CMD_AT2), 10) != HAL_OK) {
        Error_Handler();

    }
    HAL_Delay(500);

    if(HAL_UART_Transmit(&huart2, CMD_AT3, sizeof(CMD_AT3), 10) != HAL_OK) {
        Error_Handler();

    }
    HAL_Delay(500);
}
void send_data_to_thingspeak(UART_HandleTypeDef *huart, uint8_t api_key[], int data_value) {

	    uint8_t CMD_AT_SEND[128];
	    sprintf((char *)CMD_AT_SEND, "GET https://api.thingspeak.com/update?api_key=%s&field1=%d\r\n", api_key, data_value);


	    char CMD_AT7[32];
	    sprintf(CMD_AT7, "AT+CIPSEND=%lu\r\n", (unsigned long)strlen((char *)CMD_AT_SEND));

	    if (HAL_UART_Transmit(huart, (uint8_t *)CMD_AT7, strlen(CMD_AT7), 10) != HAL_OK) {
	        Error_Handler();
	    }
	    HAL_Delay(1000);
	    if (HAL_UART_Transmit(huart, (uint8_t *)CMD_AT_SEND, strlen((char *)CMD_AT_SEND), 10) != HAL_OK) {
	        Error_Handler();

	    }
	    HAL_Delay(2000);

	    char CMD_AT8[] = "AT+CIPCLOSE\r\n";
	    if (HAL_UART_Transmit(huart, (uint8_t *)CMD_AT8, strlen(CMD_AT8), 10) != HAL_OK) {
	        Error_Handler();
	    }
}
void send_data() {
	//FlagTimeToSendData = 0 ;
    if(HAL_UART_Transmit(&huart2, CMD_AT6, sizeof(CMD_AT6), 10) != HAL_OK) {
        Error_Handler();
    }
    HAL_Delay(500);

    send_data_to_thingspeak(&huart2, api_key, Val);
    HAL_Delay(500);
    if(HAL_UART_Transmit(&huart2, CMD_AT8, sizeof(CMD_AT8), 10) != HAL_OK) {
        Error_Handler();
    }
   HAL_Delay(500);

}
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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  //HAL_TIM_Base_Start_IT(&htim3);
  configure_esp();  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(FlagTimeToSendData){
		  send_data();
		 // Timer_count=1;
		  HAL_TIM_Base_Start_IT(&htim3);
		  //FlagTimeToSendData=0;

		 // timer_count=true;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_0) {
    	if(HAL_ADC_Start_IT(&hadc1) != HAL_OK) {
    		        Error_Handler();
    		    }
       // adc_config();
       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
        Val = (uint16_t)HAL_ADC_GetValue(&hadc1);
        //FlagTimeToSendData = 1 ;
        if(HAL_ADC_Stop_IT(&hadc1) != HAL_OK) {
                    Error_Handler();
        }

        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
        FlagTimeToSendData = 1 ;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//if(Timer_count){
	FlagTimeToSendData=0;
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);

        if(HAL_ADC_Start_IT(&hadc1) != HAL_OK) {
        	        Error_Handler();
        	    }
        //Timer_count=0;
//}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
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

#ifdef  USE_FULL_ASSERT
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
