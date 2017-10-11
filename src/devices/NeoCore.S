
/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; NeoCore.s
; 
; Allen Wild
; March 2014
;
; Edited:
; Beau Roland
; August 2015
;
; ARM assembly functions for writing to Adafruit NeoPixels
; with the nrf51
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/
	//.area neo_core, CODE, READONLY

	.data
	.extern neo_fio_reg
	.extern neo_gpio_clr_reg
	.extern neo_gpio_set_reg
	.extern neo_bitmask
	
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.text
	.global neo_out //; void neo_out(int *data, int n);
//; Main function called from the C++ library
//; R0 contains a pointer to the array of color data to send
//; R1 contains the number of bytes of data to send
neo_out:
	PUSH	{R0}
	PUSH	{R1}
	PUSH	{R2}
	PUSH	{R3}
	PUSH	{R4}
	PUSH	{R5}
	PUSH	{R6}
	PUSH	{R7}
	PUSH	{LR}
	MOV		R7, R1					//; move length to R7
	MOV		R6, R0					//; move address to R6
	LDR		R1, =neo_gpio_clr_reg
	LDR		R2, =neo_gpio_set_reg
	LDR		R3, =neo_bitmask
	LDR		R1, [R1]				//; load actual values from memory
	LDR		R2, [R2]
	LDR		R3, [R3]
neo_ledDataLoop:
	MOVS	R4, #1
	LSL		R4,R4,#23	//; loads 1<<23 into R4 for bit test
	LDR		R5, [R6]	//; load rgb to send
neo_rgbLoop:
	STR		R3, [R2]	//; t0: sets pin
neo_bitLoop:
	NOP
	NOP
	NOP					//; ORIGINAL AFTER THIS
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP					//;
	TST		R5,R4		//; t1:
	BNE		t1			//; t1: if "0" then we need to clr, regardless, we continue on and wait this section
	STR		R3, [R1]
t1:						//;     NOP - 1 cycle ~ 62-82ns per cycle
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP					//; ORIGINAL AFTER THIS
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP					//;
t2:						//; t2:
	STR		R3, [R1]	//; t2: clears pin
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP					//; original past this point
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP					//;
	LSR		R4,R4,#1	//; 	to save time on t2, performing this operation in the beginning
	CMP		R4, #0			//; if still more bits, loop back
	BNE		neo_rgbLoop
nextLed:
	ADD 	R6, R6, #4		//; increment address by a full word
	SUB 	R7, R7, #1		//; decrement led count
	CMP		R7, #0
	BNE		neo_ledDataLoop	//; continue if not done
	POP		{R7}
	MOV		LR, R7
	POP		{R7}
	POP		{R6}
	POP		{R5}
	POP		{R4}
	POP		{R3}
	POP		{R2}
	POP		{R1}
	POP		{R0}
	BX		LR

	.end //end code region



