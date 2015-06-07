/*
 * Salloc.h
 *
 * Created: 6/6/2015 1:32:43 PM
 *  Author: Levi.Balling
 */ 


#ifndef SALLOC_H_
#define SALLOC_H_
#include <stdint.h>

uint8_t * salloc(uint8_t size);

uint16_t getMemoryLeft(void);

#endif /* SALLOC_H_ */