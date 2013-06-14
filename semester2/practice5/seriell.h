//*----------------------------------------------------------------------------
//* File Name           : serielh
//* Object              : Standard C Header File
//*
//* 1.0 04/12/07 MP     : Creation
//*----------------------------------------------------------------------------

#ifndef seriell_h
#define seriell_h

#include "../h/pmc.h"
#include "../h/pio.h"
#include "../h/usart.h"

//void init_ser(void);
//void putc(char);
//char getc(void);

#define 	DEFAULT_BAUD = 9600
#define 	CLOCK_SPEED = 25000000
//#define 	US_BAUD =  (CLOCK_SPEED / (16*(DEFAULT_BAUD))	// 25MHz / ( 16 * 38400) = 40.69  -> 41 -> 0x29
#define 	US_BAUD = 0x29

#endif /* seriell_h */

