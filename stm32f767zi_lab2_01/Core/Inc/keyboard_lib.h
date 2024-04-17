/*
 * keyboard_lib.h
 *
 *  Created on: Mar 20, 2024
 *      Author: aordu
 */

#ifndef INC_KEYBOARD_LIB_H_
#define INC_KEYBOARD_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "main.h"
// Порты, к которым подключена клавиатура

void seg_off(GPIO_TypeDef** ports, uint16_t* pins);
void reset_key_pins();
int read_key();
void out_num_low(int num);
void off_low_seg();
void out_num_high(int num);
void off_high_seg();
void out_num_both(int num);
void off_seg();
void seg_out(GPIO_TypeDef** ports, uint16_t* pins, int dig);

#endif /* INC_KEYBOARD_LIB_H_ */
