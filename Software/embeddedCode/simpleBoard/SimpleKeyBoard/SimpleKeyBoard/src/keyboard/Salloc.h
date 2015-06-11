/*
 * Salloc.h
 *
 * Created: 6/6/2015 1:32:43 PM
 *  Author: Levi.Balling
 */ 


#ifndef SALLOC_H_
#define SALLOC_H_
#include <stdint.h>

void initSalloc(void);

uint8_t * salloc(uint32_t size);

uint16_t getMemoryLeft(void);

#endif /* SALLOC_H_ */