/*
 * hss-shield.c
 *
 *  Created on: Mar 21, 2023
 *      Author: kgaya
 */

#include "hss-shield.h"
#include "stdio.h"

//struct profet_params BTS7002;

enum DiagStatus_t diagStatus;

struct profet_params bts_7002 = {
    .kilis = 22700,
    .iisFault = 0.0044,
    .iisOl = 0.0018,
    .iisEn = 0.000002,
	.rSense = 4700,
	.vBatOffset = 0.26,
	.currentOffset = 0.15,
	.vss_threshold = 0.05
};


struct shield_status my_status = {0};

ExponentialFilterState vfilt = {
    .tau = 100,
    .y = 0.0f,
    .y_last = 0.0f,
    .x = 0.0f
};

ExponentialFilterState cfilt_1 = {
    .tau = 100,
    .y = 0.0f,
    .y_last = 0.0f,
    .x = 0.0f
};

ExponentialFilterState cfilt_2 = {
    .tau = 100,
    .y = 0.0f,
    .y_last = 0.0f,
    .x = 0.0f
};

ExponentialFilterState cfilt_3 = {
    .tau = 100,
    .y = 0.0f,
    .y_last = 0.0f,
    .x = 0.0f
};

ExponentialFilterState cfilt_4 = {
    .tau = 100,
    .y = 0.0f,
    .y_last = 0.0f,
    .x = 0.0f
};


void HSS_Shield_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_3_PIN|LED_4_PIN|IN_3_PIN|DEN2_DEN4_PIN|OLOFF_PIN|
      PUSHBUTTON_DIGITAL_PIN|PUSHBUTTON_ANALOG_PIN|VOLTAGE_SENSE_VBAT_PIN|CURRENT_SENSE_U1U2_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_1_PIN|LED_2_PIN|IN_2_PIN|IN_4_PIN|DEN1_DEN3_PIN|CURRENT_SENSE_U3U4_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, IN_1_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pins : PORT A */
  GPIO_InitStruct.Pin = LED_3_PIN|LED_4_PIN|IN_3_PIN|DEN2_DEN4_PIN|OLOFF_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PUSHBUTTON_DIGITAL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PUSHBUTTON_ANALOG_PIN|VOLTAGE_SENSE_VBAT_PIN|CURRENT_SENSE_U1U2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PORT B */
  GPIO_InitStruct.Pin = LED_1_PIN|LED_2_PIN|IN_2_PIN|IN_4_PIN|DEN1_DEN3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = CURRENT_SENSE_U3U4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PORT C */
  GPIO_InitStruct.Pin = IN_1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


void HSS_Shield_ADC_Init(ADC_HandleTypeDef *hadc)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc->Instance = ADC1;
  hadc->Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc->Init.ContinuousConvMode = ENABLE;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    Error_Handler();
  }

}

void ADC_Select_CH0(ADC_HandleTypeDef *hadc)
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_0;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_384CYCLES;
	  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH1(ADC_HandleTypeDef *hadc)
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_384CYCLES;
	  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH2(ADC_HandleTypeDef *hadc)
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_384CYCLES;
	  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH3(ADC_HandleTypeDef *hadc)
{
	  ADC_ChannelConfTypeDef sConfig = {0};
	  sConfig.Channel = ADC_CHANNEL_8;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_384CYCLES;
	  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}



void enable_CH(enum Channel_t ch)
{
	//extern struct shield_status my_status;

	if (ch == CHANNEL1){
			HAL_GPIO_WritePin(IN_1_PORT, IN_1_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_SET);
			my_status.in1 = P_ON;
		}
	else if (ch == CHANNEL2){
			HAL_GPIO_WritePin(IN_2_PORT, IN_2_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_SET);
			my_status.in2 = P_ON;
		}
	else if (ch == CHANNEL3){
			HAL_GPIO_WritePin(IN_3_PORT, IN_3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_SET);
			my_status.in3 = P_ON;
		}
	else if (ch == CHANNEL4){
			HAL_GPIO_WritePin(IN_4_PORT, IN_4_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_SET);
			my_status.in4 = P_ON;
		}
	else if (ch == CHANNEL_ALL){
			HAL_GPIO_WritePin(IN_1_PORT, IN_1_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IN_2_PORT, IN_2_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IN_3_PORT, IN_3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(IN_4_PORT, IN_4_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_SET);
			my_status.in1 = P_ON;
			my_status.in2 = P_ON;
			my_status.in3 = P_ON;
			my_status.in4 = P_ON;
		}
}

void disable_CH(enum Channel_t ch)
{
	if (ch == CHANNEL1){
			HAL_GPIO_WritePin(IN_1_PORT, IN_1_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_RESET);
			my_status.in1 = P_OFF;
		}
	else if (ch == CHANNEL2){
			HAL_GPIO_WritePin(IN_2_PORT, IN_2_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);
			my_status.in2 = P_OFF;
		}
	else if (ch == CHANNEL3){
			HAL_GPIO_WritePin(IN_3_PORT, IN_3_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_RESET);
			my_status.in3 = P_OFF;
		}
	else if (ch == CHANNEL4){
			HAL_GPIO_WritePin(IN_4_PORT, IN_4_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_RESET);
			my_status.in4 = P_OFF;
		}
	else if (ch == CHANNEL_ALL){
			HAL_GPIO_WritePin(IN_1_PORT, IN_1_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN_2_PORT, IN_2_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN_3_PORT, IN_3_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN_4_PORT, IN_4_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_RESET);
			my_status.in1 = P_OFF;
			my_status.in2 = P_OFF;
			my_status.in3 = P_OFF;
			my_status.in4 = P_OFF;
		}
}

void diag_switch_en(enum Channel_t ch)
{
	if ((ch == CHANNEL1)|(ch == CHANNEL3)){
			HAL_GPIO_WritePin(DEN1_DEN3_PORT, DEN1_DEN3_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DEN2_DEN4_PORT, DEN2_DEN4_PIN, GPIO_PIN_RESET);
			my_status.diag_ctrl1 = P_ON;
			}
	else if ((ch == CHANNEL2)|(ch == CHANNEL4)){
			HAL_GPIO_WritePin(DEN2_DEN4_PORT, DEN2_DEN4_PIN, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DEN1_DEN3_PORT, DEN1_DEN3_PIN, GPIO_PIN_RESET);
			my_status.diag_ctrl2 = P_ON;
		}
}

void diag_switch_disen(enum Channel_t ch)
{
	if ((ch == CHANNEL1)|(ch == CHANNEL3)){
			HAL_GPIO_WritePin(DEN1_DEN3_PORT, DEN1_DEN3_PIN, GPIO_PIN_RESET);
			my_status.diag_ctrl1 = P_OFF;
			}
	else if ((ch == CHANNEL2)|(ch == CHANNEL4)){
			HAL_GPIO_WritePin(DEN2_DEN4_PORT, DEN2_DEN4_PIN, GPIO_PIN_RESET);
			my_status.diag_ctrl2 = P_OFF;
		}
	else if (ch == CHANNEL_ALL){
			HAL_GPIO_WritePin(DEN1_DEN3_PORT, DEN1_DEN3_PIN, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DEN2_DEN4_PORT, DEN2_DEN4_PIN, GPIO_PIN_RESET);
			my_status.diag_ctrl1 = P_OFF;
			my_status.diag_ctrl2 = P_OFF;
			}
}

void enable_diag(enum Channel_t ch)
{
	if (ch == CHANNEL1){
		my_status.diag1 = P_ON;
	}else if (ch == CHANNEL2){
		my_status.diag2 = P_ON;
	}else if (ch == CHANNEL3){
		my_status.diag3 = P_ON;
	}else if (ch == CHANNEL4){
		my_status.diag4 = P_ON;
	}else if(ch == CHANNEL_ALL){
		my_status.diag1 = P_ON;
		my_status.diag2 = P_ON;
		my_status.diag3 = P_ON;
		my_status.diag4 = P_ON;
	}
}

void disable_diag(enum Channel_t ch)
{
	if (ch == CHANNEL1){
		my_status.diag1 = P_OFF;
	}else if (ch == CHANNEL2){
		my_status.diag2 = P_OFF;
	}else if (ch == CHANNEL3){
		my_status.diag3 = P_OFF;
	}else if (ch == CHANNEL4){
		my_status.diag4 = P_OFF;
	}else if(ch == CHANNEL_ALL){
		my_status.diag1 = P_OFF;
		my_status.diag2 = P_OFF;
		my_status.diag3 = P_OFF;
		my_status.diag4 = P_OFF;
	}
}

uint16_t analog_read(ADC_HandleTypeDef *hadc,int ch)
{
	uint16_t adc = 0;

	if (ch == 1){
		ADC_Select_CH0(hadc);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 10);
		adc = HAL_ADC_GetValue(hadc);
		HAL_ADC_Stop(hadc);

	}
	else if(ch == 2){
		ADC_Select_CH1(hadc);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 10);
		adc = HAL_ADC_GetValue(hadc);
		HAL_ADC_Stop(hadc);
	}
	else if(ch == 3){
		ADC_Select_CH2(hadc);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 10);
		adc = HAL_ADC_GetValue(hadc);
		HAL_ADC_Stop(hadc);
	}
	else if(ch == 4){
		ADC_Select_CH3(hadc);
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 10);
		adc = HAL_ADC_GetValue(hadc);
		HAL_ADC_Stop(hadc);
	}
	return adc;
}

float readIs(ADC_HandleTypeDef *hadc,enum Channel_t ch)
{
	uint16_t adc = 0;
	float isCurrent = 0;
	float isVoltage = 0;

	switch(ch)
	{
	case 1:
				if ((my_status.diag_ctrl1 == 0) & (my_status.diag_ctrl1 == 0) & (my_status.diag1 == P_ON)){
					my_status.adc1 = P_ON;
					//enable_CH(ch);
					diag_switch_en(ch);
					adc = analog_read(hadc, 3);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_1, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_1);
					my_status.adc1 = P_OFF;
				}
				else if (my_status.diag1 == P_ON) {
					my_status.adc1 = P_ON;
					//enable_CH(ch);
					diag_switch_disen(CHANNEL_ALL);
					diag_switch_en(ch);
					adc = analog_read(hadc, 3);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_1, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_1);
					my_status.adc1 = P_OFF;
				}
				break;
	case 2:
				if ((my_status.diag_ctrl1 == 0) & (my_status.diag_ctrl2 == 0) & (my_status.diag2 == P_ON)){
					my_status.adc1 = P_ON;
					//enable_CH(ch);
					diag_switch_en(ch);
					adc = analog_read(hadc, 3);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_2, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_2);
					my_status.adc1 = P_OFF;
				}
				else if (my_status.diag2 == P_ON){
					my_status.adc1 = P_ON;
					//enable_CH(ch);
					diag_switch_disen(CHANNEL_ALL);
					diag_switch_en(ch);
					adc = analog_read(hadc, 3);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_2, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_2);
					my_status.adc1 = P_OFF;
				}
				break;
	case 3:
				if ((my_status.diag_ctrl1 == 0) & (my_status.diag_ctrl2 == 0) & (my_status.diag3 == P_ON)){
					my_status.adc2 = P_ON;
					//enable_CH(ch);
					diag_switch_en(ch);
					adc = analog_read(hadc, 4);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_3, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_3);
					my_status.adc2 = P_OFF;
				}
				else if (my_status.diag3 == P_ON){
					my_status.adc2 = P_ON;
					//enable_CH(ch);
					diag_switch_disen(CHANNEL_ALL);
					diag_switch_en(ch);
					adc = analog_read(hadc, 4);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_3, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_3);
					my_status.adc2 = P_OFF;
				}
				break;
	case 4:
				if ((my_status.diag_ctrl1 == 0) & (my_status.diag_ctrl2 == 0) & (my_status.diag4 == P_ON)){
					my_status.adc2 = P_ON;
					//enable_CH(ch);
					diag_switch_en(ch);
					adc = analog_read(hadc, 4);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_4, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_4);
					my_status.adc2 = P_OFF;
				}
				else if (my_status.diag4 == P_ON){
					my_status.adc2 = P_ON;
					//enable_CH(ch);
					diag_switch_disen(CHANNEL_ALL);
					diag_switch_en(ch);
					adc = analog_read(hadc, 4);
					diag_switch_disen(ch);
					isVoltage = (adc/(float)4096.0)*(float)20.0;
					isCurrent = ((isVoltage*bts_7002.kilis)/bts_7002.rSense) - bts_7002.currentOffset;
					exponentialFilter_input(&cfilt_4, isCurrent);
					isCurrent = exponentialFilter_output(&cfilt_4);
					my_status.adc2 = P_OFF;
				}
				break;
	case 5:
				break;
	}
		return isCurrent;
}

float readVss(ADC_HandleTypeDef *hadc)
{
	float voltage = 0;
	uint16_t adc = 0;
	adc = analog_read(hadc, 2);
	voltage = adc * ((float)20/(float)4096);
	voltage = (voltage - bts_7002.vBatOffset);
	exponentialFilter_input(&vfilt, voltage);
	voltage = exponentialFilter_output(&vfilt);
	return voltage;
}



enum DiagStatus_t diagRead_On(float i_sense, float vss)
{
	if(vss <= bts_7002.vss_threshold){
		diagStatus = FAULT_VSS;
	}else{
		if(i_sense >= (bts_7002.iisFault * bts_7002.kilis)){
			diagStatus = FAULT;
		}
		else if((i_sense <= (bts_7002.iisEn * bts_7002.kilis))){
			diagStatus = FAULT_OL_IC;
		}
		else{
			diagStatus = NORMAL;
		}
	}
	return diagStatus;
}

enum DiagStatus_t diagRead_Off(float currentOn, float currentOff,float vss)
{
	if(vss <= bts_7002.vss_threshold){
			diagStatus = FAULT_VSS;
	}else{
		if((currentOn > (bts_7002.iisOl * bts_7002.kilis)) && (currentOn < (bts_7002.iisFault * bts_7002.kilis))){
			if((currentOff > (bts_7002.iisOl * bts_7002.kilis)) && (currentOff < (bts_7002.iisFault * bts_7002.kilis))){
				return SHORT_TO_VSS;
			}
			else{
				return OPEN_LOAD;
			}
		}
		else{
			if((currentOn > (bts_7002.iisFault * bts_7002.kilis))){
				return SHORT_TO_GND_OR_OT;
			}
			else{
				return NORMAL;
			}
		}
	}
	return diagStatus;
}

enum DiagStatus_t diagRead_State(ADC_HandleTypeDef *hadc,enum Channel_t ch)
{
	float currentOn = 0.0;
	float currentOff = 0.0;
	float vss = 0.0;

	switch(ch)
	{
	case 1:
			//diag_switch_en(ch);
		if(my_status.diag1 == P_OFF){
			diagStatus = NOT_ENABLED;
		}else{
			if(my_status.in1 == 1){
				currentOn = readIs(hadc,ch);
				vss = readVss(hadc);
				diagStatus = diagRead_On(currentOn,vss);
			}else
			{
				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_SET);
				//HAL_Delay(100);
				currentOn = readIs(hadc,ch);

				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_RESET);
				//HAL_Delay(400);
				currentOff = readIs(hadc,ch);
				vss = readVss(hadc);

				diagStatus = diagRead_Off(currentOn, currentOff,vss);
			}
			diag_switch_disen(ch);
		}
			break;

	case 2:
			//diag_switch_en(ch);
		if(my_status.diag2 == P_OFF){
			diagStatus = NOT_ENABLED;
		}else{
			if(my_status.in2 == 1){
				currentOn = readIs(hadc,ch);
				vss = readVss(hadc);
				diagStatus = diagRead_On(currentOn,vss);
			}else
			{
				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_SET);
				//HAL_Delay(100);
				currentOn = readIs(hadc,ch);

				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_RESET);
				//HAL_Delay(400);
				currentOff = readIs(hadc,ch);
				vss = readVss(hadc);

				diagStatus = diagRead_Off(currentOn, currentOff,vss);
			}
			diag_switch_disen(ch);
		}
			break;

	case 3:
			//diag_switch_en(ch);
		if(my_status.diag3 == P_OFF){
			diagStatus = NOT_ENABLED;
		}else{
			if(my_status.in3 == 1){
				currentOn = readIs(hadc,ch);
				vss = readVss(hadc);
				diagStatus = diagRead_On(currentOn,vss);
			}else
			{
				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_SET);
				//HAL_Delay(100);
				currentOn = readIs(hadc,ch);

				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_RESET);
				//HAL_Delay(400);
				currentOff = readIs(hadc,ch);
				vss = readVss(hadc);

				diagStatus = diagRead_Off(currentOn, currentOff,vss);
			}
			diag_switch_disen(ch);
		}
			break;

	case 4:
			//diag_switch_en(ch);
		if(my_status.diag4 == P_OFF){
			diagStatus = NOT_ENABLED;
		}else{
			if(my_status.in4 == 1){
				currentOn = readIs(hadc,ch);
				vss = readVss(hadc);
				diagStatus = diagRead_On(currentOn,vss);
			}else
			{
				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_SET);
				//HAL_Delay(100);
				currentOn = readIs(hadc,ch);

				HAL_GPIO_WritePin(OLOFF_PORT, OLOFF_PIN, GPIO_PIN_RESET);
				//HAL_Delay(400);
				currentOff = readIs(hadc,ch);
				vss = readVss(hadc);

				diagStatus = diagRead_Off(currentOn, currentOff,vss);
			}
			diag_switch_disen(ch);
		}
			break;

	case 5:
		break;
	}
	return diagStatus;
}
