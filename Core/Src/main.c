/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum
{
  IDLE,
  BUSY,
  COLLISION,
} state_t;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DELAY 1110
#define HALF_BIT 700
#define LEN 255
#define NMAX 2000
#define MY_ADDR 0x28
/* PB6 - Channel Monitor
 * PA5 - Transmitter
 * PA0 - Receiver
 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
state_t curr_state;
uint8_t phase = 0;
uint32_t curr_index = 0;
char curr_char;
uint8_t curr_bit = 0;
uint8_t rec_bit = 0;
uint32_t rec_index = 0;
uint8_t size = 0;
char input[261];
char msg_buff[LEN] = "";
char option;
uint8_t curr_level;
uint8_t prev_level = 1;
uint32_t last_edge = 0;
bool first_edge = true;
bool print_msg = false;
bool tx_failed = false;
bool start = false;
uint8_t attempts = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */
//uint8_t gencrc(uint8_t *data, uint32_t len) {
//	uint8_t crc = 0xff;
//	uint32_t i, j;
//	for (i = 0; i < len; i++) {
//		crc ^= data[i];
//		for (j = 0; j < 8; j++) {
//			if ((crc & 0x80) != 0)
//				crc = (uint8_t) ((crc << 1) ^ 0x31);
//			else
//				crc <<= 1;
//		}
//	}
//	return crc;
//}
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
  MX_USART2_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

  curr_state = IDLE;
  HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //SET IDLE
  printf("List of commands\n"
		  "t - Transmit Message\n"
		  "r - Check for received message\n");
  srand(HAL_GetTick());
  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  printf("Enter Address:\n");
  			uint8_t dest_addr;
  			scanf("%d", &dest_addr);
  			printf("Enter Message:\n");
  			char msg[LEN];
  			scanf(" %[^\n]", &msg);
  			size = strlen(msg);
  			input[0] = 0x55;
  			input[1] = MY_ADDR;
  			input[2] = dest_addr;
  			input[3] = size;
  			input[4] = 0x00;
  			for (uint8_t i = 0; i < size; i++) {
  				input[5 + i] = msg[i];
  			}
  			input[5 + size] = 0xAA;

  			phase = 0;
  			curr_bit = 0;
  			curr_index = 0;
  			curr_char = input[curr_index];
  			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET){
  			  			  	  curr_state = COLLISION;
  			  			}
  			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  	printf("Enter cmd\n");
	    scanf("%c", &option);
	    switch (option) {
		case 't':
			printf("Enter Address:\n");
			uint8_t dest_addr;
			scanf("%d", &dest_addr);
			printf("Enter Message:\n");
			char msg[LEN];
			scanf(" %[^\n]", &msg);
			size = strlen(msg);
			input[0] = 0x55;
			input[1] = MY_ADDR;
			input[2] = dest_addr;
			input[3] = size;
			input[4] = 0x00;
			for (uint8_t i = 0; i < size; i++) {
				input[5 + i] = msg[i];
			}
			input[5 + size] = 0xAA;
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET){
			  	  curr_state = COLLISION;
			}
			phase = 0;
			curr_bit = 0;
			curr_index = 0;
			curr_char = input[curr_index];
			start = true;
			break;
		case 'r':
			if (print_msg) {
				printf("Received message from %x: ", msg_buff[1]);
				for (uint16_t i = 5; i < rec_index - 1; i++) {
					printf("%c", msg_buff[i]);
				}
				printf("\n");
				print_msg = false;
				rec_index = 0;
				rec_bit = 0;
				uint8_t prev_level = 1;
				bool first_edge = true;
				memset(msg_buff, 0, LEN);
			}
			break;
		default:
			break;
		}
		switch (curr_state) {
		case IDLE:
//			if (start) {
//				HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
//				start = false;
//			}
			HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(COLL_LED_GPIO_Port, COLL_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
			break;
		case BUSY:
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(COLL_LED_GPIO_Port, COLL_LED_Pin, GPIO_PIN_RESET);
			break;
		case COLLISION:
			if (curr_index < 6 + size && attempts <= 10) {
				HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
				phase = 0;
				curr_bit = 0;
				curr_index = 0;
				curr_char = input[curr_index];
				tx_failed = true;
				TIM5->ARR = (rand() % NMAX) + 1;
				TIM5->EGR |= 0x1;
				HAL_TIM_Base_Start_IT(&htim5);
			}
			HAL_GPIO_WritePin(COLL_LED_GPIO_Port, COLL_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
			break;
		default:
			break;
		}
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
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 500;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 83;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 1110;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_TIM_ENABLE_OCxPRELOAD(&htim4, TIM_CHANNEL_2);
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */
  //Prescaler  of 41999 = Nmax of 2000
  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 41999;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|COLL_LED_Pin|BUSY_LED_Pin|IDLE_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 COLL_LED_Pin BUSY_LED_Pin IDLE_LED_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|COLL_LED_Pin|BUSY_LED_Pin|IDLE_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback (TIM_HandleTypeDef * htim){
	if (htim->Instance == TIM4) {
		HAL_TIM_OC_Stop(&htim4, TIM_CHANNEL_2);
		TIM4->CCR2 = TIM4->CCR1 + DELAY;
		uint32_t temp = TIM4->CNT;
		TIM4->EGR |= 0x1;
		TIM4->CNT = temp;
		HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2);
	    curr_state = BUSY;
	    HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(COLL_LED_GPIO_Port, COLL_LED_Pin, GPIO_PIN_RESET);
	}
	if (htim->Instance == TIM2) {
			uint32_t curr_edge = TIM2->CCR1;
			int diff = abs((int) curr_edge - (int) last_edge);
			if ((first_edge && (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 0))
					|| diff > HALF_BIT) {
				first_edge = false;
				curr_level = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
				if (curr_level > prev_level) { // 1
					msg_buff[rec_index] = (msg_buff[rec_index] << 0x1) | 0x1;
				} else { // 0
					msg_buff[rec_index] = msg_buff[rec_index] << 0x1;
				}
				rec_bit++;
				if (rec_bit > 7) {
					rec_bit = 0;
					rec_index++;
				}
				last_edge = curr_edge;
				prev_level = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
			} else {
				prev_level = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
			}
			if (rec_index > 2 && (msg_buff[2] == MY_ADDR || msg_buff[2] == 0xFF)  && (rec_index >= 6 + msg_buff[3])) {
				print_msg = true;
			}
	}
}

void HAL_TIM_OC_DelayElapsedCallback (TIM_HandleTypeDef * htim){
	if(htim->Instance == TIM4) {
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET){
			curr_state = IDLE;
			HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(COLL_LED_GPIO_Port, COLL_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
		} else{
			curr_state = COLLISION;
			HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
			phase = 0;
			curr_bit = 0;
			curr_index = 0;
			curr_char = input[curr_index];
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(COLL_LED_GPIO_Port, COLL_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IDLE_LED_GPIO_Port, IDLE_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
		}
	}
	if(htim->Instance == TIM3){
		if(!tx_failed && curr_state != COLLISION && curr_index < 6+ size){
			if(phase == 0){
				if((( ((uint8_t)curr_char) >>(7-curr_bit)) & 1) == 1){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				} else{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				}
			} else{
				if((( ((uint8_t)curr_char) >>(7-curr_bit)) & 1) == 1){
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				} else {
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				}
					curr_bit++;
			}
			if(curr_bit > 7){
				curr_bit = 0;
				curr_index++;
			}
			phase ^= 1;
			curr_char = input[curr_index];
			if(curr_index >= 6 + size){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); //SET TO IDLE
				HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
				tx_failed = false;
				attempts = 0;
			}
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM5) {
		if (tx_failed && curr_state == IDLE && attempts <= 10) {
			HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
			tx_failed = false;
			attempts++;
		}
//		else{
//			TIM5->ARR = (rand() % NMAX) + 1;
//			TIM5->EGR |= 0x1;
//			HAL_TIM_Base_Start_IT(&htim5);
//		}
	}
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
