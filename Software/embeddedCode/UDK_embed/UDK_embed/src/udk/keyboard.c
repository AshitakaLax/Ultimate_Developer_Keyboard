/*
 * keyboard.c
 *
 * Created: 12/12/2014 8:49:02 AM
 *  Author: Levi.Balling
 */ 

#include <asf.h>
#include "keyboard.h"
#include "ioport.h"


static KEY_BOARD_OBJ keyBoard;
//add each the number of keys per row
static uint8_t row0Keys = 3;//we currently have Rows 3,4,5
static uint8_t rowOffset = 3;//row offset
static uint8_t columns = 6;
#define NUMBER_OF_KEY_DATA  10

static HID_KEY_DATA downKeys[NUMBER_OF_KEY_DATA];//this is all of the keys that are currently being pressed
static HID_KEY_DATA releasedKeys[NUMBER_OF_KEY_DATA];//this is all of the keys that are currently being pressed

static HID_KEY_DATA hidMapping[18];


static void clearDownKeys(void);
static void clearReleasedKeys(void);

static void setupMapping(void)
{
	uint8_t index = 0;
	//setup the 18 keys
	//key 0
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_A;

	//key 1
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_O;

	//key 2
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_E;

	//key 3
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_U;

	//key 4
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_I;

	//key 5
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_QUOTE;

	//key 6
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_Q;

	//key 7
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_J;

	//key 8
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_K;

	//key 9
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_X;

	//key 10
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_ASH;

	//key 11
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_COMMA;

	//key 12
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_DOT;

	//key 13
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_P;

	//key 14
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_Y;

	//key 15
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_L;

	//key 16
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_R;

	//key 17
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_C;
}


/**
 * Initializes all of the keyboard keys
 */
void initKeyboard(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t index = 0;
	setupMapping();
	keyBoard.keyIsDown = false;
	keyBoard.keysPressed = 0;

	for(i = 0; i < row0Keys; i++)
	{

		keyBoard.rows[i].row = (KEY_OUTPUT)(KEY_OUT_0 + i+ rowOffset);

		//outputs
		ioport_set_pin_dir(keyBoard.rows[i].row, IOPORT_DIR_OUTPUT);
		ioport_set_pin_level(keyBoard.rows[i].row, IOPORT_PIN_LEVEL_LOW);//all low except for the active row

		for(j = 0; j < columns; j++)
		{
			keyBoard.rows[i].rowOfKeys[j].column = ((KEY_INPUT)KEY_IN_0 + j);
			keyBoard.rows[i].rowOfKeys[j].keyIsDown = false;

			keyBoard.rows[i].rowOfKeys[j].data.modifiers = hidMapping[index].modifiers;
			keyBoard.rows[i].rowOfKeys[j].data.value = hidMapping[index++];

			//inputs
			//set the direction
			//set the
			ioport_set_pin_dir(keyBoard.rows[i].rowOfKeys[j].column, IOPORT_DIR_INPUT);//set direction to input
			ioport_set_pin_mode(keyBoard.rows[i].rowOfKeys[j].column, (IOPORT_MODE_PULLDOWN | IOPORT_MODE_DEBOUNCE));
		}

	}
}

void checkKeyboard(void)
{

	uint8_t i = 0;
	uint8_t j = 0;
	bool keyState = false;

	for(i=0; i < row0Keys; i++)
	{
		//set the pit output to be high
		ioport_set_pin_level(keyBoard.rows[i].row, true);//works
		
		for(j = 0; j < columns; j++)
		{
			keyState = false;
			//check if the column is high
			keyState = ioport_get_pin_level(keyBoard.rows[i].rowOfKeys[j].column);
			//ioport_get_pin_level(EXAMPLE_BUTTON)

			if(keyState && !keyBoard.rows[i].rowOfKeys[j].keyIsDown)
			{
				//key is high, but was low
				keyBoard.rows[i].rowOfKeys[j].keyIsDown = true;
				keyBoard.rows[i].rowOfKeys[j].justPressed = true;
				keyBoard.rows[i].rowOfKeys[j].keyReleased = false;

			}	
			else if(keyState)
			{
				//
				//it was down previously, but has been released
				keyBoard.rows[i].rowOfKeys[j].keyIsDown = true;//still true
				keyBoard.rows[i].rowOfKeys[j].justPressed = false;//still true
			}
			else
			{
				if(keyBoard.rows[i].rowOfKeys[j].keyIsDown)
				{
					//it was down previously, but has been released
					keyBoard.rows[i].rowOfKeys[j].keyReleased = true;
				}
				keyBoard.rows[i].rowOfKeys[j].keyIsDown = false;
				keyBoard.rows[i].rowOfKeys[j].justPressed = false;//still true
			}
		}
		ioport_set_pin_level(keyBoard.rows[i].row, false);
	}
	
	//iterate through all of the keys
}


void getJustPressedKeys(HID_KEY_DATA * keys, uint8_t *numberOfKeys)
{
	uint8_t i;
	uint8_t j;
	uint8_t keyCounter = 0;

	clearDownKeys();
	for(i = 0; i < row0Keys; i++)
	{
		for(j = 0; j < columns; j++)
		{
			if(keyBoard.rows[i].rowOfKeys[j].justPressed)
			{
				downKeys[keyCounter].modifiers = keyBoard.rows[i].rowOfKeys[j].data.modifiers;
				downKeys[keyCounter].value = keyBoard.rows[i].rowOfKeys[j].data.value;
			}

		}
	}
	*numberOfKeys = keyCounter;
	keys = downKeys;
}

void getReleaseKeys(HID_KEY_DATA * keys, uint8_t *numberOfKeys)
{
	uint8_t i;
	uint8_t j;
	uint8_t keyCounter = 0;

	clearReleasedKeys();
	for(i = 0; i < row0Keys; i++)
	{
		for(j = 0; j < columns; j++)
		{
			if(keyBoard.rows[i].rowOfKeys[j].keyReleased)
			{
				releasedKeys[keyCounter].modifiers = keyBoard.rows[i].rowOfKeys[j].data.modifiers;
				releasedKeys[keyCounter].value = keyBoard.rows[i].rowOfKeys[j].data.value;
			}

		}
	}
	*numberOfKeys = keyCounter;
	keys = releasedKeys;
}


//todo change this to be an array of keys
KEY_OBJ *getPressedKey(void)
{
	uint8_t i;
	uint8_t j;

	for(i = 0; i < row0Keys; i++)
	{
		for(j = 0; j < columns; j++)
		{
			if(keyBoard.rows[i].rowOfKeys[j].keyIsDown)
			{
				return &keyBoard.rows[i].rowOfKeys[j];
			}
		}

	}
	return 0;

}
/**
 * This gets the number of keys that are being pressed
 * @return the number of keys
 */
uint8_t getNumberOfKeysPressed(void)
{
	uint8_t keysPressed = 0;
	uint8_t i = 0;
	uint8_t j = 0;

	for(i=0; i < row0Keys; i++)
	{
		for(j = 0; j < columns; j++)
		{
			//check if the column is high
			//ioport_get_pin_level(EXAMPLE_BUTTON)
			if(ioport_get_pin_level(keyBoard.rows[i].rowOfKeys[j].keyIsDown))
			{
				//key is currently high
				keysPressed++;
			}
		}
	}
	return keysPressed;
}


static void clearDownKeys(void)
{
	uint8_t i;

	for(i= 0; i < NUMBER_OF_KEY_DATA; i++)
	{
		downKeys[i].modifiers = 0x00;
		downKeys[i].value = 0x00;
	}
}
static void clearReleasedKeys(void)
{
	uint8_t i;

	for(i= 0; i < NUMBER_OF_KEY_DATA; i++)
	{
		releasedKeys[i].modifiers = 0x00;
		releasedKeys[i].value = 0x00;
	}
}




