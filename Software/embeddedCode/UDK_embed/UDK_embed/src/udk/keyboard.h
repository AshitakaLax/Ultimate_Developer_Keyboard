/*
 * keyboard.h
 *
 * Created: 12/12/2014 8:48:45 AM
 *  Author: Levi.Balling
 *  This file will handle all of the keys as a whole keyboard.
 *  this will allow us to check everything as a whole
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#include "key.h"
//CHANGE THE NUMBER OF KEYS BASED ON KEYBOARD
#define KEY_BOARD_KEYS   3

//all keyboards are capable of
//create a key object
typedef struct key_board_obj
{
	uint8_t keysPressed;//the number of keys being pressed
	KEY_ROW rows[KEY_BOARD_KEYS];//all of the keys on the board
	bool keyIsDown;
} KEY_BOARD_OBJ;



/**
 * Initializes all of the keyboard keys
 */
void initKeyboard(void);

/**
 * This checks all of the keys on the keyboard
 */
void checkKeyboard(void);

//void getJustPressedKeys(HID_KEY_DATA * keys, uint8_t *numberOfKeys);

HID_KEY_DATA *  getJustPressedKeys(uint8_t *numberOfKeys);


HID_KEY_DATA *  getReleaseKeys(uint8_t *numberOfKeys);
/**
 * Temp function just to validate the keys send the value up
 */
HID_KEY_DATA getPressedKey(void);
/**
 * This gets the number of keys that are being pressed
 * @return the number of keys
 */
uint8_t getNumberOfKeysPressed(void);



#endif /* KEYBOARD_H_ */
