/*
 * keyboard_lib.c
 *
 *  Created on: Mar 20, 2024
 *      Author: aordu
 */



#include "keyboard_lib.h"
#include "stm32f7xx_hal.h"

GPIO_TypeDef* LowSeg_Port[8] = {LowSeg_A_GPIO_Port, LowSeg_B_GPIO_Port,
LowSeg_C_GPIO_Port, LowSeg_D_GPIO_Port, LowSeg_E_GPIO_Port,
LowSeg_F_GPIO_Port, LowSeg_G_GPIO_Port, LowSeg_DP_GPIO_Port};
uint16_t LowSeg_Pin[8] = {LowSeg_A_Pin, LowSeg_B_Pin, LowSeg_C_Pin,
LowSeg_D_Pin, LowSeg_E_Pin, LowSeg_F_Pin, LowSeg_G_Pin, LowSeg_DP_Pin};

GPIO_TypeDef* HighSeg_Port[8] = {HighSeg_A_GPIO_Port, HighSeg_B_GPIO_Port,
HighSeg_C_GPIO_Port, HighSeg_D_GPIO_Port, HighSeg_E_GPIO_Port,
HighSeg_F_GPIO_Port, HighSeg_G_GPIO_Port, HighSeg_DP_GPIO_Port};
uint16_t HighSeg_Pin[8] = {HighSeg_A_Pin, HighSeg_B_Pin, HighSeg_C_Pin,
HighSeg_D_Pin, HighSeg_E_Pin, HighSeg_F_Pin, HighSeg_G_Pin, HighSeg_DP_Pin};

GPIO_TypeDef* Keypad_port[2][4] = {
{Keyboard_row1_GPIO_Port,Keyboard_row2_GPIO_Port,Keyboard_row3_GPIO_Port,Keyboard_row4_GPIO_Port},
{Keyboard_col1_GPIO_Port,Keyboard_col2_GPIO_Port,Keyboard_col3_GPIO_Port,Keyboard_col4_GPIO_Port}
};

// Номера контактов, к которым подключена клавиатура
uint16_t Keypad_pin[2][4] = {
{Keyboard_row1_Pin,Keyboard_row2_Pin,Keyboard_row3_Pin,Keyboard_row4_Pin},
{Keyboard_col1_Pin,Keyboard_col2_Pin,Keyboard_col3_Pin,Keyboard_col4_Pin}
};
// Значения кнопок на клавиатуре
int keys[4][4] = {
 {1, 2, 3, 10},
 {4, 5, 6, 11},
 {7, 8, 9, 12},
 {0, 15, 14, 13}
};

// Набор кодов для отображения чисел от 0 до 15
// на семисегментном индикаторе с общим анодом (Зажигается нулём)
int digits[16][8] = {
 // a,b,c,d,e,f,g,dp
{0,0,0,0,0,0,1,1}, //0
{1,0,0,1,1,1,1,1}, //1
{0,0,1,0,0,1,0,1}, //2
{0,0,0,0,1,1,0,1}, //3
{1,0,0,1,1,0,0,1}, //4
{0,1,0,0,1,0,0,1}, //5
{0,1,0,0,0,0,0,1}, //6
{0,0,0,1,1,1,1,1}, //7
{0,0,0,0,0,0,0,1}, //8
{0,0,0,0,1,0,0,1}, //9
{0,0,0,1,0,0,0,1}, //A
{1,1,0,0,0,0,0,1}, //b
{0,1,1,0,0,0,1,1}, //C
{1,0,0,0,0,1,0,1}, //d
{0,1,1,0,0,0,0,1}, //E
{0,1,1,1,0,0,0,1} //F
};

// Вывести на выбранный индикатор цифру dig
void seg_out(GPIO_TypeDef** ports, uint16_t* pins, int dig){
for(int i = 0; i < 8; i++){
HAL_GPIO_WritePin(ports[i],pins[i],digits[dig][i]);
}
}

// Перевести клавиатуру в исходное состояние
void reset_key_pins(){
for(int i = 0; i<4; i++){
HAL_GPIO_WritePin(Keypad_port[1][i],Keypad_pin[1][i],0);
}
}
// Проверка, какая кнопка сейчас нажата. Если не нажата, то возвращает -1
int read_key(){
int col_num = -1;
int row_num = -1;
for(int i = 0; i<4; i++){
// Ищем строчку, которая замкнулась на землю
if(!HAL_GPIO_ReadPin(Keypad_port[0][i],Keypad_pin[0][i])){
row_num = i;
}
}
// Если найдена строка с нажатой кнопкой
if(row_num != -1){
for(int i = 0; i<4; i++){
// По очереди подаём логич. 1 на разные столбцы и отслеживаем
// на каком значение строки тоже установится в 1
HAL_GPIO_WritePin(Keypad_port[1][i],Keypad_pin[1][i],1);

if(col_num == -1 && HAL_GPIO_ReadPin(Keypad_port[0][row_num],
Keypad_pin[0][row_num])){
col_num = i;
}
}
// Обратно замыкаем все контакты столбцов на землю
reset_key_pins();
if(col_num!=-1){
// По номерам строки и столбца определяем число
int curKey = keys[row_num][col_num];
return curKey;
}
// Найдена строка, а столбец нет
return -1;
}
// Ни одна из кнопок не нажата
return -1;
}
void seg_off(GPIO_TypeDef** ports, uint16_t* pins){
for(int i = 0; i < 8; i++){
HAL_GPIO_WritePin(ports[i],pins[i],1);
}
}
// Вывести число на индикатор младшего разряда
void out_num_low(int num){
seg_out(LowSeg_Port,LowSeg_Pin,num%16);
}
// Погасить все сегменты индикатора младшего разряда
void off_low_seg(){
seg_off(LowSeg_Port,LowSeg_Pin);
}
// Вывести число на индикатор старшего разряда
void out_num_high(int num){
seg_out(HighSeg_Port,HighSeg_Pin,num%16);
}
// Погасить все сегменты индикатора старшего разряда
void off_high_seg(){
seg_off(HighSeg_Port,HighSeg_Pin);
}
// Вывести число на оба индикатора
void out_num_both(int num){
out_num_high(num/10);
out_num_low(num%10);
}
// Погасить все сегменты обоих индикаторов
void off_seg(){
off_low_seg();
off_high_seg();
}

