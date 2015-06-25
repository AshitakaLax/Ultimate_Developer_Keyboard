/*
 * keyboard.c
 *
 * Created: 5/7/2015 6:46:27 PM
 *  Author: Levi.Balling
 */ 

#include <asf.h>
#include <ioport.h>
#include <port.h>
#include "keyboard.h"
#include "KeyIO.h"
#include <delay.h>
#include <usb_protocol_hid.h>
#include "Salloc.h"

static KEYBOARD_OBJ keyboardObj;
static KEYBOARD_OBJ rightKeyboardObj;
#define KEY_DELAY_MS    1

#define MAX_NUMBER_OF_LAYERS    15


static void wakeUpKeyboard(void);

static void handleKeyJustPressed(KEY_OBJ *key);
static void handleKeyJustReleased(KEY_OBJ *key);

//it is a 6 x 7 matrixst


//programming davorak board
//$&[{}(
//tab;,.py
//caps aoeui
//shift 'qjkx
//cntrl alt windows

static uint8_t keyArray[NUM_OF_KEY_ROWS][NUM_OF_KEY_COLUMNS] = {
	HID_SPACEBAR, HID_DELETE, HID_BACKSPACE, HID_SPACEBAR, HID_PAGEDOWN, HID_J,
	HID_PAGEUP, 
	HID_P, HID_6, HID_7, HID_8, HID_9, HID_0,
	HID_I, HID_O, 
	HID_Z, HID_X, HID_C, HID_V, HID_B, HID_U,
	HID_TAB, HID_A, HID_S, HID_D, HID_F, HID_G,	HID_T,
	HID_TAB/**TODO change to be Alt **/, HID_Q, HID_W, HID_E, HID_R, HID_T, HID_CAPS_LOCK,
	HID_TILDE, HID_1, HID_2, HID_3, HID_4, HID_5, HID_ESCAPE,
};

//create the left hand keyboard here
static uint8_t keyArray2[NUM_OF_KEY_ROWS][NUM_OF_KEY_COLUMNS] = { 
	HID_BACKSPACE, HID_DELETE, HID_END, 
	0, 0, HID_HOME,
	HID_SLASH, HID_TILDE,
	HID_6, HID_7, HID_8, HID_9, HID_0,
	0, 0, HID_Z, HID_X, HID_C, HID_V, HID_B, 1,
	 
	HID_TAB, HID_A, HID_S, HID_D, HID_F, HID_G,
	HID_BACKSLASH, 
	HID_TAB, HID_Q, HID_W, HID_E, HID_R, HID_T, 1,
	HID_PLUS, HID_1, HID_2, HID_3, HID_4, HID_5, HID_ESCAPE,
};
		// left hand
		//KEY_Equal_Plus,	KEY_1_Exclamation,	KEY_2_At,	KEY_3_Pound,	KEY_4_Dollar,	KEY_5_Percent,	KEY_Escape,
		//KEY_Backslash_Pipe,	KEY_q_Q,	KEY_w_W,	KEY_e_E,	KEY_r_R,	KEY_t_T,	1,
		//KEY_Tab,	KEY_a_A,	KEY_s_S,	KEY_d_D,	KEY_f_F,	KEY_g_G,
		//KEY_LeftShift,	KEY_z_Z,	KEY_x_X,	KEY_c_C,	KEY_v_V,	KEY_b_B,	1,
		//KEY_LeftGUI,	KEY_GraveAccent_Tilde,	KEY_Backslash_Pipe,	KEY_LeftArrow,	KEY_RightArrow,
		//KEY_LeftControl,	KEY_LeftAlt,
		//0,	0,	KEY_Home,
		//KEY_DeleteBackspace,	KEY_DeleteForward,	KEY_End,


//ASCII ART



//     KEY ID TABLE
// ___________________________________________                             ___________________________________________
// |  0  |  1  |  2  |  3  |  4  |  5  |  6  |                             |  44 |  43 |  42 |  41 |  40 |  39 |  38 |
// -------------------------------------------                             -------------------------------------------
// |  7  |  8  |  9  |  10 |  11 |  12 |     |                             |     |  50 |  49 |  48 |  47 |  46 |  45 |
// ------------------------------------| 13  |                             | 51  |------------------------------------
// |  14 |  15 |  16 |  17 |  18 |  19 |_____|                             |____ |  57 |  56 |  55 |  54 |  53 |  52 |
// ------------------------------------|     |                             |     |------------------------------------
// |  20 |  21 |  22 |  23 |  24 |  25 |  26 |  __________   __________    | 64  |  63 |  62 |  61 |  60 |  59 |  58 |
// ------------------------------------------- / 37 / 36 /   \ 74 \ 75 \   -------------------------------------------
//  | 27 |  28 |  29 |  30 |  31 |       _____/____/____/     \____\____\_____         |  69 |  68 |  67 | 66 | 65  |
//  ------------------------------      /    /    / 35 /       \ 73 \    \    \        ------------------------------
//                                     / 32 / 33 /____/         \____\ 71 \ 70 \
//                                    /    /    / 34 /           \ 72 \    \    \
//                                   /____/____/____/             \____\____\____\
//

//     KEY ID TABLE
// ___________________________________________                             ___________________________________________
// |  0  |  1  |  2  |  3  |  4  |  5  |  6  |                             |  44 |  43 |  42 |  41 |  40 |  39 |  38 |
// -------------------------------------------                             -------------------------------------------
// |  7  |  8  |  9  |  10 |  11 |  12 |     |                             |     |  50 |  49 |  48 |  47 |  46 |  45 |
// ------------------------------------| 13  |                             | 51  |------------------------------------
// |  14 |  15 |  16 |  17 |  18 |  19 |_____|                             |____ |  57 |  56 |  55 |  54 |  53 |  52 |
// ------------------------------------|     |                             |     |------------------------------------
// |  20 |  21 |  22 |  23 |  24 |  25 |  26 |  __________   __________    | 64  |  63 |  62 |  61 |  60 |  59 |  58 |
// ------------------------------------------- / 37 / 36 /   \ 74 \ 75 \   -------------------------------------------
//  | 27 |  28 |  29 |  30 |  31 |       _____/____/____/     \____\____\_____         |  69 |  68 |  67 | 66 | 65  |
//  ------------------------------      /    /    / 35 /       \ 73 \    \    \        ------------------------------
//                                     / 32 / 33 /____/         \____\ 71 \ 70 \
//                                    /    /    / 34 /           \ 72 \    \    \
//                                   /____/____/____/             \____\____\____\
//



static uint8_t keyIdMapping[NUM_OF_KEY_ROWS][NUM_OF_KEY_COLUMNS] = {
	//HID_SPACEBAR, HID_DELETE, HID_BACKSPACE, HID_SPACEBAR, HID_PAGEDOWN, HID_J,
	//HID_PAGEUP,
	//HID_P, HID_6, HID_7, HID_8, HID_9, HID_0,
	//HID_I, HID_O,
	//HID_Z, HID_X, HID_C, HID_V, HID_B, HID_U,
	//HID_TAB, HID_A, HID_S, HID_D, HID_F, HID_G,	HID_T,
	//HID_TAB/**TODO change to be Alt **/, HID_Q, HID_W, HID_E, HID_R, HID_T, HID_CAPS_LOCK,
  //C6, C5, C4, C3, C2, C1, C0
	36, 37, 33, 32, 35, 34, 0,//Row0
	27, 28, 29, 30, 31, 0, 0,//Row1
	20, 21, 22, 23, 24, 25, 26,//Row2
	14, 15, 16, 17, 18, 19, 0,//Row3
	 7,  8,  9, 10, 11, 12, 13,//Row4
	0,1, 2, 3,4,5,6//Row5
	};
	
static uint8_t keyIdMappingRight[NUM_OF_KEY_ROWS][NUM_OF_KEY_COLUMNS] = {

74, 72, 71, 70, 73, 75, 0,//Row0
65, 66, 67, 68, 69, 0, 0,//Row1
58, 59, 60, 61, 62, 63, 64,//Row2
52, 53, 54, 55, 56, 57, 0,//Row3
45, 46, 47, 48, 49, 50, 51,//Row4
38, 39, 40, 41, 42, 43, 44,//Row5
};

//we need a keyboard that is strictly not the keys, but what happens after a key is pressed
typedef struct layer_manager
{
	LAYER* layers[MAX_NUMBER_OF_LAYERS];
	uint8_t numberOfLayers;//the current number of layers on the board
	uint8_t currentLayer;
	KEYBOARD_LAYER currentLayerType;
}LAYER_MANAGER;

static LAYER_MANAGER boardLayerManager;


void initLayerOneKey(void)
{
	//add 1 layer and it's keys
	
	LAYER * temp = initLayer(NUMBER_LAYER);
	if(!addLayer(temp))
	{
		//error couldn't add the layer to the system.
	}
	//
	
	uint8_t keyIndex = 0;
	//create the keys and handle them
	GENERIC_KEY * tempKey;

	//Top Row left hand
	tempKey = initStandardKey(HID_PLUS, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_1, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_2, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_3, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_4, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_5, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_ESCAPE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//0-6
	//Second Row left hand
	tempKey = initStandardKey(HID_BACKSLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_Q, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_W, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_E, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_R, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_T, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//7-12
	
	//Layer Key
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 3 left hand
	tempKey = initStandardKey(HID_TAB, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_A, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_S, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_D, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//18
	tempKey = initStandardKey(HID_F, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_G, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 4 left hand
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//
	tempKey = initStandardKey(HID_Z, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//
	tempKey = initStandardKey(HID_X, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_C, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_V, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_B, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer Key
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 5 left hand
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_UI);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_TILDE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_BACKSLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_LEFT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_RIGHT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
	//Thumb keys left hand
	tempKey = initStandardKey(HID_BACKSPACE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DELETE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_END, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_HOME, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_ALT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_CTRL);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
	
	
	
	//Right Hand
	//Row 5 top right
	tempKey = initStandardKey(HID_UNDERSCORE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_0, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_9, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_8, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_7, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_6, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer button
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 4 top right
	tempKey = initStandardKey(HID_CLOSE_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_P, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_O, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_I, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_U, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_Y, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_OPEN_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 3 right
	tempKey = initStandardKey(HID_QUOTE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_COLON, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_K, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_J, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_H, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 2 right
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_SLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DOT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_COMMA, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_M, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_N, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer button
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 1 right
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_UI);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_RIGHT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_UP, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DOWN, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_LEFT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 0 Thumb
	tempKey = initStandardKey(HID_SPACEBAR, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_ENTER, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_PAGEDOWN, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_PAGEUP, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_ALT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_CTRL);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
		
}


void initKeys(void)
{
	//here we will setup the keys for the specific boards
	//need an array of pointers
	uint8_t keyIndex = 0;
	//create the keys and handle them
	GENERIC_KEY * tempKey;

	//Top Row left hand
	tempKey = initStandardKey(HID_PLUS, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_1, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_2, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_3, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_4, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_5, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_ESCAPE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
//0-6
	//Second Row left hand
	tempKey = initStandardKey(HID_BACKSLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_Q, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_W, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_E, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_R, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_T, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//7-12
	
	//Layer Key
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 3 left hand
	tempKey = initStandardKey(HID_TAB, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_A, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_S, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_D, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//18	
	tempKey = initStandardKey(HID_F, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_G, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	//Row 4 left hand
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		//
	tempKey = initStandardKey(HID_Z, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		//
	tempKey = initStandardKey(HID_X, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_C, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_V, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	tempKey = initStandardKey(HID_B, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer Key
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
		
	//Row 5 left hand
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_UI);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_TILDE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_BACKSLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_LEFT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_RIGHT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
	//Thumb keys left hand
	tempKey = initStandardKey(HID_BACKSPACE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DELETE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_END, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_HOME, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_ALT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_CTRL);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
	
	
	
	//Right Hand
	//Row 5 top right
	tempKey = initStandardKey(HID_UNDERSCORE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_0, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_9, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_8, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_7, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_6, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer button
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 4 top right
	tempKey = initStandardKey(HID_CLOSE_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_P, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_O, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_I, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_U, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_Y, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_OPEN_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 3 right
	tempKey = initStandardKey(HID_QUOTE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_COLON, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_K, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_J, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_H, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 2 right
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_SLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DOT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_COMMA, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_M, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_N, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer button
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 1 right
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_UI);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_RIGHT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_UP, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DOWN, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_LEFT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 0 Thumb
	tempKey = initStandardKey(HID_SPACEBAR, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_ENTER, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_PAGEDOWN, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_PAGEUP, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_ALT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_CTRL);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
}


void initDovrakKeys(void)
{
	//here we will setup the keys for the specific boards
	//need an array of pointers
	uint8_t keyIndex = 0;
	//create the keys and handle them
	GENERIC_KEY * tempKey;

	//Top Row left hand
	tempKey = initStandardKey(HID_PLUS, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_7, HID_MODIFIER_LEFT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_OPEN_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_3, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_4, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_5, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_ESCAPE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//0-6
	//Second Row left hand
	tempKey = initStandardKey(HID_BACKSLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_Q, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_W, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_E, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_R, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_T, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//7-12
	
	//Layer Key
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 3 left hand
	tempKey = initStandardKey(HID_TAB, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_A, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_S, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_D, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//18
	tempKey = initStandardKey(HID_F, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_G, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 4 left hand
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//
	tempKey = initStandardKey(HID_Z, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	//
	tempKey = initStandardKey(HID_X, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_C, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_V, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_B, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer Key
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 5 left hand
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_UI);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_TILDE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_BACKSLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_LEFT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_RIGHT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
	//Thumb keys left hand
	tempKey = initStandardKey(HID_BACKSPACE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DELETE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_END, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_HOME, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_ALT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_LEFT_CTRL);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	
	
	
	
	//Right Hand
	//Row 5 top right
	tempKey = initStandardKey(HID_UNDERSCORE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_0, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_9, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_8, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_7, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_6, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer button
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 4 top right
	tempKey = initStandardKey(HID_CLOSE_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_P, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_O, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_I, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_U, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_Y, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_OPEN_BRACKET, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 3 right
	tempKey = initStandardKey(HID_QUOTE, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_COLON, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_K, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_J, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_H, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 2 right
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_SHIFT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_SLASH, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DOT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_COMMA, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_M, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_N, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Layer button
	tempKey = initStandardKey(HID_L, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 1 right
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_UI);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_RIGHT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_UP, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_DOWN, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_LEFT, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	//Row 0 Thumb
	tempKey = initStandardKey(HID_SPACEBAR, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_ENTER, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_PAGEDOWN, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(HID_PAGEUP, HID_MODIFIER_NONE);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_ALT);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
	tempKey = initStandardKey(0, HID_MODIFIER_RIGHT_CTRL);
	addKey(keyIndex++,tempKey, STANDARD_LAYER);
	
}

static void initLayerManager(void)
{
	boardLayerManager.currentLayer = 0;
	boardLayerManager.currentLayerType = STANDARD_LAYER;
	boardLayerManager.numberOfLayers = 0;
	//initialize all layers to be 0
	//change to memset
	for(uint8_t i = 0; i < MAX_NUMBER_OF_LAYERS; i++)
	{
		//make sure that they are all 0
		boardLayerManager.layers[i] = 0;		
	}

	//setup the standard layer
	LAYER * temp = initLayer(STANDARD_LAYER);
	if(!addLayer(temp))
	{
		//error couldn't add the layer to the system.
	}

}

void initKeyBoard(void)
{
	//configure pin PB10 as output High
	bool tempPinValue = false;	
	//configure pin PB06 as input
	uint8_t i = 0;
	uint8_t j = 0;
	//clear all data in ram buffer
	initSalloc();

	//setup data structure
	initLayerManager();
	initKeys();
	
//outputs
	keyboardObj.rowIOArr[0] = ROW_0;
	keyboardObj.rowIOArr[1] = ROW_1;
	keyboardObj.rowIOArr[2] = ROW_2;
	keyboardObj.rowIOArr[3] = ROW_3;
	keyboardObj.rowIOArr[4] = ROW_4;
	keyboardObj.rowIOArr[5] = ROW_5;
	

//input
	keyboardObj.columnIOArr[0] = COLUMN_0;
	keyboardObj.columnIOArr[1] = COLUMN_1;
	keyboardObj.columnIOArr[2] = COLUMN_2;
	keyboardObj.columnIOArr[3] = COLUMN_3;
	keyboardObj.columnIOArr[4] = COLUMN_4;
	keyboardObj.columnIOArr[5] = COLUMN_5;
	keyboardObj.columnIOArr[6] = COLUMN_6;

	// Right Hand
	//outputs
	rightKeyboardObj.rowIOArr[0] = R_ROW_0;
	rightKeyboardObj.rowIOArr[1] = R_ROW_1;
	rightKeyboardObj.rowIOArr[2] = R_ROW_2;
	rightKeyboardObj.rowIOArr[3] = R_ROW_3;
	rightKeyboardObj.rowIOArr[4] = R_ROW_4;
	rightKeyboardObj.rowIOArr[5] = R_ROW_5;

	//input
	rightKeyboardObj.columnIOArr[0] = R_COLUMN_0;
	rightKeyboardObj.columnIOArr[1] = R_COLUMN_1;
	rightKeyboardObj.columnIOArr[2] = R_COLUMN_2;
	rightKeyboardObj.columnIOArr[3] = R_COLUMN_3;
	rightKeyboardObj.columnIOArr[4] = R_COLUMN_4;
	rightKeyboardObj.columnIOArr[5] = R_COLUMN_5;
	rightKeyboardObj.columnIOArr[6] = R_COLUMN_6;

//interrupts
// pin is pulled down so when we go to sleep mode we
// Set all of the output pins high, and
// set the interrupt to trigger when the pin goes high.

	
	struct extint_chan_conf *config_extint_chan;
	uint8_t eicLine = 0;


//Currently we have disabled all interrupts
	for(i = 0; i < 0; i++)
	{
		config_extint_chan = &keyboardObj.configExtintChanColumns[i];

		extint_chan_get_config_defaults(config_extint_chan);
		config_extint_chan->gpio_pin_pull       = EXTINT_PULL_DOWN;
		config_extint_chan->filter_input_signal = true;
		config_extint_chan->detection_criteria  = EXTINT_DETECT_RISING;

		if(i == 0)
		{
			config_extint_chan->gpio_pin            = COLUMN_0_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_0_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_0_EIC_LINE;
		}
		else if(i == 1)
		{
			config_extint_chan->gpio_pin            = COLUMN_1_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_1_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_1_EIC_LINE;
		}
		else if(i == 2)
		{
			config_extint_chan->gpio_pin            = COLUMN_2_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_2_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_2_EIC_LINE;
		}
		else if(i == 3)
		{
			config_extint_chan->gpio_pin            = COLUMN_3_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_3_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_1_EIC_LINE;
		}
		else if(i == 4)
		{
			config_extint_chan->gpio_pin            = COLUMN_4_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_4_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_1_EIC_LINE;
		}
		else if(i == 5)
		{
			config_extint_chan->gpio_pin            = COLUMN_5_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_5_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_5_EIC_LINE;
		}
		else if(i == 6)
		{
			config_extint_chan->gpio_pin            = COLUMN_6_EIC_PIN;  //;BUTTON_0_EIC_PIN;
			config_extint_chan->gpio_pin_mux        = COLUMN_6_EIC_MUX; //BUTTON_0_EIC_MUX;
			eicLine = COLUMN_6_EIC_LINE;
		}
	
		
		extint_chan_set_config(eicLine, config_extint_chan);
		extint_register_callback(wakeUpKeyboard, eicLine, EXTINT_CALLBACK_TYPE_DETECT);
		extint_chan_enable_callback(eicLine, EXTINT_CALLBACK_TYPE_DETECT);
		
	}


//setup all of the pins

	ioport_init();
	delay_init();
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
		KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
		row->rowIO = keyboardObj.rowIOArr[i];
		rightRow->rowIO = rightKeyboardObj.rowIOArr[i];
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			KEY_OBJ *rightKey = &rightRow->keys[j];
			key->columnIO = keyboardObj.columnIOArr[j];
			key->currentState = false;
			key->previousState = false;
			key->keyId = keyIdMapping[i][j];
			key->keyJustDown = false;
			key->keyJustRelease = false;
			key->keyHoldCount = 0;

			//right key
			rightKey->columnIO = rightKeyboardObj.columnIOArr[j];
			rightKey->currentState = false;
			rightKey->previousState = false;
			rightKey->keyId = keyIdMappingRight[i][j];
			rightKey->keyJustDown = false;
			rightKey->keyJustRelease = false;
			rightKey->keyHoldCount = 0;
			
			ioport_set_pin_dir(key->columnIO, IOPORT_DIR_INPUT);
			ioport_set_pin_mode(key->columnIO, IOPORT_MODE_PULLDOWN);
			
			ioport_set_pin_dir(rightKey->columnIO, IOPORT_DIR_INPUT);
			ioport_set_pin_mode(rightKey->columnIO, IOPORT_MODE_PULLDOWN);
	}
	ioport_set_pin_dir(row->rowIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(row->rowIO, false);//set the pin low
	
	ioport_set_pin_dir(rightRow->rowIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(rightRow->rowIO, false);//set the pin low
		
	}
	
}


LAYER *initLayer(KEYBOARD_LAYER layer)
{
	//a layer holds the number of keys on the board(whether 64, 76, 108, any number)
	LAYER *boardLayer = (LAYER*)salloc(sizeof(LAYER));
	boardLayer->type = layer;
	//zero out all of the keys
	for(uint8_t i = 0; i < NUMBER_OF_KEYS; i++)
	{
		boardLayer->keys[i] = 0;
	}
	return boardLayer;
}

bool addLayer(LAYER *layer)
{
	//check if we already have a the same layer in the manager
	for(uint8_t i = 0; i < boardLayerManager.numberOfLayers; i++)
	{
		
		if(boardLayerManager.layers[i]->type == layer->type)
		{
			return false;//Layer is already in the manager, add a different one
		}
	}
	//add the layer to the manager
	boardLayerManager.layers[boardLayerManager.numberOfLayers] = layer;
	boardLayerManager.numberOfLayers++;
}
// Adds a key
// You can override already assigned keys in run time
bool addKey(uint8_t keyId, GENERIC_KEY *key, KEYBOARD_LAYER layer)
{
	//adds a key to a specific layer already on the board
	for(uint8_t i = 0; i < boardLayerManager.numberOfLayers; i++)
	{
		if(boardLayerManager.layers[i]->type == layer)
		{
			//valid layer
			return addKeyToLayer(keyId, key, boardLayerManager.layers[i]);
		}
	}
	return false;//layer currently isn't in the board
}

bool addKeyToLayer(uint8_t keyId, GENERIC_KEY *key, LAYER *layer)
{
	//can over write existing keys
	//check that the input is valid
	if(key == 0 || layer == 0)
	{
		return false;//invalid key, or layer
	}
	layer->keys[keyId] = key;
	return true;
}



uint8_t checkKeys(void)
{
	uint8_t i = 0;
	uint8_t j = 0;
	static uint8_t  keySendCounter= 0;
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
		KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
		
		ioport_set_pin_level(row->rowIO, true);
		ioport_set_pin_level(rightRow->rowIO, true);
		
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			KEY_OBJ *rightKey = &rightRow->keys[j];
				
			key->previousState = key->currentState;
			key->currentState = ioport_get_pin_level(key->columnIO);
			//delay_ms(KEY_DELAY_MS);
			//check if there is a just pressed/or just released
			if(key->currentState == true && key->previousState == true)
			{
				key->keyHoldCount++;
				//check if just pressed is true
				if(key->keyHoldCount == 1)
				{
					key->keyJustDown = true;
					if(keySendCounter < 6)
					{
					
						handleKeyJustPressed(key);
						keySendCounter++;
					}
				}
				else
				{
					key->keyJustDown = false;
					
				}
			}
			else if(key->currentState == false && key->previousState == false)
			{
				//check that there is a keyHold Count
				if (key->keyHoldCount != 0 && key->keyJustRelease == false)
				{
					key->keyJustRelease = true;
					//handle the key being released
					
					handleKeyJustReleased(key);
					if(keySendCounter > 0)
					{
						
						keySendCounter--;
					}
				}
				else
				{
					key->keyJustRelease = false;
					key->keyHoldCount = 0;
				}
			}
			else
			{
				key->keyJustRelease = false;
				key->keyJustDown = false;
			}
			
			//Right Hand check
			//right hand items
			rightKey->previousState = rightKey->currentState;
			rightKey->currentState = ioport_get_pin_level(rightKey->columnIO);

			if(rightKey->currentState == true && rightKey->previousState == true)
			{
				rightKey->keyHoldCount++;
				//check if just pressed is true
				if(rightKey->keyHoldCount == 1)
				{
					rightKey->keyJustDown = true;
					if(keySendCounter < 6)
					{
						
						handleKeyJustPressed(rightKey);
						keySendCounter++;
					}
					
				}
				else
				{
					rightKey->keyJustDown = false;
								
				}
			}
			else if(rightKey->currentState == false && rightKey->previousState == false)
			{
				//check that there is a keyHold Count
				if (rightKey->keyHoldCount != 0 && rightKey->keyJustRelease == false)
				{
					rightKey->keyJustRelease = true;
					handleKeyJustReleased(rightKey);
					if(keySendCounter > 0)
					{
						
						keySendCounter--;
					}
				}
				else
				{
					rightKey->keyJustRelease = false;
					rightKey->keyHoldCount = 0;
				}
			}
			else
			{
				rightKey->keyJustRelease = false;
				rightKey->keyJustDown = false;
			}
		}
		
		ioport_set_pin_level(row->rowIO, false);//set the pin low
		ioport_set_pin_level(rightRow->rowIO, false);//set the pin low
	}
	
	
	return 0;
}


uint8_t getJustPressedKeys(KEY_OBJ **keyArr)
{
		uint8_t i = 0;
		uint8_t j = 0;
		uint8_t numOfKeyPressed = 0;
		for(i=0; i < NUM_OF_KEY_ROWS; i++)
		{
			KEY_ROW *row = &keyboardObj.keyRows[i];
			KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
			
			for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
			{
				KEY_OBJ *key = &row->keys[j];
				KEY_OBJ *rightKey = &rightRow->keys[j];
				//check if key is just pressed
				if(key->keyJustDown)
				{
					//returns an array of pointers to the key
					keyArr[numOfKeyPressed++] = key;
				}
				
				if(rightKey->keyJustDown)
				{
					//returns an array of pointers to the key
					keyArr[numOfKeyPressed++] = rightKey;
				}
				
			}
		}
	return numOfKeyPressed;
}

uint8_t getJustReleaseKeys(KEY_OBJ **keyArr)
{

	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t numOfKeyReleased = 0;
	for(i=0; i < NUM_OF_KEY_ROWS; i++)
	{
		KEY_ROW *row = &keyboardObj.keyRows[i];
		KEY_ROW *rightRow = &rightKeyboardObj.keyRows[i];
	
		for(j=0; j < NUM_OF_KEY_COLUMNS; j++)
		{
			KEY_OBJ *key = &row->keys[j];
			KEY_OBJ *rightKey = &rightRow->keys[j];
			//check if key is just pressed
			if(key->keyJustRelease)
			{
				//returns an array of pointers to the key
				keyArr[numOfKeyReleased++] = key;
			}
			
			if(rightKey->keyJustRelease)
			{
				//returns an array of pointers to the key
				keyArr[numOfKeyReleased++] = rightKey;
			}
		}
	}
	return numOfKeyReleased;
}

//
// puts the keyboard into sleep mode and enable the external pins to wakeup
//
void KeyboardSleepMode(void)
{
	//disable all of the pins
	
}

//
// puts the keyboard into Running mode and Disable the external pins to wakeup
//
void KeyboardWakeupMode(void)
{
	
}

static void wakeUpKeyboard(void)
{
	
	//udc_remotewakeup();//wakeup the keyboard if it was in sleep mode.
	
	
}


static void handleKeyJustPressed(KEY_OBJ *key)
{
	uint8_t i = 0;
	LAYER *layer = boardLayerManager.layers[boardLayerManager.currentLayer];
	uint8_t keyId = key->keyId;
	GENERIC_KEY *genericKey = layer->keys[keyId];
	if(genericKey == 0)
	{	
		return;//no key
	}
	//since all of the generic keys have the type as the first var in the struct this works
	KEY_PTR_TYPE keyType = genericKey->standard.type;
	if(keyType == STANDARD_KEY_TYPE)
	{
		//
		STANDARD_KEY *standardKey = (STANDARD_KEY*) genericKey;
		udi_hid_kbd_modifier_down(standardKey->modifiers);
		udi_hid_kbd_down(standardKey->hid);
	}
	else if(keyType == LAYER_KEY_TYPE)
	{
		//depending on the layer the key is, we will switch accordingly
		LAYER_KEY *layerKey = (LAYER_KEY *) genericKey;
		//check if it is a layer up or down key
		//check if there is a layer in that direction. if we reach the max wrap around
		if(layerKey->layer == LAYER_UP)
		{
			//check if we can move the layer up
			boardLayerManager.currentLayer++;
			if(boardLayerManager.currentLayer == boardLayerManager.numberOfLayers)
			{
				//go to 0 layer
				boardLayerManager.currentLayer = 0;
			}
		}
		else if(layerKey->layer == LAYER_DOWN)
		{
			//check if we can move the layer Down
			if(boardLayerManager.currentLayer == 0)
			{
				//go to the max layer
				
				boardLayerManager.currentLayer = boardLayerManager.numberOfLayers-1;
//				boardLayerManager.currentLayer = boardLayerManager.;
			}
			else
			{
				boardLayerManager.currentLayer--;
			}
		}
		else
		{
			//go through each layer till the layer matches the current layer
			for(i = 0; i < boardLayerManager.numberOfLayers; i++)
			{
				if(boardLayerManager.layers[i]->type == layerKey->type)
				{
					//set the current layer
//					boardLayerManager.currentLayer = 
				}
				
			}
//			boardLayerManager
			
		}
		
	}
	
}


static void handleKeyJustReleased(KEY_OBJ *key)
{
	LAYER *layer = boardLayerManager.layers[boardLayerManager.currentLayer];
	uint8_t keyId = key->keyId;
	GENERIC_KEY *genericKey = layer->keys[keyId];
	if(genericKey == 0)
	{
		
		return;//no key
	}
	//since all of the generic keys have the type as the first var in the struct this works
	KEY_PTR_TYPE keyType = genericKey->standard.type;
	if(keyType == STANDARD_KEY_TYPE)
	{
		STANDARD_KEY *standardKey = (STANDARD_KEY*) genericKey;
		udi_hid_kbd_modifier_up(standardKey->modifiers);
		udi_hid_kbd_up(standardKey->hid);		
	}
	else if(keyType == LAYER_KEY_TYPE)
	{
		
		
	}
	
}