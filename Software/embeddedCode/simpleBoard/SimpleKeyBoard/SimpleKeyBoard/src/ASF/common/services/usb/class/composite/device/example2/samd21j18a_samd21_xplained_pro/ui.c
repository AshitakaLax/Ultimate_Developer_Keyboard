/**
 * \file
 *
 * \brief User Interface
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include "ui.h"
#include "keyboard/keyboard.h"
#include <string.h>
#include <delay.h>

#define  MOUSE_MOVE_RANGE 5

//! Sequence process running each \c SEQUENCE_PERIOD ms

//! Wakeup, ignore button change until button is back to default state
static bool btn_wakeup = false;

//! Choice of Button function, 0: Keyboard, 1: Mouse
static uint8_t button_function;

/* Interrupt on "pin change" from push button to do wakeup on USB
 * Note:
 * This interrupt is enable when the USB host enable remote wakeup feature
 * This interrupt wakeup the CPU if this one is in idle mode
 */
static void ui_wakeup_handler(void)
{
	/* It is a wakeup then send wakeup USB */
	udc_remotewakeup();
	LED_On(LED_0_PIN);
}

void ui_init(void)
{
	struct extint_chan_conf config_extint_chan;

	extint_chan_get_config_defaults(&config_extint_chan);

	config_extint_chan.gpio_pin            = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux        = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull       = EXTINT_PULL_UP;
	config_extint_chan.filter_input_signal = true;
	config_extint_chan.detection_criteria  = EXTINT_DETECT_FALLING;
	extint_chan_set_config(BUTTON_0_EIC_LINE, &config_extint_chan);
	extint_register_callback(ui_wakeup_handler, BUTTON_0_EIC_LINE,
			EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);

	/* Initialize LEDs */
	initKeyBoard();
	LED_On(LED_0_PIN);
}

void ui_powerdown(void)
{
	LED_Off(LED_0_PIN);
}

void ui_wakeup_enable(void)
{
	//enable the external interrupts here
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}

void ui_wakeup_disable(void)
{
	//disable the external interrupts here
	extint_chan_disable_callback(BUTTON_0_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}

void ui_wakeup(void)
{
	//when we wake up we need to turn all of the pins low, instead of driving them high
	LED_On(LED_0_PIN);
}

void ui_com_open(uint8_t port)
{
	UNUSED(port);
}


void ui_com_close(uint8_t port)
{
	UNUSED(port);
}


void ui_com_rx_start(void)
{

}


void ui_com_rx_stop(void)
{

}


void ui_com_tx_start(void)
{

}


void ui_com_tx_stop(void)
{

}


void ui_com_error(void)
{

}


void ui_com_overflow(void)
{

}

void ui_start_read(void)
{
}

void ui_stop_read(void)
{
}

void ui_start_write(void)
{
}

void ui_stop_write(void)
{
}

void ui_process(uint16_t framenumber)
{
	static uint8_t cpt_sof = 0;
	uint8_t value;
	uint8_t numberOfKeys = 0;
	uint8_t i= 0;
	uint8_t hidOnlyCount = 0;
	uint8_t keyValue = 0;

	//use the cpt_sof to handle the debounce
	cpt_sof++;
	if(cpt_sof < 15)
	{
		return;
	}
	cpt_sof = 0;

	//array of pointers to an array
	//Yes this Way more that is needed
	KEY_OBJ *keyArr[NUM_OF_KEY_COLUMNS*NUM_OF_KEY_ROWS];
	//clear out all of the in the array
	memset(keyArr, 0, (sizeof(KEY_OBJ*)*NUM_OF_KEY_COLUMNS*NUM_OF_KEY_ROWS));

	if ((framenumber % 1000) == 0) {
		LED_On(LED_0_PIN);
	}
	
	
	
	if ((framenumber % 1000) == 500) {
		LED_Off(LED_0_PIN);
		
		
		
	}
	
	for(i = 0; i < NUM_OF_KEY_COLUMNS*NUM_OF_KEY_ROWS; i++)
	{
		keyArr[i] = 0;
	}
	//checks and sends the keys to the pc	
	checkKeys();
	//numberOfKeys = getJustPressedKeys(keyArr);
	
	//for the first 6 keys
	//for(i = 0; i < numberOfKeys; i++)
	//{
		//if(keyArr[i]->hidKey != 0 && hidOnlyCount < 6)
		//{
			//keyValue = keyArr[i]->hidKey;
			//udi_hid_kbd_down(keyValue);
			//hidOnlyCount++;
			//
			//LED_On(LED_0_PIN);
		//}
		////check the modifier keys
		//if(keyArr[i]->hidKeyMod != HID_MODIFIER_NONE)
		//{
			//udi_hid_kbd_modifier_down(keyArr[i]->hidKeyMod);
		//}
	//}
	
	//numberOfKeys = getJustReleaseKeys(keyArr);
		
	//for the first 6 keys
	//for(i = 0; i < numberOfKeys; i++)
	//{
		//if(keyArr[i]->hidKey != 0 && hidOnlyCount < 6)
		//{
			//udi_hid_kbd_up(keyArr[i]->hidKey);
			//hidOnlyCount++;
			//
			//LED_Off(LED_0_PIN);
		//}
		////check the modifier keys
		//if(keyArr[i]->hidKeyMod != HID_MODIFIER_NONE)
		//{
			//udi_hid_kbd_modifier_up(keyArr[i]->hidKeyMod);
		//}
	//}
	
	//if(button_function) {
		//if (cpt_sof < 5) {
			//return;
		//}
		//cpt_sof = 0;
		///* Uses buttons to move mouse */
		//if (!port_pin_get_input_level(BUTTON_0_PIN)) {
			//udi_hid_mouse_moveY(-MOUSE_MOVE_RANGE);
		//}
	//} else {
		//if ((cpt_sof % 2) == 0) {
			//return;
		//}
		//// Scan buttons on switch 0 to send keys sequence
		//b_btn_state = (!port_pin_get_input_level(BUTTON_0_PIN));
		//if (b_btn_state != btn_last_state) {
			//btn_last_state = b_btn_state;
			//if (btn_wakeup) {
				//if (!b_btn_state) {
					//btn_wakeup = false;
				//}
			//} else {
				//sequence_running = true;
			//}
		//}
		//// Sequence process running each period
		//if (SEQUENCE_PERIOD > cpt_sof) {
			//return;
		//}
		//cpt_sof = 0;
		//
		//if (sequence_running) {
			//// Send next key
			//value = ui_sequence[sequence_pos].value;
			//if (value!=0) {
				//if (ui_sequence[sequence_pos].b_modifier) {
					//if (ui_sequence[sequence_pos].b_down) {
						//success = udi_hid_kbd_modifier_down(value);
					//} else {
						//success = udi_hid_kbd_modifier_up(value);
					//}
				//} else {
					//if (ui_sequence[sequence_pos].b_down) {
						//success = udi_hid_kbd_down(value);
					//} else {
						//success = udi_hid_kbd_up(value);
					//}
				//}	
				//if (!success) {
					//return; // Retry it on next schedule
				//}
			//}
			//// Valid sequence position
			//sequence_pos++;
			//if (sequence_pos >=
				//sizeof(ui_sequence) / sizeof(ui_sequence[0])) {
				//sequence_pos = 0;
				//sequence_running = false;
				//button_function  = 1;
			//}
		//}
	//}
}

void ui_kbd_led(uint8_t value)
{
	UNUSED(value);
}

/**
 * \defgroup UI User Interface
 *
 * Human interface on SAMD21-XPlain:
 * - LED0 blinks when USB host has checked and enabled HID Keyboard, mouse, CDC and MSC interface
 * - When firstly press SW0 down, application will open a notepad application on Windows O.S.
 *   and sends key sequence "Atmel ARM"
 * - After first press SW0, Push button 0 (SW0) are used to move mouse up.
 * - Only SW0 down will generate a wakeup to USB Host in remote wakeup mode.
 */
