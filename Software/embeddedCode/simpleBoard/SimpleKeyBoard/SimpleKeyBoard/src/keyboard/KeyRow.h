/*
 * KeyColumn.h
 *
 * Created: 5/8/2015 9:17:50 PM
 *  Author: Levi.Balling
 */ 


#ifndef KEYROW_H_
#define KEYROW_H_


#include "Key.h"

#define NUM_OF_KEY_COLUMNS  7

//here is where we define all of the pins being used
typedef struct  key_row
{
	uint32_t rowIO;
	KEY_OBJ keys[NUM_OF_KEY_COLUMNS];
}KEY_ROW;


#endif /* KEYCOLUMN_H_ */