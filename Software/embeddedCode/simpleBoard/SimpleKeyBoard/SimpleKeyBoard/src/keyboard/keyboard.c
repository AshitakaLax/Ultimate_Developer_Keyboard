/*
 * keyboard.c
 *
 * Created: 5/7/2015 6:46:27 PM
 *  Author: Levi.Balling
 */ 

#include <asf.h>
#include <ioport.h>
#include <port.h>
#include "keyboard.h"
#include "KeyIO.h"
#include <delay.h>
#include <usb_protocol_hid.h>

static KEYBOARD_OBJ keyboardObj;
#define KEY_DELAY_MS    1


static void wakeUpKeyboard(void);


void initKeyBoard(void)
{
	//configure pin PB10 as output High
	bool tempPinValue = false;	
	//configure pin PB06 as input
	uint8_t i = 0;
	uint8_t j = 0;
	//setup data structure
	
//outputs
	keyboardObj.rowIOArr[0] = ROW_0;
	keyboardObj.rowIOArr[1] = ROW_1;
	keyboardObj.rowIOArr[2] = ROW_2;
	keyboardObj.rowIOArr[3] = ROW_3;
	keyboardObj.rowIOArr[4] = ROW_4;
	keyboardObj.rowIOArr[5] = ROW_5;

//input
	keyboardObj.columnIOArr[0] = COLUMN_0;
	keyboardObj.columnIOArr[1] = COLUMN_1;
	keyboardObj.columnIOArr[2] = COLUMN_2;
	keyboardObj.columnIOArr[3] = COLUMN_3;
	keyboardObj.columnIOArr[4] = COLUMN_4;
	keyboardObj.columnIOArr[5] = COLUMN_5;
	keyboardObj.columnIOArr[6] = COLUMN_6;

//interrupts
// pin is pulled down so when we go to sleep mode we
// Set all of the output pins high, and
// set the interrupt to trigger when the pin goes high.

	
	struct extint_chan_conf *config_extint_chan;
	uint8_t eicLine = 0;


	for(i = 0; i < 0; i++)
	{
		config_extint_chan = &keyboardObj.configExtintChanColumns[i];

		extint_chan_get_config_defaults(config_extint_chan);
		config_extint_chan->gpio_pin_pull       = EXTINT_PULL_DOWN;
		config_extint_chan->filter_input_signal = true;
		config_extint_chan->detection_criteria  = EXTINT_DETECT_RISING;

		if(i == 0)
		{
			config_extint_chan->gpio_pin            = COLUMN_0_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_0_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_0_EIC_LINE;
		}
		else if(i == 1)
		{
			config_extint_chan->gpio_pin            = COLUMN_1_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_1_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_1_EIC_LINE;
		}
		else if(i == 2)
		{
			config_extint_chan->gpio_pin            = COLUMN_2_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_2_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_2_EIC_LINE;
		}
		else if(i == 3)
		{
			config_extint_chan->gpio_pin            = COLUMN_3_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_3_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_1_EIC_LINE;
		}
		else if(i == 4)
		{
			config_extint_chan->gpio_pin            = COLUMN_4_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_4_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_1_EIC_LINE;
		}
		else if(i == 5)
		{
			config_extint_chan->gpio_pin            = COLUMN_5_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_5_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_5_EIC_LINE;
		}
		else if(i == 6)
		{
			config_extint_chan->gpio_pin            = COLUMN_6_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_6_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_6_EIC_LINE;
		}
	
		
		extint_chan_set_config(eicLine, config_extint_chan);
		extint_register_callback(wakeUpKeyboard, eicLine, EXTINT_CALLBACK_TYPE_DETECT);
		extint_chan_enable_callback(eicLine, EXTINT_CALLBACK_TYPE_DETECT);
		
	}


//setup all of the pins

	ioport_init();
	delay_init();
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
		row->rowIO = keyboardObj.rowIOArr[i];
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			key->columnIO = keyboardObj.columnIOArr[j];
			key->currentState = false;
			key->previousState = false;
			key->hidKeyMod = HID_MODIFIER_LEFT_SHIFT;
			key->hidKey = HID_A+j+i;//no HID Key
			key->keyJustDown = false;
			key->keyJustRelease = false;
			key->specialKey = 0;//for later use
			key->keyHoldCount = 0;
			
			ioport_set_pin_dir(key->columnIO, IOPORT_DIR_INPUT);
			ioport_set_pin_mode(key->columnIO, IOPORT_MODE_PULLDOWN);
		}
		ioport_set_pin_dir(row->rowIO, IOPORT_DIR_OUTPUT);
		ioport_set_pin_level(row->rowIO, false);//set the pin low
		
	}
	
}

uint8_t checkKeys(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
		
		ioport_set_pin_level(row->rowIO, true);
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
		
			key->previousState = key->currentState;
			key->currentState = ioport_get_pin_level(key->columnIO);
			//delay_ms(KEY_DELAY_MS);
			//check if there is a just pressed/or just released
			if(key->currentState == true && key->previousState == true)
			{
				key->keyHoldCount++;
				//check if just pressed is true
				if(key->keyHoldCount == 1)
				{
					key->keyJustDown = true;
				}
				else
				{
					key->keyJustDown = false;
					
				}
			}
			else if(key->currentState == false && key->previousState == false)
			{
				//check that there is a keyHold Count
				if (key->keyHoldCount != 0 && key->keyJustRelease == false)
				{
					key->keyJustRelease = true;
					
				}
				else
				{
					key->keyJustRelease = false;
					key->keyHoldCount = 0;
				}
			}
			else
			{
				key->keyJustRelease = false;
				key->keyJustDown = false;
			}
		}
		ioport_set_pin_level(row->rowIO, false);//set the pin low
	}
	return 0;
}


uint8_t getJustPressedKeys(KEY_OBJ **keyArr)
{
		uint8_t i = 0;
		uint8_t j = 0;
		uint8_t numOfKeyPressed = 0;
		for(i=0; i < NUM_OF_KEY_ROWS; i++)
		{
			KEY_ROW *row = &keyboardObj.keyRows[i];
			
			for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
			{
				KEY_OBJ *key = &row->keys[j];
				//check if key is just pressed
				if(key->keyJustDown)
				{
					//returns an array of pointers to the key
					keyArr[numOfKeyPressed++] = key;
				}
			}
		}
	return numOfKeyPressed;
}

uint8_t getJustReleaseKeys(KEY_OBJ **keyArr)
{

	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t numOfKeyReleased = 0;
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
	
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			//check if key is just pressed
			if(key->keyJustRelease)
			{
				//returns an array of pointers to the key
				keyArr[numOfKeyReleased++] = key;
			}
		}
	}
	return numOfKeyReleased;
}

//
// puts the keyboard into sleep mode and enable the external pins to wakeup
//
void KeyboardSleepMode(void)
{
	//disable all of the pins
	
}

//
// puts the keyboard into Running mode and Disable the external pins to wakeup
//
void KeyboardWakeupMode(void)
{
	
}

static void wakeUpKeyboard(void)
{
	
	//udc_remotewakeup();//wakeup the keyboard if it was in sleep mode.
	
	
}