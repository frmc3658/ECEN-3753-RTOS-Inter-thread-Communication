/*
 * app.c
 *
 *  Created on: Jan 31, 2024
 *      Author: Frank McDermott
 */

#include "app.h"


/************************************
 * 			Static Variables		*
 ************************************/

static struct Message_t
{
	GPIO_PinState 		buttonState;
	gyroRotationRate 	rotation;
}appMessage;


/* Static Variables */
volatile static uint8_t ledInfoEventFlagGroup;

/* Static Timer */
static osTimerId_t appTimerID;
static const osTimerAttr_t appTimerAttr = { .name = "appTimer"};
volatile static osStatus_t appTimerStatus;

/* Static Semaphore */
static osSemaphoreId_t gyroInputSemaphorID;
static const osSemaphoreAttr_t gyroInputSemaphorAttr = { .name = "gyroInputSemaphore" };

/* Static Event Flag */
static osEventFlagsId_t buttonEventFlagID;
static const osEventFlagsAttr_t buttonEventFlagAttr = { .name = "buttonEventFlagAttr" };

/* Static Message Queue */
static osMessageQueueId_t ledInfoMsgQueueID;
static const osMessageQueueAttr_t ledInfoMsgQueueAttr = { .name = "ledInfoMsgQueue" };

/* Static Tasks */
static osThreadId_t gyroInputTask;
static const osThreadAttr_t gyroInputTaskAttr = { .name = "gyroInputTask" };

static osThreadId_t buttonInputTask;
static const osThreadAttr_t buttonInputTaskAttr = { .name = "buttonInputTask" };

static osThreadId_t ledOutputTask;
static const osThreadAttr_t ledOutputTaskAttr = { .name = "ledOutputTask" };

/************************************
 * 	Static Function Prototypes		*
 ************************************/
/* Static Callback functions */
static void appTimerCallback(void* arg);

/* Static App Functions */


/* Static Task Functions */
static void gyroInput(void* arg);
static void buttonInput(void* arg);
static void ledOutput(void* arg);

/* Static Functions */
static void sampleUserButton(void);
static gyroRotationRate getGyroRateOfRotation(void);
static void driveLEDs(struct Message_t msg);


/************************************
 * 		Function Definitions		*
 ************************************/


static void appTimerCallback(void* arg)
{
	// Avoids compiler warning for unsed parameter
	(void) &arg;

	// On timer tick, release the semaphore
	osStatus_t semaphoreStatus = osSemaphoreRelease(gyroInputSemaphorID);

	// Verify that status of the release
	switch(semaphoreStatus)
	{
		case osErrorResource:
		case osErrorParameter:
			// Spin forever on error
			while(1){}
			break;
		default: /* osOK */
			// Token has been released and count incremented
			break;
	}
}

/****************************************
 * 		Function Definitions: Tasks		*
 ****************************************/

static void gyroInput(void* arg)
{
	// Avoids compiler warning for unsed parameter
	(void) &arg;

	osStatus_t gyroMessagePutStatus;

	while(1)
	{
		// Pend on the gyroInputSemaphore
		osStatus_t semaphoreStatus = osSemaphoreAcquire(gyroInputSemaphorID, osWaitForever);

		switch(semaphoreStatus)
		{
			// Possible error codes
			case osErrorTimeout:
			case osErrorResource:
			case osErrorParameter:
				// Spin on error
				while(1){}
				break;
			default: /* osOK */
				// The token has been obtained and token count decremented
				break;
		}

		appMessage.rotation = getGyroRateOfRotation();

		gyroMessagePutStatus = osMessageQueuePut(ledInfoMsgQueueID, &appMessage,
						  	  	  	  	  	  	 DEFAULT_MSG_PRIORITY, osWaitForever);

		switch(gyroMessagePutStatus)
		{
			// Possible error codes
			case osErrorTimeout:
			case osErrorResource:
			case osErrorParameter:
				// Spin on error
				while(1){}
				break;
			default: /* osOK */
				// The token has been obtained and token count decremented
				break;
		}
	}
}


static void buttonInput(void* arg)
{
	// Avoids compiler warning for unsed parameter
	(void) &arg;

	osStatus_t buttonMessagePutStatus;


	while(1)
	{
		// Wait for the button event flag to be set
		uint32_t flags = osEventFlagsWait(buttonEventFlagID, BUTTON_EVENT_FLAG,
										  osFlagsWaitAll, osWaitForever);

		// Verify that the flags set aren't errors
		switch(flags)
		{
			// Possible Error codes
			case osFlagsErrorUnknown:
			case osFlagsErrorTimeout:
			case osFlagsErrorResource:
			case osFlagsErrorParameter:
				// Spin on error
				while(1){}
				break;
			default: /* Event flag set: 0x01 */
				break;
		}


		// Sample the user button
		// NOTE: Updates the message queue with the button state
		sampleUserButton();

		// Put the message into the queue
		buttonMessagePutStatus = osMessageQueuePut(ledInfoMsgQueueID, &appMessage, DEFAULT_MSG_PRIORITY, osWaitForever);

		switch(buttonMessagePutStatus)
		{
			// Possible error codes
			case osErrorTimeout:
			case osErrorResource:
			case osErrorParameter:
				// Spin on error
				while(1){}
				break;
			default: /* osOK */
				// The token has been obtained and token count decremented
				break;
		}
	}
}


static void ledOutput(void* arg)
{
	// Avoids compiler warning for unsed parameter
	(void) &arg;

	// Buffer to store the message retrieved from the message queue
	struct Message_t ledOutputMessage;
	osStatus_t messageStatus;

	//

	while(1)
	{
		// Get the next message from the message queue
		messageStatus = osMessageQueueGet(ledInfoMsgQueueID, &ledOutputMessage,
						  NULL, osWaitForever);

		// Verify that the message was retrieved sucessfully
		switch(messageStatus)
		{
			case osErrorTimeout:
			case osErrorResource:
			case osErrorParameter:
				// Spin on error
				while(1){}
				break;
			default: /* osOK */
				driveLEDs(ledOutputMessage);
				break;
		}
	}
}


/************************************************
 * 		Function Definitions: App Functions		*
 ************************************************/

/*
 *
 *
 */
void appInit(void)
{
	// Create a nw timer
	appTimerID = osTimerNew(appTimerCallback, osTimerPeriodic, NULL, &appTimerAttr);


	// Verify that the timer was created properly
	if(appTimerID == NULL)
	{
		while(1){}
	}

	// Start the OS Timer
	appTimerStatus = osTimerStart(appTimerID, APP_TIMER_TICKS_100MS);

	if(appTimerStatus != osOK)
	{
		while(1){}
	}

	// Create a new semaphore
	gyroInputSemaphorID = osSemaphoreNew(MAKE_BINARY_SEMAPHORE, SEMAPHORE_ONE_INIT_TOKEN, &gyroInputSemaphorAttr);

	// Verify that the semaphore was created properly
	if(gyroInputSemaphorID == NULL)
	{
		while(1){}
	}

	buttonEventFlagID = osEventFlagsNew(&buttonEventFlagAttr);

	if(buttonEventFlagID == NULL)
	{
		while(1){}
	}

	ledInfoMsgQueueID = osMessageQueueNew(MAX_MSG_COUNT, MAX_MSG_SIZE_BYTE, &ledInfoMsgQueueAttr);

	if(ledInfoMsgQueueID == NULL)
	{
		while(1){}
	}

	gyroInputTask = osThreadNew(gyroInput, NULL, &gyroInputTaskAttr);

	if(gyroInputTask == NULL)
	{
		while(1){}
	}

	buttonInputTask = osThreadNew(buttonInput, NULL, &buttonInputTaskAttr);


	if(buttonInputTask == NULL)
	{
		while(1){}
	}


	ledOutputTask = osThreadNew(ledOutput, NULL, &ledOutputTaskAttr);

	if(ledOutputTask == NULL)
	{
		while(1){}
	}
}



/*
 *  @brief Sample User Button
 *  @retval none
 */
void sampleUserButton(void)
{
	// Sample current button state
	appMessage.buttonState = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);
}


/*
 *  @brief Get Gyro Rate of Rotation
 *  @retval gyroRate
 * */
static gyroRotationRate getGyroRateOfRotation(void)
{
	// Variable to store and return the gyro rotation rate
	gyroRotationRate gyroRate;

	// Get the gyro velocity
	int16_t rawVelocity = Gyro_Get_Velocity();

	// Set the gyro rate based upon where the raw value
	// of the gyro velocity fits within the enumerated
	// gyroRotationRate ranges:
	// 		velocity <= -15000 			= counterClockwiseFast
	//		-15000 < velocity <= -2000 	= counterClockwiseSlow
	//		-2000 < velocity < 2000		= nearlyZero (treated as clockwise)
	//		150 <= velocity < 15000		= clockwiseSlow
	//		velocity >= 15000			= clockwiseFast
	if(rawVelocity <= counterClockwiseFast)
	{
		gyroRate = counterClockwiseFast;
	}
	else if(rawVelocity <= counterClockwiseSlow)
	{
		gyroRate = counterClockwiseSlow;
	}
	else if(rawVelocity < clockwiseSlow)
	{
		gyroRate = nearlyZero;
	}
	else if(rawVelocity < clockwiseFast)
	{
		gyroRate = clockwiseSlow;
	}
	else // rawVelocity > clockwiseFast
	{
		gyroRate = clockwiseFast;
	}

	return gyroRate;
}


/*
 * @brief	Drive the User LEDs based on button and gyro inputs
 * @retval	None
 */
void driveLEDs(struct Message_t msg)
{
	// Drive green LED if button is pressed or gyro is rotating counter-clockwise
	if((msg.buttonState == GPIO_PIN_SET) || (msg.rotation <= counterClockwiseSlow))
	{
		HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_SET);
	}
	// ... Otherwise turn off green LED
	else
	{
		HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);
	}

	// Drive red LED if button is pressed and gyro is rotating clockwise
	if((msg.buttonState == GPIO_PIN_SET) && (msg.rotation > counterClockwiseSlow))
	{
		HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_SET);
	}
	// ... Otherwise turn off red LED
	else
	{
		HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);
	}
}


/************************************************
 * 		Function Definitions: IRQ Handlers		*
 ************************************************/


/*
 * @brief User Button (GPIO) ISR which samples and set the buttonState variable
 * @retval none
 */
void EXTI0_IRQHandler(void)
{
	// Disable interrupts
	HAL_NVIC_DisableIRQ(BUTTON_IRQn);

	// Set the button event flag
	osEventFlagsSet(buttonEventFlagID, BUTTON_EVENT_FLAG);

	// Clear interuppt flag
	__HAL_GPIO_EXTI_CLEAR_IT(BUTTON_PIN);

	// Re-enable interrupts
	HAL_NVIC_EnableIRQ(BUTTON_IRQn);
}
