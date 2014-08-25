/*
 * keyMatrix.h
 *
 * Created: 8/15/2014 11:17:33 PM
 *  Author: Levi.Balling
 * Fetches the data from the io, and does a debounce check. This will be an array of objects that hold the key's number, 
 * Current key or keys.
 */ 


#ifndef KEYMATRIX_H_
#define KEYMATRIX_H_


//a single key will need the following
typedef struct ukey//ukey is for the ultimate key
{
	uint8_t id;//the keyNumber
	uint8_t keyState;//1(true) if key is being pressed, false(0) if not pressed
	uint8_t checkCount;//this is how long it has been pressed for.
	uint8_t keySent;//this is the temp to check if it has been press for 2 checks.	
} UKEY;


/************************************************************************/
/* This will check all of the keys and return an array of all the pressed keys*/
/************************************************************************/
void getSelectedKeys(UKEY *selectedKeys, uint8_t *size);



#endif /* KEYMATRIX_H_ */