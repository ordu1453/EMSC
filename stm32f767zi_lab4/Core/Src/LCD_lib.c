/*
 * LCD_lib.c
 *
 *  Created on: Apr 17, 2024
 *      Author: aordu
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32f7xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

#define I2C_ADDRESS_LCD    0x48
#define I2C_TIMEOUT    10

#define ESC 					0x1B
#define BRACKET					0x5B //[
#define CURSOR_POS_CMD			0x48 //H
#define CURSOR_SAVE_CMD			0x73 //s
#define CURSOR_RSTR_CMD			0x75 //u
#define DISP_CLR_CMD			0x6A //j
#define ERASE_INLINE_CMD		0x4B //K
#define ERASE_FIELD_CMD			0x4E //N
#define LSCROLL_CMD				0x40 //@
#define RSCROLL_CMD				0x41 //A
#define RST_CMD					0x2A //*
#define DISP_EN_CMD				0x65 //e
#define DISP_MODE_CMD			0x68 //h
#define CURSOR_MODE_CMD			0x63 //c
#define TWI_SAVE_ADDR_CMD		0x61 //a
#define BR_SAVE_CMD				0x62 //b
#define PRG_CHAR_CMD			0x70 //p
#define SAVE_RAM_TO_EEPROM_CMD	0x74 //t
#define LD_EEPROM_TO_RAM_CMD	0x6C //l
#define DEF_CHAR_CMD			0x64 //d
#define COMM_MODE_SAVE_CMD		0x6D //m
#define EEPROM_WR_EN_CMD		0x77 //w
#define CURSOR_MODE_SAVE_CMD	0x6E //n
#define DISP_MODE_SAVE_CMD		0x6F //o

//access parameters for communication ports
#define	PAR_ACCESS_DSPI0			0
#define	PAR_ACCESS_DSPI1			1
#define	PAR_SPD_MAX				625000
/* ------------------------------------------------------------ */
/*					Errors Definitions							*/
/* ------------------------------------------------------------ */

#define LCDS_ERR_SUCCESS			0	// The action completed successfully
#define LCDS_ERR_ARG_ROW_RANGE		1	// The argument is not within 0, 2 range for rows
#define LCDS_ERR_ARG_COL_RANGE		2	// The argument is not within 0, 39 range
#define LCDS_ERR_ARG_ERASE_OPTIONS	3	// The argument is not within 0, 2 range for erase types
#define LCDS_ERR_ARG_BR_RANGE		4	// The argument is not within 0, 6 range
#define LCDS_ERR_ARG_TABLE_RANGE	5	// The argument is not within 0, 3 range for table selection
#define LCDS_ERR_ARG_COMM_RANGE		6	// The argument is not within 0, 7 range
#define LCDS_ERR_ARG_CRS_RANGE		7	// The argument is not within 0, 2 range for cursor modes
#define LCDS_ERR_ARG_DSP_RANGE		8	// The argument is not within 0, 3 range for display settings types
#define LCDS_ERR_ARG_POS_RANGE		9	// The argument is not within 0, 7 range for characters position in the memory

//other defines used for library functions
#define MAX						150

static void LCD_SendBytes(uint8_t* dataChars, uint8_t lenght){
	HAL_I2C_Master_Transmit(&hi2c2, (I2C_ADDRESS_LCD << 1), dataChars, lenght, I2C_TIMEOUT);
}

/* ------------------------------------------------------------------- */
/** void LCDS::DisplaySet(bool setDisplay, bool setBckl)
**
**	Parameters:
**		setDisplay - boolean parameter through which the display is set on or off
**		setBckl - boolean parameter through which the backlight is set on or off
**
**	Description:
**		This function turns the display and the backlight on or off, according to the user's selection
**
-----------------------------------------------------------------------*/
static void LCD_DisplaySet(bool setDisplay, bool setBckl) {
    uint8_t dispBcklOff []  = {ESC, BRACKET, '0', DISP_EN_CMD};
	uint8_t dispOnBckl  []  = {ESC, BRACKET, '1', DISP_EN_CMD};
	uint8_t dispBcklOn  []  = {ESC, BRACKET, '2', DISP_EN_CMD};
	uint8_t dispOnBcklOn[]  = {ESC, BRACKET, '3', DISP_EN_CMD};

	if ((!setDisplay)&&(!setBckl))	{
		//send the command for both display and backlight off
		LCD_SendBytes(dispBcklOff, 4);
	}
	else if ((setDisplay)&&(!setBckl))	{
		//send the command for display on and backlight off
		LCD_SendBytes(dispOnBckl, 4);
	}
	else if ((!setDisplay)&&(setBckl))	{
		//send the command for backlight on and display off
		LCD_SendBytes(dispBcklOn, 4);
	}
	else {
		//send the command for both display and backlight on
		LCD_SendBytes(dispOnBcklOn, 4);
	}
}

/* ------------------------------------------------------------------- */
/** void LCDS::DisplayClear()
**
**	Description:
**		This function clears the display and returns the cursor home
**
-----------------------------------------------------------------------*/
static void LCD_DisplayClear() {
	uint8_t dispClr[] = {ESC, BRACKET, DISP_CLR_CMD};
	//clear the display and returns the cursor home
	LCD_SendBytes(dispClr, 3);
}

/* ------------------------------------------------------------------- */
/** uint8_t  LCDS::WriteStringAtPos(uint8_t idxLine, uint8_t idxPos, char* strLn)
**
**	Parameters:
**		idxLine - the line where the string is written: 0 or 1
**		idxPos  - the start column for the string to be written:0 to 39
**		strLn   - the string to be written
**
**		This function writes a string at a specified position
**
-----------------------------------------------------------------------*/
static uint8_t LCD_WriteStringAtPos(uint8_t idxRow, uint8_t idxCol, char* strLn, uint8_t length) {
	int i;
	int j;
	uint8_t bResult = LCDS_ERR_SUCCESS;
	if (idxRow < 0 || idxRow > 2){
		bResult |= LCDS_ERR_ARG_ROW_RANGE;
	}
	if (idxCol < 0 || idxCol > 39){
		bResult |= LCDS_ERR_ARG_COL_RANGE;
	}
	if (bResult == LCDS_ERR_SUCCESS){
		//separate the position digits in order to send them, useful when the position is greater than 10
		uint8_t firstDigit 		= idxCol % 10;
		uint8_t secondDigit 	= idxCol / 10;
		//uint8_t length 			= strlen(strLn);
		uint8_t lengthToPrint   = length + idxCol;
		uint8_t stringToSend[]  = {ESC, BRACKET, idxRow + '0', ';', secondDigit + '0', firstDigit + '0', CURSOR_POS_CMD};
		if (lengthToPrint > 40) {
			//truncate the lenght of the string
			//if it's greater than the positions number of a line
			length = 40 - idxCol;
		}
		LCD_SendBytes(stringToSend, 7);
		LCD_SendBytes((uint8_t *)strLn, length);
	}
	return bResult;
}

/* ------------------------------------------------------------------- */
/** unint8_t  LCDS::SetPos(uint8_t idxRow, uint8_t idxCol)
**		This function sets the cursor position
-----------------------------------------------------------------------*/
static uint8_t LCD_SetPos(uint8_t idxRow, uint8_t idxCol) {
	uint8_t bResult = LCDS_ERR_SUCCESS;
	if (idxRow < 0 || idxRow > 2){
		bResult |= LCDS_ERR_ARG_ROW_RANGE;
	}
	if (idxCol < 0 || idxCol > 39){
		bResult |= LCDS_ERR_ARG_COL_RANGE;
	}
	if (bResult == LCDS_ERR_SUCCESS){
		//separate the position digits in order to send them, useful when the position is greater than 10
		uint8_t firstDigit 	= idxCol % 10;
		uint8_t secondDigit 	= idxCol / 10;
		uint8_t stringToSend[] = {ESC, BRACKET, idxRow + '0', ';', secondDigit + '0', firstDigit + '0', CURSOR_POS_CMD};
		LCD_SendBytes(stringToSend, 7);
	}
	return	bResult;
}


/* ------------------------------------------------------------------- */
/** void  LCDS::BuildUserDefChar(uint8_t* strUserDef,  char* cmdStr)
**
**	Parameters:
**		strUserDef - bytes array containing the values to be converted in values that are recognized by the firmware
**		cmdStr	   - characters array containing the values converted
**	Description:
**		This function builds the string to be converted in an interpretable array of chars for the LCD
**
-----------------------------------------------------------------------*/
static void LCD_BuildUserDefChar(uint8_t* strUserDef, char* cmdStr) {
	uint8_t len = 8;
	char elStr[4];
	//print the bytes from the input array as hex values
	for(int i = 0; i < len; i++){
		sprintf(elStr, "%2.2X", strUserDef[i]);
		//concatenate the result with the 0x chars to be able to send it to the LCD
		strcat(cmdStr, "0x");
		//since the Arduino compiler has a bug in the sprintf function, so it doesn't interpret the
		//precision and width specifiers as it should, by adding a 0 in front of a number greater than 16,
		//we need to eliminate that 0 from the string
		if (strUserDef[i] > 15) {
			elStr[3] = 0;
			strcat(cmdStr, elStr + 1);
		}
		else {
			elStr[2] = 0;
			strcat(cmdStr, elStr);
		}
		strcat(cmdStr, ";");
	}
}
/* ------------------------------------------------------------------- */
/** uint8_t  LCDS::DefineUserChar((uint8_t* strUserDef, uint8_t charPos)
**
**	Parameters:
**		strUserDef - characters array containing the numerical value of each row in the char
**		charPos - the position of the character saved in the memory
**	Return Value:
**		uint8_t
**					- LCDS_ERR_SUCCESS - The action completed successfully
**					- LCDS_ERR_ARG_POS_RANGE - The argument is not within 0, 7 range
**	Description:
**		This function saves a user defined char in the RAM memory
**
-----------------------------------------------------------------------*/
static uint8_t LCD_DefineUserChar(uint8_t* strUserDef, uint8_t charPos) {
	char rgcCmd[MAX];
	uint8_t bResult;
	if (charPos >= 0 && charPos <= 7){
		rgcCmd[0] = ESC;
		rgcCmd[1] = BRACKET;
		rgcCmd[2] = 0;
		//build the values to be sent for defining the custom character
		LCD_BuildUserDefChar(strUserDef, rgcCmd + 2);
		uint8_t bLength = strlen(rgcCmd);
		rgcCmd[bLength++] = (char)charPos + '0';
		rgcCmd[bLength++] = DEF_CHAR_CMD;
		//save the defined character in the RAM
		rgcCmd[bLength++] = ESC;
		rgcCmd[bLength++] = BRACKET;
		rgcCmd[bLength++] = '3';
		rgcCmd[bLength++] = PRG_CHAR_CMD;
		LCD_SendBytes((unsigned char*)(rgcCmd), bLength);
		bResult = LCDS_ERR_SUCCESS;
	}
	else {
		bResult = LCDS_ERR_ARG_POS_RANGE;
	}
	return bResult;
}
/* ------------------------------------------------------------------- */
/** void  LCDS::DispUserChar(uint8_t* charPos, uint8_t charNumber, uint8_t idxRow, uint8_t idxCol)
**
**	Parameters:
**		charPos - array containing the positions of the characters saved in the RAM memory
**		charNumber - the number of custom chars to be displayed on the LCD
**		idxRow - the row at which the first character should be displayed
**		idxCol - the column starting from where the character should be displayed
**
**	Return Value:
**		uint8_t
**					- LCDS_ERR_SUCCESS - The action completed successfully
**					- a combination of the following errors (OR-ed):
**						- LCDS_ERR_ARG_COL_RANGE - The argument is not within 0, 39 range
**						- LCDS_ERR_ARG_ROW_RANGE - The argument is not within 0, 2 range
**	Description:
**		This function displays a user defined char at the specified position on the LCD
**
-----------------------------------------------------------------------*/
static uint8_t LCD_DispUserChar(uint8_t* charPos, uint8_t charNumber, uint8_t idxRow, uint8_t idxCol) {
	uint8_t bResult = LCDS_ERR_SUCCESS;
	if (idxRow < 0 || idxRow > 2){
		bResult |= LCDS_ERR_ARG_ROW_RANGE;
	}
	if (idxCol < 0 || idxCol > 39){
		bResult |= LCDS_ERR_ARG_COL_RANGE;
	}
	if (bResult == LCDS_ERR_SUCCESS){
		//set the position of the cursor to the wanted line/column for displaying custom chars
		LCD_SetPos(idxRow, idxCol);
		//send the position of the character or characters that have to be displayed, and their number
		LCD_SendBytes(charPos, charNumber);
	}
	return bResult;
}

/* Вывод целого числа, начиная с заданной позиции */
static void LCD_disp_int(uint8_t idxRow, uint8_t idxCol, int8_t num, int8_t digits){
	char temp_char[digits+1];

	if(num < 0){
		temp_char[0] = '-';
		num = -num;
	}else{
		temp_char[0] = ' ';
	}
	for(int i=digits; i>0; i--){
		temp_char[i] = '0'+num%10;
		num = num/10;
	}

	LCD_WriteStringAtPos(idxRow, idxCol, temp_char, digits+1);
}

/* Вывод дробного числа, начиная с заданной позиции */
static void LCD_disp_double(uint8_t idxRow, uint8_t idxCol, double num){
	char temp_char[8] = {"   .    "};

	if(num < 0){
		temp_char[0] = '-';
		num = -num;
	}
	temp_char[1] = '0'+num/10;
	temp_char[2] = '0'+(int)num%10;

	for(int i=4; i<=7; i++){
		num = num*10;
		temp_char[i] = '0'+(int)num%10;
	}

	LCD_WriteStringAtPos(idxRow, idxCol, temp_char, 7);
}


