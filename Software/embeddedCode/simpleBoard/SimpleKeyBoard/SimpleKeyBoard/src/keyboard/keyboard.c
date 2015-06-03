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
static KEYBOARD_OBJ rightKeyboardObj;
#define KEY_DELAY_MS    1


static void wakeUpKeyboard(void);


//it is a 6 x 7 matrixst


//programming davorak board
//$&[{}(
//tab;,.py
//caps aoeui
//shift 'qjkx
//cntrl alt windows

//space back space 

static uint8_t keyArray[NUM_OF_KEY_ROWS][NUM_OF_KEY_COLUMNS] = {
	HID_SPACEBAR, HID_DELETE, HID_BACKSPACE, HID_SPACEBAR, HID_PAGEDOWN, HID_J,
	HID_PAGEUP, 
	HID_P, HID_6, HID_7, HID_8, HID_9, HID_0,
	HID_I, HID_O, 
	HID_Z, HID_X, HID_C, HID_V, HID_B, HID_U,
	HID_TAB, HID_A, HID_S, HID_D, HID_F, HID_G,	HID_T,
	HID_TAB/**TODO change to be Alt **/, HID_Q, HID_W, HID_E, HID_R, HID_T, HID_CAPS_LOCK,
	HID_TILDE, HID_1, HID_2, HID_3, HID_4, HID_5, HID_ESCAPE,
};

//create the left hand keyboard here
static uint8_t keyArray2[NUM_OF_KEY_ROWS][NUM_OF_KEY_COLUMNS] = { 
	HID_BACKSPACE, HID_DELETE, HID_END, 
	0, 0, HID_HOME,
	HID_SLASH, HID_TILDE,
	HID_6, HID_7, HID_8, HID_9, HID_0,
	0, 0, HID_Z, HID_X, HID_C, HID_V, HID_B, 1,
	 
	HID_TAB, HID_A, HID_S, HID_D, HID_F, HID_G,
	HID_BACKSLASH, 
	HID_TAB, HID_Q, HID_W, HID_E, HID_R, HID_T, 1,
	HID_PLUS, HID_1, HID_2, HID_3, HID_4, HID_5, HID_ESCAPE,
};
		// left hand
		//KEY_Equal_Plus,	KEY_1_Exclamation,	KEY_2_At,	KEY_3_Pound,	KEY_4_Dollar,	KEY_5_Percent,	KEY_Escape,
		//KEY_Backslash_Pipe,	KEY_q_Q,	KEY_w_W,	KEY_e_E,	KEY_r_R,	KEY_t_T,	1,
		//KEY_Tab,	KEY_a_A,	KEY_s_S,	KEY_d_D,	KEY_f_F,	KEY_g_G,
		//KEY_LeftShift,	KEY_z_Z,	KEY_x_X,	KEY_c_C,	KEY_v_V,	KEY_b_B,	1,
		//KEY_LeftGUI,	KEY_GraveAccent_Tilde,	KEY_Backslash_Pipe,	KEY_LeftArrow,	KEY_RightArrow,
		//KEY_LeftControl,	KEY_LeftAlt,
		//0,	0,	KEY_Home,
		//KEY_DeleteBackspace,	KEY_DeleteForward,	KEY_End,


//ASCII ART

// ___________________________________________                             ___________________________________________
// |  1  |  2  |  3  |  4  |  5  |  6  |  7  |                             |  45 |  44 |  43 |  42 |  41 |  40 |  39 |
// -------------------------------------------                             -------------------------------------------
// |  8  |  9  |  10 |  11 |  12 |  13 |     |                             |     |  51 |  50 |  49 |  48 |  47 |  46 |
// ------------------------------------| 14  |                             | 52  |------------------------------------
// |  15 |  16 |  17 |  18 |  19 |  20 |_____|                             |_____|  58 |  57 |  56 |  55 |  54 |  53 |
// ------------------------------------|     |                             |     |------------------------------------
// |  21 |  22 |  23 |  24 |  25 |  26 | 27  |  __________   __________    |  65 |  64 |  63 |  62 |  61 |  60 |  59 |
// ------------------------------------------- / 38 / 37 /   \ 75 \ 76 \   -------------------------------------------
//  | 28 |  29 |  30 |  31 |  32 |       _____/____/____/     \____\____\_____         |  70 |  69 |  68 |  67 | 66 |
//  ------------------------------      /    /    / 36 /       \ 74 \    \    \        ------------------------------
//                                     / 33 / 34 /____/         \____\ 72 \ 71 \ 
//                                    /    /    / 35 /           \ 73 \    \    \
//                                   /____/____/____/             \____\____\____\


void initKeys(void)
{
	//here we will setup the keys for the specific boards
	
}

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

	// Right Hand
	//outputs
	rightKeyboardObj.rowIOArr[0] = R_ROW_0;
	rightKeyboardObj.rowIOArr[1] = R_ROW_1;
	rightKeyboardObj.rowIOArr[2] = R_ROW_2;
	rightKeyboardObj.rowIOArr[3] = R_ROW_3;
	rightKeyboardObj.rowIOArr[4] = R_ROW_4;
	rightKeyboardObj.rowIOArr[5] = R_ROW_5;

	//input
	rightKeyboardObj.columnIOArr[0] = R_COLUMN_0;
	rightKeyboardObj.columnIOArr[1] = R_COLUMN_1;
	rightKeyboardObj.columnIOArr[2] = R_COLUMN_2;
	rightKeyboardObj.columnIOArr[3] = R_COLUMN_3;
	rightKeyboardObj.columnIOArr[4] = R_COLUMN_4;
	rightKeyboardObj.columnIOArr[5] = R_COLUMN_5;
	rightKeyboardObj.columnIOArr[6] = R_COLUMN_6;

//interrupts
// pin is pulled down so when we go to sleep mode we
// Set all of the output pins high, and
// set the interrupt to trigger when the pin goes high.

	
	struct extint_chan_conf *config_extint_chan;
	uint8_t eicLine = 0;


//Currently we have disabled all interrupts
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
		KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
		row->rowIO = keyboardObj.rowIOArr[i];
		rightRow->rowIO = rightKeyboardObj.rowIOArr[i];
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			KEY_OBJ *rightKey = &rightRow->keys[j];
			key->columnIO = keyboardObj.columnIOArr[j];
			key->currentState = false;
			key->previousState = false;
			key->hidKeyMod = 0;
			key->hidKey = keyArray[i][j];//   HID_A+j+i;//no HID Key
			key->keyJustDown = false;
			key->keyJustRelease = false;
			key->specialKey = 0;//for later use
			key->keyHoldCount = 0;

			//right key
			rightKey->columnIO = rightKeyboardObj.columnIOArr[j];
			rightKey->currentState = false;
			rightKey->previousState = false;
			rightKey->hidKeyMod = 0;
			rightKey->hidKey = keyArray[i][j];//   HID_A+j+i;//no HID Key
			rightKey->keyJustDown = false;
			rightKey->keyJustRelease = false;
			rightKey->specialKey = 0;//for later use
			rightKey->keyHoldCount = 0;
			
			ioport_set_pin_dir(key->columnIO, IOPORT_DIR_INPUT);
			ioport_set_pin_mode(key->columnIO, IOPORT_MODE_PULLDOWN);
			
			ioport_set_pin_dir(rightKey->columnIO, IOPORT_DIR_INPUT);
			ioport_set_pin_mode(rightKey->columnIO, IOPORT_MODE_PULLDOWN);
	}
	ioport_set_pin_dir(row->rowIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(row->rowIO, false);//set the pin low
	
	ioport_set_pin_dir(rightRow->rowIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(rightRow->rowIO, false);//set the pin low
		
	}
	
}

uint8_t checkKeys(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
		KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
		
		ioport_set_pin_level(row->rowIO, true);
		ioport_set_pin_level(rightRow->rowIO, true);
		
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			KEY_OBJ *rightKey = &rightRow->keys[j];
				
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
			
			//Right Hand check
			if(rightKey->currentState == true && rightKey->previousState == true)
			{
				rightKey->keyHoldCount++;
				//check if just pressed is true
				if(rightKey->keyHoldCount == 1)
				{
					rightKey->keyJustDown = true;
				}
				else
				{
					rightKey->keyJustDown = false;
								
				}
			}
			else if(rightKey->currentState == false && rightKey->previousState == false)
			{
				//check that there is a keyHold Count
				if (rightKey->keyHoldCount != 0 && rightKey->keyJustRelease == false)
				{
					rightKey->keyJustRelease = true;
								
				}
				else
				{
					rightKey->keyJustRelease = false;
					rightKey->keyHoldCount = 0;
				}
			}
			else
			{
				rightKey->keyJustRelease = false;
				rightKey->keyJustDown = false;
			}
		}
		
		ioport_set_pin_level(row->rowIO, false);//set the pin low
		ioport_set_pin_level(rightRow->rowIO, false);//set the pin low
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
			KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
			
			for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
			{
				KEY_OBJ *key = &row->keys[j];
				KEY_OBJ *rightKey = &rightRow->keys[j];
				//check if key is just pressed
				if(key->keyJustDown)
				{
					//returns an array of pointers to the key
					keyArr[numOfKeyPressed++] = key;
				}
				
				if(rightKey->keyJustDown)
				{
					//returns an array of pointers to the key
					keyArr[numOfKeyPressed++] = rightKey;
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
		KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
	
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			KEY_OBJ *rightKey = &rightRow->keys[j];
			//check if key is just pressed
			if(key->keyJustRelease)
			{
				//returns an array of pointers to the key
				keyArr[numOfKeyReleased++] = key;
			}
			
			if(rightKey->keyJustRelease)
			{
				//returns an array of pointers to the key
				keyArr[numOfKeyReleased++] = rightKey;
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