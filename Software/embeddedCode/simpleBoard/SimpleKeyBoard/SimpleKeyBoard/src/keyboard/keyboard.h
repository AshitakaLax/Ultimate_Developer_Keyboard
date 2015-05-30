/*
 * keyboard.h
 *
 * Created: 5/7/2015 6:44:01 PM
 *  Author: Levi.Balling
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "KeyRow.h"
#include "Key.h"

#define NUM_OF_KEY_ROWS  6

typedef struct  keyboard_obj
{
	uint32_t rowIOArr[NUM_OF_KEY_ROWS];
	uint32_t columnIOArr[NUM_OF_KEY_COLUMNS];
	struct extint_chan_conf configExtintChanColumns[NUM_OF_KEY_COLUMNS];
	KEY_ROW keyRows[NUM_OF_KEY_ROWS];
}KEYBOARD_OBJ;

//this is the main keyboard logic
void initKeyBoard(void);


//nothing to init here, 
//get key pressed


uint8_t checkKeys(void);


uint8_t getJustPressedKeys(KEY_OBJ **keyArr);

uint8_t getJustReleaseKeys(KEY_OBJ **keyArr);

#endif /* KEYBOARD_H_ */