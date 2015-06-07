/*
 * Key.h
 *
 * Created: 5/8/2015 9:15:14 PM
 *  Author: Levi.Balling
 */ 


#ifndef KEY_H_
#define KEY_H_

#include <asf.h>
//key structure
/************************************************************************/
/* Each Key needs to have a row ID to it.                                                                     */
/************************************************************************/
typedef struct key_obj
{
	uint32_t columnIO;
	bool previousState;
	bool currentState;
	bool keyJustDown;
	bool keyJustRelease;
	uint32_t keyHoldCount;
	uint8_t keyId;//this key will map to layer manager
	uint8_t hidKey;
	uint8_t hidKeyMod;
	uint8_t specialKey;
}KEY_OBJ;

/************************************************************************/
/* Using the Keyboard Language you can just use the english keyboard setting
/* In all OS's You don't have press a different button for mac, linux, or windows                                                                      */
/************************************************************************/
typedef enum
{
	QWERTY_LANGUAGE,
	DVORAK_LANGUAGE,
	PROGRAMMING_DVORAK_LANGUAGE
} KEYBOARD_LANGUAGE;

typedef enum
{
	STANDARD_LAYER,//This is the default layer of the keyboard
	PROGRAMMING_LAYER,//This is a layer for all of the short cuts in programming editors
	NUMBER_LAYER,//This is a
	SPARE_1_LAYER,//This is a generic layer
	SPARE_2_LAYER,
	SPARE_3_LAYER,
	SPARE_4_LAYER,
	SPARE_5_LAYER,
	SPARE_6_LAYER,
	SPARE_7_LAYER,
	SPARE_8_LAYER,
	SPARE_9_LAYER,
	SPARE_10_LAYER,//Add more if you like
} KEYBOARD_LAYER;


typedef enum
{
	STANDARD_KEY_TYPE,//This would be a single Key press that will send the single Key(and/or) modifier to the computer
	COMBO_KEY_TYPE,//This is a combo key that will send multiple keys up to the computer in one stroke(not multiples)
	MACRO_KEY_TYPE,//This is a sequence of keys that allows you to send multiple keys to the server in a row.
	PORT_KEY_TYPE,//This is a Port key. It sends a specific ID across the Com port allowing software on the other side of the computer to handle the pressing of the key.(something no one else does)
	LAYER_KEY_TYPE,//This key changes what all of the other keys are doing.
	METHOD_KEY_TYPE,//This key will execute a Method(function, I used method since Fn already exists on keyboards) that has been made in the code(something no one else does)
}KEY_PTR_TYPE;


typedef struct standard_key
{
	KEY_PTR_TYPE type;
	uint8_t hid;
	uint8_t modifiers;
}STANDARD_KEY;

typedef struct layer_key
{
	KEY_PTR_TYPE type;
	KEYBOARD_LAYER layer;
	bool lockableKey;
	bool isLocked;
}LAYER_KEY;

typedef struct combo_key
{
	KEY_PTR_TYPE type;
	//still needs to be implemented
}COMBO_KEY;

typedef struct macro_key
{
	KEY_PTR_TYPE type;
	//still needs to be implemented
}MACRO_KEY;

typedef struct port_key
{
	KEY_PTR_TYPE type;
	//still needs to be implemented
}PORT_KEY;

typedef struct method_key
{
	KEY_PTR_TYPE type;
	//still needs to be implemented
}METHOD_KEY;

typedef union generic_key
{
	STANDARD_KEY standard;
	LAYER_KEY layer;
	COMBO_KEY combo;
	MACRO_KEY macro;
	PORT_KEY port;
	METHOD_KEY method;
	//still needs to be implemented
}GENERIC_KEY;


GENERIC_KEY * initStandardKey(uint8_t hid_key, uint8_t hid_modifier);


GENERIC_KEY * initLayerKey(KEYBOARD_LAYER layer, bool isLockable);



#endif /* KEY_H_ */