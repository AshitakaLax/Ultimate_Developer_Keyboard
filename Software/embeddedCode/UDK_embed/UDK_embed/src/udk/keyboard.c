/*
 * keyboard.c
 *
 * Created: 12/12/2014 8:49:02 AM
 *  Author: Levi.Balling
 */ 

#include <asf.h>
#include "keyboard.h"
#include "ioport.h"
#include <delay.h>


static KEY_BOARD_OBJ keyBoard;
//add each the number of keys per row
static uint8_t row0Keys = 3;//we currently have Rows 3,4,5
static uint8_t rowOffset = 3;//row offset
static uint8_t columns = 6;
#define NUMBER_OF_KEY_DATA  10
#define VALID_KEY_COUNT  5 // there has to be 5 checks on the pin in order for it to be valid

typedef struct key_data_buffer
{
	HID_KEY_DATA downKey[NUMBER_OF_KEY_DATA];
	HID_KEY_DATA releasedKey[NUMBER_OF_KEY_DATA];
	uint8_t downKeyCounter;//keep track of the number down keys in the buffer
	uint8_t releaseKeyCounter;//keep track of the number of keys released
}KEY_DATA_BUFFER;

static KEY_DATA_BUFFER keyBuffer;
static HID_KEY_DATA downKeys[NUMBER_OF_KEY_DATA];//this is all of the keys that are currently being pressed
static HID_KEY_DATA releasedKeys[NUMBER_OF_KEY_DATA];//this is all of the keys that are currently being pressed

static HID_KEY_DATA hidMapping[18];


static void clearDownKeys(void);
static void clearReleasedKeys(void);

/**
 * Adds a key to the downKey buffer to send the keys out
 */
static void addDownKeyToBuffer(HID_KEY_DATA data);
static void addReleaseKeyToBuffer(HID_KEY_DATA data)
static void setupMapping(void)
{
	uint8_t index = 0;
	//setup the 18 keys
	//key 0
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_A;

	//key 1
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_S;

	//key 2
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_D;

	//key 3
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_F;

	//key 4
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_G;

	//key 5
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_H;

	//key 6
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_Q;

	//key 7
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_W;

	//key 8
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_E;

	//key 9
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_R;

	//key 10
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_T;

	//key 11
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_Y;

	//key 12
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_Z;

	//key 13
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_X;

	//key 14
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_C;

	//key 15
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_V;

	//key 16
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_B;

	//key 17
	hidMapping[index].modifiers = 0;
	hidMapping[index++].value = UDK_HID_N;
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
	clearReleasedKeys();
	clearDownKeys();

	for(i = 0; i < row0Keys; i++)
	{

		keyBoard.rows[i].row = (KEY_OUTPUT)(KEY_OUT_0 + i+ rowOffset);
		keyBoard.rows[i].rowKeyCount = 0;
		//outputs
		ioport_set_pin_dir(keyBoard.rows[i].row, IOPORT_DIR_OUTPUT);
		ioport_set_pin_level(keyBoard.rows[i].row, IOPORT_PIN_LEVEL_LOW);//all low except for the active row

		for(j = 0; j < columns; j++)
		{
			KEY_OBJ * currentKey = &keyBoard.rows[i].rowOfKeys[j];

			currentKey->column = ((KEY_INPUT)KEY_IN_0 + j);
			currentKey->keyIsDown = false;
			currentKey->justPressed = false;
			currentKey->keyReleased = false;
			currentKey->validCount = 0;


			currentKey->data.modifiers = hidMapping[index].modifiers;
			currentKey->data.value = hidMapping[index++].value;

			//inputs
			//set the direction
			//set the
			ioport_set_pin_dir(currentKey->column, IOPORT_DIR_INPUT);//set direction to input
			ioport_set_pin_mode(currentKey->column, (IOPORT_MODE_PULLDOWN));
		}

	}
}

void checkKeyboard(void)
{

	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t rowKeyCount = 0;
	bool keyState = false;

	for(i=0; i < row0Keys; i++)
	{
		//set the pit output to be high
		KEY_OUTPUT row = keyBoard.rows[i].row;
		ioport_set_pin_level(row, true);//works
		rowKeyCount = 0;
		//we need a wait period here for the pin levels to settle.
		for(j = 0; j < columns; j++)
		{
			keyState = false;
			KEY_OBJ * currentKey = &keyBoard.rows[i].rowOfKeys[j];
			//check if the column is high
			keyState = ioport_get_pin_level(currentKey->column);
			
			//if pin is high
			if(keyState)
			{
				//incrementKeyCount
				currentKey->validCount++;//
				if(currentKey->validCount == VALID_KEY_COUNT)
				{
					//set the key to be just pressed
					//load key into  just pressed key buffer
					currentKey->justPressed = true;
					currentKey->keyIsDown = true;
					currentKey->keyReleased = false;
					addDownKeyToBuffer(currentKey->data);
				}
				else if(currentKey->validCount > VALID_KEY_COUNT)
				{
					//key has been pressed for a while
					currentKey->keyIsDown = true;
					currentKey->keyReleased = false;
				}
				//we are still checking that it is  valid

			}
			else//key is up
			{
				//reset the valid count
				currentKey->validCount = 0;
				//check if the key is down
				if(currentKey->keyIsDown)
				{
					//key was just release
					addReleaseKeyToBuffer(currentKey->data);
					currentKey->justPressed = false;
					currentKey->keyIsDown = false;
					currentKey->keyReleased = true;
				}

			}

		}
		ioport_set_pin_level(keyBoard.rows[i].row, false);
		
		keyBoard.rows[i].rowKeyCount = rowKeyCount;

	}
	
	//iterate through all of the keys
}

static void addDownKeyToBuffer(HID_KEY_DATA data)
{
	if(keyBuffer.downKeyCounter >= NUMBER_OF_KEY_DATA)
	{
		return;//reached the max
	}
	keyBuffer.downKey[keyBuffer.downKeyCounter].value = data.value;
	keyBuffer.downKey[keyBuffer.downKeyCounter++].modifiers = data.modifiers;
}

static void addReleaseKeyToBuffer(HID_KEY_DATA data)
{
	if(keyBuffer.releaseKeyCounter >= NUMBER_OF_KEY_DATA)
	{
		return;//reached the max
	}
	keyBuffer.releasedKey[keyBuffer.releaseKeyCounter].value = data.value;
	keyBuffer.releasedKey[keyBuffer.releaseKeyCounter++].modifiers = data.modifiers;
}

//for now we will focus on just supporting 1 keys

HID_KEY_DATA *  getJustPressedKeys(uint8_t *numberOfKeys)
{
	*numberOfKeys = keyBuffer.downKeyCounter;
	//clear the keys by setting the counter value back to 0
	keyBuffer.downKeyCounter = 0;//the data is still in the buffer, but will be overwritten with the next key press
	return keyBuffer.downKey;
//	uint8_t i;
//	uint8_t j;
//	uint8_t keyCounter = 0;
//
//	clearDownKeys();
//	for(i = 0; i < row0Keys; i++)
//	{
//		for(j = 0; j < columns; j++)
//		{
//			if(keyBoard.rows[i].rowOfKeys[j].justPressed)
//			{
//				downKeys[keyCounter].modifiers = keyBoard.rows[i].rowOfKeys[j].data.modifiers;
//				downKeys[keyCounter++].value = keyBoard.rows[i].rowOfKeys[j].data.value;
//			}
//
//		}
//	}
//	*numberOfKeys = keyCounter;
//	return downKeys;
}

HID_KEY_DATA *  getReleaseKeys(uint8_t *numberOfKeys)
{

	*numberOfKeys = keyBuffer.releaseKeyCounter;
	//clear the keys by setting the counter value back to 0
	keyBuffer.releaseKeyCounter = 0;//the data is still in the buffer, but will be overwritten with the next key press
	return keyBuffer.releasedKey;
//	uint8_t i;
//	uint8_t j;
//	uint8_t keyCounter = 0;
//
//	clearReleasedKeys();
//	for(i = 0; i < row0Keys; i++)
//	{
//		for(j = 0; j < columns; j++)
//		{
//			if(keyBoard.rows[i].rowOfKeys[j].keyReleased)
//			{
//				releasedKeys[keyCounter].modifiers = keyBoard.rows[i].rowOfKeys[j].data.modifiers;
//				releasedKeys[keyCounter++].value = keyBoard.rows[i].rowOfKeys[j].data.value;
//				keyBoard.rows[i].rowOfKeys[j].keyReleased = false;
//
//			}
//
//		}
//	}
//	*numberOfKeys = keyCounter;
//	return releasedKeys;
}


//todo change this to be an array of keys
HID_KEY_DATA getPressedKey(void)
{
	uint8_t i;
	uint8_t j;
	HID_KEY_DATA dummyKey;
	dummyKey.modifiers = HID_MODIFIER_NONE;
	dummyKey.value = (HID_KEY_VAL)0;
	for(i = 0; i < row0Keys; i++)
	{
		for(j = 0; j < columns; j++)
		{
			if(keyBoard.rows[i].rowOfKeys[j].justPressed)
			{
				return keyBoard.rows[i].rowOfKeys[j].data;
			}
		}
	}
	return dummyKey;
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
		keyBuffer.downKey[i].modifiers = 0x00;
		keyBuffer.downKey[i].value = 0x00;
	}
	keyBuffer.downKeyCounter = 0;
}
static void clearReleasedKeys(void)
{
	uint8_t i;

	for(i= 0; i < NUMBER_OF_KEY_DATA; i++)
	{
		keyBuffer.releasedKey[i].modifiers = 0x00;
		keyBuffer.releasedKey[i].value = 0x00;
	}
	keyBuffer.releaseKeyCounter = 0;
}




