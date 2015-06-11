/*
 * salloc.c
 *
 * Created: 6/6/2015 1:33:51 PM
 *  Author: Levi.Balling
 */ 

#include "Salloc.h"
#include <string.h>

//this is size is compatible with teensy boards
//#define KEY_MEMORY_SIZE  1024

//My board capabilities and plenty to spare
#define KEY_MEMORY_SIZE  10240

static volatile uint8_t runTimeMemory[KEY_MEMORY_SIZE];
static uint32_t timeMemoryLength = 0;//current length of memory
void initSalloc(void)
{
	memset(runTimeMemory, 0, KEY_MEMORY_SIZE);
	
}
uint8_t * salloc(uint32_t size)
{
	uint8_t *memPtr;
	if(timeMemoryLength + size > KEY_MEMORY_SIZE)
	{
		//out of memory 
		return 0;//null
	}
	memPtr = &runTimeMemory[timeMemoryLength];
	timeMemoryLength += size;
	return memPtr;
}

uint16_t getMemoryLeft(void)
{
	return timeMemoryLength;
	
}