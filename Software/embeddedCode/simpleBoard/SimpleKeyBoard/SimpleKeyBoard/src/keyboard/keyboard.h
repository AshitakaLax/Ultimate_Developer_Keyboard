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

#define NUMBER_OF_KEYS    76

typedef struct  keyboard_obj
{
	uint32_t rowIOArr[NUM_OF_KEY_ROWS];
	uint32_t columnIOArr[NUM_OF_KEY_COLUMNS];
	struct extint_chan_conf configExtintChanColumns[NUM_OF_KEY_COLUMNS];
	KEY_ROW keyRows[NUM_OF_KEY_ROWS];
}KEYBOARD_OBJ;
typedef struct layer
{
	//contains an array of pointers to generic keys
	GENERIC_KEY *keys[NUMBER_OF_KEYS];
	KEYBOARD_LAYER type;
}LAYER;


//this is the main keyboard logic
// By default the keyboard will support the standard keyboard format
void initKeyBoard(void);



// Adds a key
// You can override already assigned keys in run time
bool addKey(uint8_t keyId, GENERIC_KEY *key, KEYBOARD_LAYER layer);

bool addKeyToLayer(uint8_t keyId, GENERIC_KEY *key, LAYER *layer);

//creates a layer for the system
LAYER *initLayer(KEYBOARD_LAYER layer);

//Adds a layer to the layer Manager
bool addLayer(LAYER *layer);

//nothing to init here, 
//get key pressed
uint8_t checkKeys(void);


uint8_t getJustPressedKeys(KEY_OBJ **keyArr);

uint8_t getJustReleaseKeys(KEY_OBJ **keyArr);





#endif /* KEYBOARD_H_ */