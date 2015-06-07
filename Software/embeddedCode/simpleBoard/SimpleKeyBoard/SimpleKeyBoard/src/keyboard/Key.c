/*
 * Key.c
 *
 * Created: 5/8/2015 9:15:29 PM
 *  Author: Levi.Balling
 */ 

#include "Salloc.h"
#include "Key.h"
//here we will create all of the keys that there are

GENERIC_KEY * initStandardKey(uint8_t hid_key, uint8_t hid_modifier)
{
	//Allocate the Ram for the key from the salloc	
	//if we want to support some other source we can add a different one
	STANDARD_KEY *key = (STANDARD_KEY*)salloc(sizeof(STANDARD_KEY));
	key->type = STANDARD_KEY_TYPE;
	key->hid = hid_key;
	key->modifiers = hid_modifier;
	return (GENERIC_KEY*)key;
}


GENERIC_KEY * initLayerKey(KEYBOARD_LAYER layer, bool isLockable)
{

	LAYER_KEY *key = (LAYER_KEY*)salloc(sizeof(LAYER_KEY));
	key->type = LAYER_KEY_TYPE;
	key->layer = layer;
	key->isLocked = false;
	key->lockableKey = isLockable;
	return (GENERIC_KEY*)key;	
	
}

