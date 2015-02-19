/*
 * key.h
 *
 * Created: 12/11/2014 12:36:34 PM
 *  Author: Levi.Balling
 */ 


#ifndef KEY_H_
#define KEY_H_

#include "udk_types.h"
#include "compiler.h"
#include "ioport.h"




// #define LED3_GPIO            (PIO_PD22_IDX)
#define KEY_OUTPUT_HIGH(led)     ioport_set_pin_level(GPIO_KEY_OUT_##, GPIO_KEY_OUT_##_INACTIVE_LEVEL)

/**
 * \brief Turns on the specified LEDs.
 *
 * \param led LED to turn on (LEDx_GPIO).
 *
 * \note The pins of the specified LEDs are set to GPIO output mode.
 */
#define KEY_OUTPUT_HIGH(led)      ioport_set_pin_level(led##_GPIO, led##_ACTIVE_LEVEL)

static void (*setPinLevel(ioport_pin_t, bool));// ioport_set_pin_level(ioport_pin_t pin, bool level)

//input and output

typedef enum
{
	UDK_HID_A               = 0x04,
	UDK_HID_B               =  0x05,
	UDK_HID_C               =  0x06,
	UDK_HID_D               =  0x07,
	UDK_HID_E               =  0x08,
	UDK_HID_F               =  0x09,
	UDK_HID_G               =  0x0A,
	UDK_HID_H               =  0x0B,
	UDK_HID_I               =  0x0C,
	UDK_HID_J               =  0x0D,
	UDK_HID_K               =  0x0E,
	UDK_HID_L               =  0x0F,
	UDK_HID_M               =  0x10,
	UDK_HID_N               =  0x11,
	UDK_HID_O               =  0x12,
	UDK_HID_P               =  0x13,
	UDK_HID_Q               =  0x14,
	UDK_HID_R               =  0x15,
	UDK_HID_S               =  0x16,
	UDK_HID_T               =  0x17,
	UDK_HID_U               =  0x18,
	UDK_HID_V                = 0x19,
	UDK_HID_W                = 0x1A,
	UDK_HID_X                = 0x1B,
	UDK_HID_Y                = 0x1C,
	UDK_HID_Z                = 0x1D,
	UDK_HID_1                = 30,
	UDK_HID_2                = 31,
	UDK_HID_3                = 32,
	UDK_HID_4                = 33,
	UDK_HID_5                = 34,
	UDK_HID_6                = 35,
	UDK_HID_7                = 36,
	UDK_HID_8                = 37,
	UDK_HID_9                = 38,
	UDK_HID_0                = 39,
	UDK_HID_ENTER            = 40,
	UDK_HID_ESCAPE           = 41,
	UDK_HID_BACKSPACE        = 42,
	UDK_HID_TAB              = 43,
	UDK_HID_SPACEBAR         = 44,
	UDK_HID_UNDERSCORE       = 45,
	UDK_HID_PLUS             = 46,
	UDK_HID_OPEN_BRACKET     = 47,// {
	UDK_HID_CLOSE_BRACKET    = 48, // }
	UDK_HID_BACKSLASH        = 49,
	UDK_HID_ASH              = 50, // # ~
	UDK_HID_COLON            = 51, // ; :
	UDK_HID_QUOTE            = 52, // ' "
	UDK_HID_TILDE            = 53,
	UDK_HID_COMMA            = 54,
	UDK_HID_DOT              = 55,
	UDK_HID_SLASH            = 56,
	UDK_HID_CAPS_LOCK        = 57,
	UDK_HID_F1               = 58,
	UDK_HID_F2               = 59,
	UDK_HID_F3               = 60,
	UDK_HID_F4               = 61,
	UDK_HID_F5               = 62,
	UDK_HID_F6               = 63,
	UDK_HID_F7               = 64,
	UDK_HID_F8               = 65,
	UDK_HID_F9               = 66,
	UDK_HID_F10              = 67,
	UDK_HID_F11              = 68,
	UDK_HID_F12              = 69,
	UDK_HID_PRINTSCREEN      = 70,
	UDK_HID_SCROLL_LOCK      = 71,
	UDK_HID_PAUSE            = 72,
	UDK_HID_INSERT           = 73,
	UDK_HID_HOME             = 74,
	UDK_HID_PAGEUP           = 75,
	UDK_HID_DELETE           = 76,
	UDK_HID_END              = 77,
	UDK_HID_PAGEDOWN         = 78,
	UDK_HID_RIGHT            = 79,
	UDK_HID_LEFT             = 80,
	UDK_HID_DOWN             = 81,
	UDK_HID_UP               = 82,
	UDK_HID_KEYPAD_NUM_LOCK  = 83,
	UDK_HID_KEYPAD_DIVIDE    = 84,
	UDK_HID_KEYPAD_AT        = 85,
	UDK_HID_KEYPAD_MULTIPLY  = 85,
	UDK_HID_KEYPAD_MINUS     = 86,
	UDK_HID_KEYPAD_PLUS      = 87,
	UDK_HID_KEYPAD_ENTER     = 88,
	UDK_HID_KEYPAD_1         = 89,
	UDK_HID_KEYPAD_2         = 90,
	UDK_HID_KEYPAD_3         = 91,
	UDK_HID_KEYPAD_4         = 92,
	UDK_HID_KEYPAD_5         = 93,
	UDK_HID_KEYPAD_6         = 94,
	UDK_HID_KEYPAD_7         = 95,
	UDK_HID_KEYPAD_8         = 96,
	UDK_HID_KEYPAD_9         = 97,
	UDK_HID_KEYPAD_0         = 98
} HID_KEY_VAL;

   //! \name HID modifier values
//    \name HID modifier values
//    \name HID modifier values
//    \name HID modifier values
//    \name HID modifier values
   //! \name HID modifier values
   //! @{
typedef enum udk_hid_modifier
{
	UDK_HID_MODIFIER_NONE         =  0x00,
	UDK_HID_MODIFIER_LEFT_CTRL    =  0x01,
	UDK_HID_MODIFIER_LEFT_SHIFT   =  0x02,
	UDK_HID_MODIFIER_LEFT_ALT     =  0x04,
	UDK_HID_MODIFIER_LEFT_UI      =  0x08,
	UDK_HID_MODIFIER_RIGHT_CTRL   =  0x10,
	UDK_HID_MODIFIER_RIGHT_SHIFT  =  0x20,
	UDK_HID_MODIFIER_RIGHT_ALT    =  0x40,
	UDK_HID_MODIFIER_RIGHT_UI     =  0x80
} HID_MODIFIER_VAL;




typedef enum key_input
{
	KEY_IN_0 = GPIO_KEY_IN_0,
	KEY_IN_1 = GPIO_KEY_IN_1,
	KEY_IN_2 = GPIO_KEY_IN_2,
	KEY_IN_3 = GPIO_KEY_IN_3,
	KEY_IN_4 = GPIO_KEY_IN_4,
	KEY_IN_5 = GPIO_KEY_IN_5,
	KEY_IN_6 = GPIO_KEY_IN_6,
	KEY_IN_7 = GPIO_KEY_IN_7,
	KEY_IN_8 = GPIO_KEY_IN_8,
	KEY_IN_9 = GPIO_KEY_IN_9,
	KEY_IN_10 = GPIO_KEY_IN_10
} KEY_INPUT;


typedef enum key_output
{
	KEY_OUT_0 = GPIO_KEY_OUT_0,
	KEY_OUT_1 = GPIO_KEY_OUT_1,
	KEY_OUT_2 = GPIO_KEY_OUT_2,
	KEY_OUT_3 = GPIO_KEY_OUT_3,
	KEY_OUT_4 = GPIO_KEY_OUT_4,
	KEY_OUT_5 = GPIO_KEY_OUT_5,
	KEY_OUT_6 = GPIO_KEY_OUT_6,
	KEY_OUT_7 = GPIO_KEY_OUT_7,
	KEY_OUT_8 = GPIO_KEY_OUT_8
} KEY_OUTPUT;


typedef struct hid_key_data
{
	HID_MODIFIER_VAL	modifiers;
	HID_KEY_VAL			value;//the HID value
} HID_KEY_DATA;

//create a key object
typedef struct key_obj
{
	KEY_INPUT	column;
	HID_KEY_DATA	data;
	bool keyIsDown;
	bool justPressed;//this is for the most recent check of the keys
	bool keyReleased;//this is for the most recent release of the keys
} KEY_OBJ;

typedef struct key_row
{
	KEY_OUTPUT 	row;
	KEY_OBJ		rowOfKeys[6];//6 keys in a row
	uint8_t numberOfKeys;
	uint8_t rowKeyCount; //number of keys pressed
} KEY_ROW;

void initKeyObj(KEY_OBJ *keyObj, KEY_INPUT input, KEY_OUTPUT output);





#endif /* KEY_H_ */
