/**
 * keyMatrix.c
 *
 * Created: 8/15/2014 11:16:16 PM
 *  Author: Levi.Balling
 * This file is to fetch all of the io from the system, and then apply it to a check whether it has been press with a debounce check.
 */ 

#include <asf.h>
#include "keyMatrix.h"
#include <string.h>
//List of output Keys
//0 PD3
//1 PD5
//2 PD7
//3 PA30
//4 PD8
//5 PD9
//6 PD15
//7 PD10
//similar to The LED Pins

/** KEY #0 pin definition . */
#define KEY_OUT_0_NAME      "KEY OUT 0 D3"
#define KEY_OUT_0       {PIO_PD3, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_0_MASK  PIO_PD3
#define KEY_OUT_0_PIO   PIOD
#define KEY_OUT_0_ID    ID_PIOD
#define KEY_OUT_0_TYPE  PIO_OUTPUT_1
#define KEY_OUT_0_ATTR  PIO_DEFAULT

#define KEY_OUT_0_GPIO            (PIO_PD3_IDX)
#define KEY_OUT_0_FLAGS           (0)
#define KEY_OUT_0_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_0_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #1 pin definition . */
#define KEY_OUT_1_NAME      "KEY OUT 1 D5"
#define KEY_OUT_1       {PIO_PD5, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_1_MASK  PIO_PD5
#define KEY_OUT_1_PIO   PIOD
#define KEY_OUT_1_ID    ID_PIOD
#define KEY_OUT_1_TYPE  PIO_OUTPUT_1
#define KEY_OUT_1_ATTR  PIO_DEFAULT

#define KEY_OUT_1_GPIO            (PIO_PD5_IDX)
#define KEY_OUT_1_FLAGS           (0)
#define KEY_OUT_1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #2 pin definition . */
#define KEY_OUT_2_NAME      "KEY OUT 2 D7"
#define KEY_OUT_2       {PIO_PD7, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_2_MASK  PIO_PD7
#define KEY_OUT_2_PIO   PIOD
#define KEY_OUT_2_ID    ID_PIOD
#define KEY_OUT_2_TYPE  PIO_OUTPUT_1
#define KEY_OUT_2_ATTR  PIO_DEFAULT

#define KEY_OUT_2_GPIO            (PIO_PD7_IDX)
#define KEY_OUT_2_FLAGS           (0)
#define KEY_OUT_2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #3 pin definition . */
#define KEY_OUT_3_NAME      "KEY OUT 3 A30"
#define KEY_OUT_3       {PIO_PA30, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_3_MASK  PIO_PA30
#define KEY_OUT_3_PIO   PIOA
#define KEY_OUT_3_ID    ID_PIOA
#define KEY_OUT_3_TYPE  PIO_OUTPUT_1
#define KEY_OUT_3_ATTR  PIO_DEFAULT

#define KEY_OUT_3_GPIO            (PIO_PA30_IDX)
#define KEY_OUT_3_FLAGS           (0)
#define KEY_OUT_3_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_3_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #4 pin definition . */
#define KEY_OUT_4_NAME      "KEY OUT 4 D8"
#define KEY_OUT_4       {PIO_PD8, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_4_MASK  PIO_PD8
#define KEY_OUT_4_PIO   PIOD
#define KEY_OUT_4_ID    ID_PIOD
#define KEY_OUT_4_TYPE  PIO_OUTPUT_1
#define KEY_OUT_4_ATTR  PIO_DEFAULT

#define KEY_OUT_4_GPIO            (PIO_PD8_IDX)
#define KEY_OUT_4_FLAGS           (0)
#define KEY_OUT_4_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_4_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #5 pin definition . */
#define KEY_OUT_5_NAME      "KEY OUT 5 D9"
#define KEY_OUT_5       {PIO_PD9, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_5_MASK  PIO_PD9
#define KEY_OUT_5_PIO   PIOD
#define KEY_OUT_5_ID    ID_PIOD
#define KEY_OUT_5_TYPE  PIO_OUTPUT_1
#define KEY_OUT_5_ATTR  PIO_DEFAULT

#define KEY_OUT_5_GPIO            (PIO_PD9_IDX)
#define KEY_OUT_5_FLAGS           (0)
#define KEY_OUT_5_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_5_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #6 pin definition . */
#define KEY_OUT_6_NAME      "KEY OUT 6 D15"
#define KEY_OUT_6       {PIO_PD15, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_6_MASK  PIO_PD15
#define KEY_OUT_6_PIO   PIOD
#define KEY_OUT_6_ID    ID_PIOD
#define KEY_OUT_6_TYPE  PIO_OUTPUT_1
#define KEY_OUT_6_ATTR  PIO_DEFAULT

#define KEY_OUT_6_GPIO            (PIO_PD15_IDX)
#define KEY_OUT_6_FLAGS           (0)
#define KEY_OUT_6_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_6_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #7 pin definition . */
#define KEY_OUT_7_NAME      "KEY OUT 7 D10"
#define KEY_OUT_7       {PIO_PD10, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_7_MASK  PIO_PD10
#define KEY_OUT_7_PIO   PIOD
#define KEY_OUT_7_ID    ID_PIOD
#define KEY_OUT_7_TYPE  PIO_OUTPUT_1
#define KEY_OUT_7_ATTR  PIO_DEFAULT

#define KEY_OUT_7_GPIO            (PIO_PD10_IDX)
#define KEY_OUT_7_FLAGS           (0)
#define KEY_OUT_7_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_7_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

//Macro to set the pin output
//example of use
//KEY_OUT_ON(KEY_OUT_5)
//#define KEY_OUT_ON(key)      ioport_set_pin_level(KEY_OUT##_GPIO, KEY_OUT##_ACTIVE_LEVEL)
//
//#define KEY_OUT_OFF(key)      ioport_set_pin_level(KEY_OUT##_GPIO, KEY_OUT##_INACTIVE_LEVEL)


//list of input keys
//0 PD11
//1 PD12
//2 PD13
//3 PD16
//4 PA9
//5 PA5
//6 PA27
//7 PD18

//we don't want an interrupt, but we want the pull-up and the debounce

// KEY 0 PD11
/**
 * Key In #0 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_0_NAME        "KEY IN 0 D11"
#define KEY_IN_0 (PIO_PD11_IDX)
#define KEY_IN_0_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_0_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 0
#define KEYIN_0       {PIO_PD11, PIOD, ID_PIOD, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_0_MASK  PIO_PD11
#define KEYIN_0_PIO PIOD
#define KEYIN_0_ID ID_PIOD
#define KEYIN_0_TYPE PIO_INPUT
#define KEYIN_0_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys



// KEY 1 PD12
/**
 * Key input #1 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_1_NAME        "KEY IN 1 D12"
#define KEY_IN_1 (PIO_PD12_IDX)
#define KEY_IN_1_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_1_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 1
#define KEYIN_1       {PIO_PD12, PIOD, ID_PIOD, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_1_MASK  PIO_PD12
#define KEYIN_1_PIO PIOD
#define KEYIN_1_ID ID_PIOD
#define KEYIN_1_TYPE PIO_INPUT
#define KEYIN_1_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys




//2 PD13

// KEY 2 PD12
/**
 * Key input #2 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_2_NAME        "KEY IN 2 D13"
#define KEY_IN_2 (PIO_PD13_IDX)
#define KEY_IN_2_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_2_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 2
#define KEYIN_2       {PIO_PD13, PIOD, ID_PIOD, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_2_MASK  PIO_PD13
#define KEYIN_2_PIO PIOD
#define KEYIN_2_ID ID_PIOD
#define KEYIN_2_TYPE PIO_INPUT
#define KEYIN_2_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys



//3 PD16

// KEY 3 PD16
/**
 * Key In #3 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_3_NAME        "KEY IN 3 D16"
#define KEY_IN_3 (PIO_PD16_IDX)
#define KEY_IN_3_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_3_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 3
#define KEYIN_3       {PIO_PD16, PIOD, ID_PIOD, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_3_MASK  PIO_PD16
#define KEYIN_3_PIO PIOD
#define KEYIN_3_ID ID_PIOD
#define KEYIN_3_TYPE PIO_INPUT
#define KEYIN_3_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys




//4 PA9

// KEY 4 PA9
/**
 * Key In #4 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_4_NAME        "KEY IN 4 A9"
#define KEY_IN_4 (PIO_PA9_IDX)
#define KEY_IN_4_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_4_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 4
#define KEYIN_4       {PIO_PA9, PIOA, ID_PIOA, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_4_MASK  PIO_PA9
#define KEYIN_4_PIO PIOA
#define KEYIN_4_ID ID_PIOA
#define KEYIN_4_TYPE PIO_INPUT
#define KEYIN_4_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys


//5 PA5

// KEY 5 PA5
/**
 * Key In #5 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_5_NAME        "KEY IN 5 A5"
#define KEY_IN_5 (PIO_PA5_IDX)
#define KEY_IN_5_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_5_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 5
#define KEYIN_5       {PIO_PA5, PIOA, ID_PIOA, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_5_MASK  PIO_PA5
#define KEYIN_5_PIO PIOA
#define KEYIN_5_ID ID_PIOA
#define KEYIN_5_TYPE PIO_INPUT
#define KEYIN_5_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys


//6 PA27

// KEY 6 PA27
/**
 * Key In #6 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_6_NAME        "KEY IN 6 A27"
#define KEY_IN_6 (PIO_PA27_IDX)
#define KEY_IN_6_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_6_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 6
#define KEYIN_6       {PIO_PA27, PIOA, ID_PIOA, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_6_MASK  PIO_PA27
#define KEYIN_6_PIO PIOA
#define KEYIN_6_ID ID_PIOA
#define KEYIN_6_TYPE PIO_INPUT
#define KEYIN_6_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys


//7 PD18

// KEY 7 PD18
/**
 * Key In #7 definition. Attributes = pull-up + debounce 
 */
#define KEY_IN_7_NAME        "KEY IN 7 D18"
#define KEY_IN_7 (PIO_PD18_IDX)
#define KEY_IN_7_FLAGS (IOPORT_MODE_PULLUP | IOPORT_MODE_DEBOUNCE)
#define KEY_IN_7_SENSE (IOPORT_SENSE_FALLING)//May not need this.

//Key input 7
#define KEYIN_7       {PIO_PD18, PIOD, ID_PIOD, PIO_INPUT, \
PIO_PULLUP | PIO_DEBOUNCE }

#define KEYIN_7_MASK  PIO_PD18
#define KEYIN_7_PIO PIOD
#define KEYIN_7_ID ID_PIOD
#define KEYIN_7_TYPE PIO_INPUT
#define KEYIN_7_ATTR (PIO_PULLUP | PIO_DEBOUNCE )//only pullup resistor and debounce
//#define PIN_PUSHBUTTON_2_IRQn  PIOA_IRQn//no Interrupt for the keys


//need 2 arrays Inputs and outputs
//inputs are easy
static ioport_pin_t keyInArray[8] = {KEY_IN_0, KEY_IN_1, KEY_IN_2, KEY_IN_3,  
	KEY_IN_4, KEY_IN_5, KEY_IN_6, KEY_IN_7};

// this is an array of all the available keys.
static UKEY buttonPressArray[8][8];

//this is an array of currently pressed keys
static UKEY currentlyPressedKeys[7];

static void checkPins(void);

static void setOuputPin(uint8_t outputKeyIndex);
void initializeKeys(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	
	//set up the keys to work 1 by 1
	//since each key has a unique number we will arrange them individually.
	// this is in colomn row order
	//[outputs][inputs]
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			buttonPressArray[i][j].id = 0;//initializes all of the keys to default
		}
	}
	
	//first row of inputs
	buttonPressArray[0][0].id = 1;
	buttonPressArray[0][1].id = 2;
	buttonPressArray[0][2].id = 3;
	buttonPressArray[0][3].id = 4;
	buttonPressArray[0][4].id = 20;
	buttonPressArray[0][5].id = 21;
	buttonPressArray[0][6].id = 22;
	buttonPressArray[0][7].id = 23;
	
	//Second Row of inputs
	buttonPressArray[1][0].id = 5;
	buttonPressArray[1][1].id = 6;
	buttonPressArray[1][2].id = 7;
	buttonPressArray[1][3].id = 8;
	buttonPressArray[1][4].id = 9;
	buttonPressArray[1][6].id = 24;
	buttonPressArray[1][7].id = 25;
	
	//Third Row of inputs
	buttonPressArray[2][0].id = 10;
	buttonPressArray[2][1].id = 11;
	buttonPressArray[2][2].id = 12;
	buttonPressArray[2][3].id = 13;
	buttonPressArray[2][4].id = 14;
	buttonPressArray[2][6].id = 26;
	buttonPressArray[2][7].id = 27;
	
	//Fourth Row of inputs
	buttonPressArray[3][0].id = 15;
	buttonPressArray[3][1].id = 16;
	buttonPressArray[3][2].id = 17;
	buttonPressArray[3][3].id = 18;
	buttonPressArray[3][4].id = 19;
}

static void checkPins(void)
{
	uint8_t inputsIndex;
	uint8_t outputIndex;
	//this is a 8 x 8 matrix of keys 
	//the pins that are going to be the outputs are
	
	//loop through all of the keys
	//set output then check all of the inputs
	for(outputIndex = 0; outputIndex < 8; outputIndex++)//number of outputs
	{
		//set output low here
		setOuputPin(outputIndex);
		for(inputsIndex = 0; inputsIndex < 8; inputsIndex++)//number of inputs
		{
			//get all of the inputs
			//buttons are pulled down when pressed
			buttonPressArray[outputIndex][inputsIndex].keyState =  !ioport_get_pin_level(keyInArray[inputsIndex]);
			if(buttonPressArray[outputIndex][inputsIndex].keyState)
			{
				buttonPressArray[outputIndex][inputsIndex].checkCount++;
				//just pressed
				if(buttonPressArray[outputIndex][inputsIndex].checkCount == 1)
				{
					buttonPressArray[outputIndex][inputsIndex].pressState = KEY_PRESSED;
					
				}
				else if(buttonPressArray[outputIndex][inputsIndex].checkCount > 1)
				{
					buttonPressArray[outputIndex][inputsIndex].pressState = KEY_HELD;
					
				}
				
				
			}
			else
			{
				//Key is released
				if(buttonPressArray[outputIndex][inputsIndex].checkCount != 0)
				{
					buttonPressArray[outputIndex][inputsIndex].pressState = KEY_RELEASED;
					buttonPressArray[outputIndex][inputsIndex].checkCount = 0;
				}
				else
				{			
					//don't do anything with the key
					buttonPressArray[outputIndex][inputsIndex].pressState = NO_KEY_STATE;
				}
				
			}
		}	
	}
}

/**
* Sets the keyoutput based on the parameter. and clears all of the other keys
*
**/
static void setOuputPin(uint8_t outputKeyIndex)
{
	//clear all of the outputs to be low
	ioport_set_pin_level(KEY_OUT_0_GPIO, KEY_OUT_0_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_1_GPIO, KEY_OUT_1_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_2_GPIO, KEY_OUT_2_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_3_GPIO, KEY_OUT_3_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_4_GPIO, KEY_OUT_4_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_5_GPIO, KEY_OUT_5_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_6_GPIO, KEY_OUT_6_ACTIVE_LEVEL);
	ioport_set_pin_level(KEY_OUT_7_GPIO, KEY_OUT_7_ACTIVE_LEVEL);

	//takes the K
	switch(outputKeyIndex)
	{
		case 0:
		ioport_set_pin_level(KEY_OUT_0_GPIO, KEY_OUT_0_INACTIVE_LEVEL);
		break;
		case 1:
		ioport_set_pin_level(KEY_OUT_1_GPIO, KEY_OUT_1_INACTIVE_LEVEL);
		break;
		case 2:
		ioport_set_pin_level(KEY_OUT_2_GPIO, KEY_OUT_2_INACTIVE_LEVEL);
		break;
		case 3:
		ioport_set_pin_level(KEY_OUT_3_GPIO, KEY_OUT_3_INACTIVE_LEVEL);
		break;
		case 4:
		ioport_set_pin_level(KEY_OUT_4_GPIO, KEY_OUT_4_INACTIVE_LEVEL);
		break;
		case 5:
		ioport_set_pin_level(KEY_OUT_5_GPIO, KEY_OUT_5_INACTIVE_LEVEL);
		break;
		case 6:
		ioport_set_pin_level(KEY_OUT_6_GPIO, KEY_OUT_6_INACTIVE_LEVEL);
		break;
		case 7:
		ioport_set_pin_level(KEY_OUT_7_GPIO, KEY_OUT_7_INACTIVE_LEVEL);
		break;
		default:
		break;
	}
	
}

//up to 10 keys at one time
void getActiveKeys(UKEY *selectedKeys, uint8_t *size)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t keyIndex = 0;
	
	//this will be call regularly
	checkPins();
	selectedKeys = currentlyPressedKeys;
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			//key if the key has changed states, if so load the key into the system.
			if(buttonPressArray[i][j].keyState !=  NO_KEY_STATE)
			{
				memcpy(&currentlyPressedKeys[keyIndex],&buttonPressArray[i][j], sizeof(UKEY));
				keyIndex++;
				*size = keyIndex;
			}
			
			//no more room for any more keys
			if(keyIndex==7)
			{
				return;
			}	
		}
	}
	
}
