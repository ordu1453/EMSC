/*
 * func.c
 *
 *  Created on: Apr 3, 2024
 *      Author: aordu
 */

#include "func.h"


void func1(void)
{
	static GPIO_PinState LED_STATES[3] = {1,1,0};//101,011,110
	static GPIO_PinState LED_STATES_OLD[3];
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, LED_STATES[0]);
	HAL_GPIO_WritePin(GPIOB, LED2_Pin, LED_STATES[1]);
	HAL_GPIO_WritePin(GPIOB, LED3_Pin, LED_STATES[2]);
	memcpy(LED_STATES_OLD, LED_STATES,sizeof(LED_STATES));
	LED_STATES[0] = LED_STATES_OLD[1];
	LED_STATES[1] = LED_STATES_OLD[2];
	LED_STATES[2] = LED_STATES_OLD[0];
	}




void func2(void)
{
	static int i = 0;
	static GPIO_PinState LED_STATES[5][3] = {
			{1,1,1},{1,1,0},{1,0,0},{0,0,0},{1,0,1}
	};
	static int SEQUENCE[11] = {0,1,2,3,4,0,1,2,3,4,0};
	static uint8_t k;
	k = SEQUENCE[i];
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, LED_STATES[k][0]);
	HAL_GPIO_WritePin(GPIOB, LED2_Pin, LED_STATES[k][1]);
	HAL_GPIO_WritePin(GPIOB, LED3_Pin, LED_STATES[k][2]);
	if (i == 4)
	{
		i = 0;
	}
	else
	{
		i++;
	}
}


void func3(void)
{
	static int num = 0;
	num += 2;
	num = ~num;
	HAL_GPIO_WritePin(GPIOB, LED1_Pin, num&(1<<0));
	HAL_GPIO_WritePin(GPIOB, LED2_Pin, num&(1<<1));
	HAL_GPIO_WritePin(GPIOB, LED3_Pin, num&(1<<2));

}


























