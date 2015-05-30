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
	uint8_t hidKey;
	uint8_t hidKeyMod;
	uint8_t specialKey;
}KEY_OBJ;



#endif /* KEY_H_ */