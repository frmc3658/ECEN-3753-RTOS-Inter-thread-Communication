/*
 * app.h
 *
 *  Created on: Jan 31, 2024
 *      Author: Frank McDermott
 */

#ifndef INC_APP_H
#define INC_APP_H

/************************************
 * 		Header Includes		*
 ************************************/
/* C-STD Headers */
#include <limits.h>
/* Core System Headers */
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
/* User Headers */
#include "Gyro_Driver.h"


/************************************
 * 		Compile-time Switches		*
 ************************************/


/************************************
 * 		Macro Definitions			*
 ************************************/
/* GPIO Definitions */
#define BUTTON_PIN					GPIO_PIN_0
#define BUTTON_PORT					GPIOA
#define BUTTON_IRQn					EXTI0_IRQn
#define RED_LED_PIN					GPIO_PIN_14
#define RED_LED_PORT				GPIOG
#define GREEN_LED_PIN				GPIO_PIN_13
#define GREEN_LED_PORT				GPIOG
/* Timer Definitions */
#define APP_TIMER_TICKS_100MS		(uint32_t)100
/* Event Flag Definitions */
#define BUTTON_EVENT_FLAG			(uint32_t)0x01
/* Message Queue Definitions */
#define MAX_MSG_COUNT				(uint32_t)8	/* Arbitrarily chose a max message count of 8 */
#define MAX_MSG_SIZE_BYTE			(uint32_t)(sizeof(struct Message_t))
#define DEFAULT_MSG_PRIORITY		(uint8_t)0
/* Semaphore Definitions */
#define MAKE_BINARY_SEMAPHORE		1	/* A max count value of 1 creates a binary semaphore */
#define SEMAPHORE_ONE_INIT_TOKEN	1	/* Initialize sempahore token count to zero; timer will release it */

/************************************
 * 			Enumerations			*
************************************/

typedef enum
{
	counterClockwiseFast 	= -15000,	/* Faster counter-clockwise (-) rotation */
	counterClockwiseSlow 	= -2000,	/* Slow but affirmative counter-clockwise (-) rotation*/
	nearlyZero 				= 0,		/* Nearly zero clockwise (+) rotation */
	clockwiseSlow 			= 2000,		/* Slow but affirmative clockwise (+) rotation */
	clockwiseFast 			= 15000,	/* Faster clockwise (+) rotation */
}gyroRotationRate;


/************************************
 * 		Function Prototypes			*
 ************************************/

void appInit(void);


#endif /* INC_APP_H */
