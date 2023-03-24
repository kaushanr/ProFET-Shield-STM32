/*
 * hss-shield.h
 *
 *  Created on: Mar 21, 2023
 *      Author: kgaya
 */

#ifndef INC_HSS_SHIELD_H_
#define INC_HSS_SHIELD_H_

#include "main.h"
#include "stm32l1xx_hal.h"
#include "hss-filter.h"

// digital pinouts

#define LED_1_PIN   GPIO_PIN_5
#define LED_1_PORT  GPIOB
#define LED_2_PIN   GPIO_PIN_4
#define LED_2_PORT  GPIOB
#define LED_3_PIN   GPIO_PIN_6
#define LED_3_PORT  GPIOA
#define LED_4_PIN   GPIO_PIN_5
#define LED_4_PORT  GPIOA
#define IN_1_PIN   GPIO_PIN_7
#define IN_1_PORT  GPIOC
#define IN_2_PIN   GPIO_PIN_6
#define IN_2_PORT  GPIOB
#define IN_3_PIN   GPIO_PIN_7
#define IN_3_PORT  GPIOA
#define IN_4_PIN   GPIO_PIN_3
#define IN_4_PORT  GPIOB
#define DEN1_DEN3_PIN   GPIO_PIN_10
#define DEN1_DEN3_PORT  GPIOB
#define DEN2_DEN4_PIN   GPIO_PIN_9
#define DEN2_DEN4_PORT  GPIOA
#define OLOFF_PIN   GPIO_PIN_8
#define OLOFF_PORT  GPIOA
#define PUSHBUTTON_DIGITAL_PIN   GPIO_PIN_10
#define PUSHBUTTON_DIGITAL_PORT  GPIOA

// analog pinouts

#define PUSHBUTTON_ANALOG_PIN   GPIO_PIN_0
#define PUSHBUTTON_ANALOG_PORT  GPIOA
#define VOLTAGE_SENSE_VBAT_PIN   GPIO_PIN_1
#define VOLTAGE_SENSE_VBAT_PORT  GPIOA
#define CURRENT_SENSE_U1U2_PIN   GPIO_PIN_4
#define CURRENT_SENSE_U1U2_PORT  GPIOA
#define CURRENT_SENSE_U3U4_PIN   GPIO_PIN_0
#define CURRENT_SENSE_U3U4_PORT  GPIOB

// status defines

#define P_ON 1
#define P_OFF 0

// ProFET params

struct profet_params
{
	//HssType_t       type;           /**< Type of the chosen High side switch */
	const uint16_t  kilis;          /**< Current sense ratio */
	float           iisFault;       /**< Current in case of fault event */
	float           iisOl;          /**< Current in case of an open load */
	float           iisEn;          /**< Leakage current when channel is enabled */
	float           iisO;
	float           vBatOffset;
	float           rSense;
	float			currentOffset;
	float			vss_threshold;
};

// function defines

enum Channel_t
{
	CHANNEL1 	 = 1,
	CHANNEL2 	 = 2,
	CHANNEL3	 = 3,
	CHANNEL4	 = 4,
	CHANNEL_ALL  = 5
};

enum DiagStatus_t
{
    DIAG_READ_ERROR     = 0,   /**< Read Error */
    NOT_ENABLED         = 1,   /**< Diagnosis not enabled */
    NORMAL              = 2,    /**< Switch works correctly */
    FAULT               = 3,    /**< Switch is in fault condition (Is_fault at IS pin), which can mean "Short to GND", "Overtemperature" or "Overload" */
    FAULT_OL_IC         = 4,    /**< Switch is is either in Open Load (whit enable channel) or inverse current is flowing */
    SHORT_TO_GND_OR_OT  = 5,    /**< Short to the ground or Overtemperature detected*/
    SHORT_TO_VSS        = 6,    /**< Short to the supply voltage */
    OPEN_LOAD           = 7,    /**< Open load detected */
	FAULT_VSS			= 8
};

extern enum DiagStatus_t diagStatus;

struct shield_status
{
	uint8_t in1 : 1;
	uint8_t in2 : 1;
	uint8_t in3 : 1;
	uint8_t in4 : 1;
	uint8_t diag1 : 1;
	uint8_t diag2 : 1;
	uint8_t diag3 : 1;
	uint8_t diag4 : 1;
	uint8_t diag_ctrl1 : 1;
	uint8_t diag_ctrl2 : 1;
	uint8_t adc1 : 1;
	uint8_t adc2 : 1;
};



void HSS_Shield_GPIO_Init(void);
void HSS_Shield_ADC_Init(ADC_HandleTypeDef *hadc);

void enable_CH(enum Channel_t ch);
void disable_CH(enum Channel_t ch);
void enable_diag(enum Channel_t ch);
void disable_diag(enum Channel_t ch);

void ADC_Select_CH0(ADC_HandleTypeDef *hadc);
void ADC_Select_CH1(ADC_HandleTypeDef *hadc);
void ADC_Select_CH2(ADC_HandleTypeDef *hadc);
void ADC_Select_CH3(ADC_HandleTypeDef *hadc);

uint16_t analog_read(ADC_HandleTypeDef *hadc,int ch);

float readIs(ADC_HandleTypeDef *hadc,enum Channel_t ch);

float readVss(ADC_HandleTypeDef *hadc);

enum DiagStatus_t diagRead_On(float i_sense, float vss);
enum DiagStatus_t diagRead_Off(float currentOn, float currentOff, float vss);

enum DiagStatus_t diagRead_State(ADC_HandleTypeDef *hadc,enum Channel_t ch);

#endif /* INC_HSS_SHIELD_H_ */
