/** [COMPLETE THIS!!!!]
 * Name:
 * Course:
 * Section:
 * Assignment:
 * File: lcd.c
 * Dependencies:
 * Usage:
 */

//Includes
//Necessary standard libraries
#include <stdbool.h>
#include <stdint.h>

//Custom header files
#include "lcd.h"
#include "delay.h"
#include "gpio.h"
#include "rcc.h"

/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
static void lcdWaitBusyFlag();

//Static helper functions
/**
 * lcdWriteCmd()
 * Description:
 * 		Sends command over to display.
 * 		Steps:
 * 		1. E=1,RS->CMD,RW->W
 * 		2. CMD-> DB
 * 		3. E=0,RS->CMD,RW->W
 * 		4. Wait appropriate delay or (EC)Poll BF
 * 	Inputs:
 * 		cmd(uint8_t) - cmd to execute on the display
 * 	Outputs: None
 */
static void lcdWriteCmd(uint8_t cmd);



/**
 * LcdSetup()
 * Description:
 * 		Configure PA[11..4] for use as 8-bit output Databus(DB)
 * 		Configure PC8 as output for RS
 * 		Configure PC9 as output for RW
 * 		Configure PC10 as output for R10
 * 		**For EC: Turn of Input Resistors in PA[11..4]
 * Inputs: Gpio a and b
 * Outputs: None
 */
void LcdSetup(){
	// Configure pins
	// Databus
	volatile Gpio *gpio_a = GPIO_A;
	volatile Gpio *gpio_c = GPIO_C;
	RccEnable(gpio_a);
	RccEnable(gpio_c);
	uint32_t mode = gpio_a->moder;
	mode &= ~(DB_MODER_FULL_MASK << (DB_OFFSET * 2));
	mode |= DB_MODER_OUTPUT_MASK << (DB_OFFSET * 2);
	gpio_a->moder = mode;

	// Control
	uint32_t clear_mask = 0;
	uint32_t set_mask = 0;
	clear_mask |= 3 << (RS * 2);
	set_mask |= MODER_OUTPUT << (RS * 2);
	clear_mask |= 3 << (RW * 2);
	set_mask |= MODER_OUTPUT << (RW * 2);
	clear_mask |= 3 << (E * 2);
	set_mask |= MODER_OUTPUT << (E * 2);
	mode = gpio_c->moder;
	mode &= ~clear_mask;
	mode |= set_mask;
	gpio_c->moder = mode;
}

/**
 * LcdInit()
 * Description:
 * 		Wait 40 MS
 * 		Set Function
 * 			8bit mode DL = 1
 * 			2Line mode N = 1
 * 			5x8 dots   F = 0
 * 		Set Function (again)
 * 		Display ON
 * 			DisplayOn D=1
 * 			CursorOn  C=1
 * 			BlinkOn   B=1
 * 		Display Clear (note longer delay)
 * 		Entry Mode Set
 * 			IncrementAddress I/D=1
 * 			ShiftOff S=0
 * Inputs: None
 * Outputs: None
 */
void LcdInit(){
	BusyMilli(40);
	lcdWriteCmd(0b00111000);
	LcdSetDisplay(true, true, true);
	LcdClear();
	lcdWriteCmd(0b00000110);
}

/**
 * LcdClear()
 * Description:
 * 		Clear Display
 * Inputs: None
 * Outputs: None
 */
void LcdClear(){
	lcdWriteCmd(0b00000001);
}

/**
 * LcdHome()
 * Description:
 * 		Move Cursor to the home position (0,0)
 * Inputs: None
 * Outputs: None
 */
void LcdHome(){
	lcdWriteCmd(0b00000010);
}

/**
 * LcdSetDisplay()
 * Description:
 * 		Set/change the display options
 * 	Inputs:
 * 		onOff(bool) - true (display on), false (display off)
 * 		cursor(bool) - true (cursor on), false (cursor off)
 * 		blink(bool) - true (blink on), false (blink off)
 * 	Outputs: none
 */
void LcdSetDisplay(bool onOff, bool cursor, bool blink){
	uint8_t bits = 0b00001000;
	if (onOff) {
		bits |= 0b0100;
	}
	if (cursor) {
		bits |= 0b0010;
	}
	if (blink) {
		bits |= 0b0001;
	}
	lcdWriteCmd(bits);
}

/**
 * LcdSetPosition()
 * Description:
 * 		Set the position of the cursor.
 * 		Top left (0,0)     Top right (0,15)
 * 		Bottom left (1,0)  Bottom right (1,15)
 * 	Inputs:
 * 		row(uint8_t) - row index, 0-top, 1-bottom
 * 		col(uint8_t) - col index, 0-left, 1-right
 */
void LcdSetPosition(uint8_t row, uint8_t col){
	uint8_t bits = 0b10000000;
	bits |= row << 6;
	bits |= col;
	lcdWriteCmd(bits);
}

/**
 * LcdWriteChar()
 * Description:
 * 		Writes the character to screen according table in
 * 		documentation.
 * 		(See lcdWriteCmd for implementation details)
 * 	Input:
 * 		c(char) - char to write
 * 	Output: none
 */
void LcdWriteChar(char c){
	volatile Gpio *data_gpio = DATA_GPIO;
	volatile Gpio *control_gpio = CONTROL_GPIO;
	uint32_t c_long = (uint32_t) c;
	uint32_t c_long_not = (uint32_t) ~c;
	uint32_t c_bsrr = (c_long | (c_long_not << 16)) << DB_OFFSET;
	control_gpio->bsrr = E_SET | RS_DATA | RW_WR;
	data_gpio->bsrr = c_bsrr;
	control_gpio->bsrr = E_CLR | RS_DATA | RW_WR;
	lcdWaitBusyFlag();
}

/**
 * LcdWriteStr()
 * Description:
 * 		Writes string to the current console.
 * 		Note: no protection provided for overwriting end of screen
 * 	Input:
 * 		str(char*) - string to write
 * 	Output:
 * 		None
 */
void LcdWriteStr(const char * const str){
	const char* walker = str;
	while (*walker != 0) {
		LcdWriteChar(*walker);
		walker++;
	}
}

/** ExtraCredit
 * LcdGetPosition()
 * Description:
 * 		Finds the current position of the cursor.
 * 	Inputs:
 * 		row(uint8_t*) - returned row location by reference
 * 		col(uint8_t*) - returned col location by reference
 */
void LcdGetPosition(uint8_t* const row, uint8_t* const col){
	volatile Gpio *data_gpio = DATA_GPIO;
	volatile Gpio *control_gpio = CONTROL_GPIO;
	uint32_t mode = data_gpio->moder;
	mode &= ~(DB_MODER_FULL_MASK << (DB_OFFSET * 2));
	data_gpio->moder = mode;
	control_gpio->bsrr = E_SET | RS_CMD | RW_RD;
	uint16_t ref = (data_gpio->idr & DB_MASK) >> DB_OFFSET;
	control_gpio->bsrr = E_CLR | RS_CMD | RW_RD;
	mode = data_gpio->moder;
	mode &= ~(DB_MODER_FULL_MASK << (DB_OFFSET * 2));
	mode |= DB_MODER_OUTPUT_MASK << (DB_OFFSET * 2);
	data_gpio->moder = mode;
	*row = (ref >> 6) & 1;
	*col = ref & 0x3f;
}

/**
 * lcdWriteCmd()
 * Description:
 * 		Sends command over to display.
 * 		Steps:
 * 		1. E=1,RS->CMD,RW->W
 * 		2. CMD-> DB
 * 		3. E=0,RS->CMD,RW->W
 * 		4. Wait appropriate delay or (EC)Poll BF
 * 	Inputs:
 * 		cmd(uint8_t) - cmd to execute on the display
 * 	Outputs: None
 */
static void lcdWriteCmd(uint8_t cmd){
	volatile Gpio *data_gpio = DATA_GPIO;
	volatile Gpio *control_gpio = CONTROL_GPIO;
	uint32_t cmd_long = (uint32_t) cmd;
	uint32_t cmd_long_not = (uint32_t) ~cmd;
	uint32_t cmd_bsrr = (cmd_long | (cmd_long_not << 16)) << DB_OFFSET;
	control_gpio->bsrr = E_SET | RS_CMD | RW_WR;
	data_gpio->bsrr = cmd_bsrr;
	control_gpio->bsrr = E_CLR | RS_CMD | RW_WR;
	lcdWaitBusyFlag();
}

/** EXTRA CREDIT
 * lcdWaitBusyFlag()
 * Description:
 * 		Blocks polling the BusyFlag until it reads '0'
 * 		Note: The mode of DB7 must be switched to input during polling
 * 				and back to output after polling is complete.
 */
static void lcdWaitBusyFlag(){
	volatile Gpio *data_gpio = DATA_GPIO;
	volatile Gpio *control_gpio = CONTROL_GPIO;
	uint32_t mode = data_gpio->moder;
	mode &= ~(DB_MODER_FULL_MASK << (DB_OFFSET * 2));
	data_gpio->moder = mode;
	control_gpio->bsrr = E_SET | RS_CMD | RW_RD;
	while ((data_gpio->idr & BUSYFLAG_IDR_MASK) > 0){
		control_gpio->bsrr = E_CLR | RS_CMD | RW_RD;
		control_gpio->bsrr = E_SET | RS_CMD | RW_RD;
	}
	control_gpio->bsrr = E_CLR | RS_CMD | RW_RD;
	mode = data_gpio->moder;
	mode &= ~(DB_MODER_FULL_MASK << (DB_OFFSET * 2));
	mode |= DB_MODER_OUTPUT_MASK << (DB_OFFSET * 2);
	data_gpio->moder = mode;
}
