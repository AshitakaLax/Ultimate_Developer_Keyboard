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


/**
 * Initializes all of the keyboard keys
 */
void initKeyboard(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
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

			keyBoard.rows[i].rowOfKeys[j].modifiers = UDK_HID_MODIFIER_NONE;
			keyBoard.rows[i].rowOfKeys[j].value = UDK_HID_A + i;

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
			if(keyState)
			{
				//key is currently high
				keyBoard.rows[i].rowOfKeys[j].keyIsDown = true;
			}	
			else
			{
				keyBoard.rows[i].rowOfKeys[j].keyIsDown = false;
			}
		}
		ioport_set_pin_level(keyBoard.rows[i].row, false);
	}
	
	//iterate through all of the keys
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



