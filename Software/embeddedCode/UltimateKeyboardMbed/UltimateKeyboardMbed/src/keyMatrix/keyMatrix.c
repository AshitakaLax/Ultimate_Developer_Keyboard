/**
 * keyMatrix.c
 *
 * Created: 8/15/2014 11:16:16 PM
 *  Author: Levi.Balling
 * This file is to fetch all of the io from the system, and then apply it to a check whether it has been press with a debounce check.
 */ 

//List of output Keys
//0 PD3
//1 PD5
//2 PD7
//3 PA30
//4 PD8
//5 PD9
//6 PD15
//7 PD10
//similar to The LED Pins

/** KEY #0 pin definition . */
#define KEY_OUT_0_NAME      "KEY OUT 0 D3"
#define KEY_OUT_0       {PIO_PD3, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_0_MASK  PIO_PD3
#define KEY_OUT_0_PIO   PIOD
#define KEY_OUT_0_ID    ID_PIOD
#define KEY_OUT_0_TYPE  PIO_OUTPUT_1
#define KEY_OUT_0_ATTR  PIO_DEFAULT

#define KEY_OUT_0_GPIO            (PIO_PD3_IDX)
#define KEY_OUT_0_FLAGS           (0)
#define KEY_OUT_0_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_0_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #1 pin definition . */
#define KEY_OUT_1_NAME      "KEY OUT 1 D5"
#define KEY_OUT_1       {PIO_PD5, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_1_MASK  PIO_PD5
#define KEY_OUT_1_PIO   PIOD
#define KEY_OUT_1_ID    ID_PIOD
#define KEY_OUT_1_TYPE  PIO_OUTPUT_1
#define KEY_OUT_1_ATTR  PIO_DEFAULT

#define KEY_OUT_1_GPIO            (PIO_PD5_IDX)
#define KEY_OUT_1_FLAGS           (0)
#define KEY_OUT_1_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_1_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #2 pin definition . */
#define KEY_OUT_2_NAME      "KEY OUT 2 D7"
#define KEY_OUT_2       {PIO_PD7, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_2_MASK  PIO_PD7
#define KEY_OUT_2_PIO   PIOD
#define KEY_OUT_2_ID    ID_PIOD
#define KEY_OUT_2_TYPE  PIO_OUTPUT_1
#define KEY_OUT_2_ATTR  PIO_DEFAULT

#define KEY_OUT_2_GPIO            (PIO_PD7_IDX)
#define KEY_OUT_2_FLAGS           (0)
#define KEY_OUT_2_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_2_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #3 pin definition . */
#define KEY_OUT_3_NAME      "KEY OUT 3 A30"
#define KEY_OUT_3       {PIO_PA30, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_3_MASK  PIO_PA30
#define KEY_OUT_3_PIO   PIOA
#define KEY_OUT_3_ID    ID_PIOA
#define KEY_OUT_3_TYPE  PIO_OUTPUT_1
#define KEY_OUT_3_ATTR  PIO_DEFAULT

#define KEY_OUT_3_GPIO            (PIO_PA30_IDX)
#define KEY_OUT_3_FLAGS           (0)
#define KEY_OUT_3_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_3_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #4 pin definition . */
#define KEY_OUT_4_NAME      "KEY OUT 4 D3"
#define KEY_OUT_4       {PIO_PD3, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_4_MASK  PIO_PD3
#define KEY_OUT_4_PIO   PIOD
#define KEY_OUT_4_ID    ID_PIOD
#define KEY_OUT_4_TYPE  PIO_OUTPUT_1
#define KEY_OUT_4_ATTR  PIO_DEFAULT

#define KEY_OUT_4_GPIO            (PIO_PD3_IDX)
#define KEY_OUT_4_FLAGS           (0)
#define KEY_OUT_4_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_4_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #5 pin definition . */
#define KEY_OUT_5_NAME      "KEY OUT 5 D3"
#define KEY_OUT_5       {PIO_PD3, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_5_MASK  PIO_PD3
#define KEY_OUT_5_PIO   PIOD
#define KEY_OUT_5_ID    ID_PIOD
#define KEY_OUT_5_TYPE  PIO_OUTPUT_1
#define KEY_OUT_5_ATTR  PIO_DEFAULT

#define KEY_OUT_5_GPIO            (PIO_PD3_IDX)
#define KEY_OUT_5_FLAGS           (0)
#define KEY_OUT_5_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_5_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #6 pin definition . */
#define KEY_OUT_6_NAME      "KEY OUT 6 D3"
#define KEY_OUT_6       {PIO_PD3, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_6_MASK  PIO_PD3
#define KEY_OUT_6_PIO   PIOD
#define KEY_OUT_6_ID    ID_PIOD
#define KEY_OUT_6_TYPE  PIO_OUTPUT_1
#define KEY_OUT_6_ATTR  PIO_DEFAULT

#define KEY_OUT_6_GPIO            (PIO_PD3_IDX)
#define KEY_OUT_6_FLAGS           (0)
#define KEY_OUT_6_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_6_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

/** KEY #7 pin definition . */
#define KEY_OUT_7_NAME      "KEY OUT 7 D3"
#define KEY_OUT_7       {PIO_PD3, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}
#define KEY_OUT_7_MASK  PIO_PD3
#define KEY_OUT_7_PIO   PIOD
#define KEY_OUT_7_ID    ID_PIOD
#define KEY_OUT_7_TYPE  PIO_OUTPUT_1
#define KEY_OUT_7_ATTR  PIO_DEFAULT

#define KEY_OUT_7_GPIO            (PIO_PD3_IDX)
#define KEY_OUT_7_FLAGS           (0)
#define KEY_OUT_7_ACTIVE_LEVEL    IOPORT_PIN_LEVEL_LOW
#define KEY_OUT_7_INACTIVE_LEVEL  IOPORT_PIN_LEVEL_HIGH

//list of input keys
//0 PD11
//1 PD12
//2 PD13
//3 PD16
//4 PA9
//5 PA5
//6 PA27
//7 PD18


static void checkPins(void);


static void checkPins(void)
{
	//this is a 8 x 8 matrix of keys 
	//the pins that are going to be the outputs are
	
	//loop through all of the keys
		
	
}