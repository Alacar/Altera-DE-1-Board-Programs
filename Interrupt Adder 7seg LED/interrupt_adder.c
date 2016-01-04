/* Robert Tomcik
 * Fall 2015
 *
 * This program is an adder that uses board interrupts to process requests.
 * Whenever a button is pushed, the program reads the value from the slider 
 * switches, and adds it to a sum that is displayed on the 7-segment LED. */

#include "nios2_ctrl_reg_macros.h"
#define SLIDER_SWITCH 0x10000040
#define GREEN_LED 0x10000010
#define RED_LED 0x10000000
#define SEG_LED 0x10000020
#define EDGE_CAPTURE_REGISTER 0x1000005C
#define INTERRUPT_MASK_REGISTER 0x10000058

inline void saveRegisters();
inline void loadRegisters();
inline void write_7seg();

/* The addresses of switches, lights and 7-seg displays. */
volatile int * slider_switch_ptr = (int *) SLIDER_SWITCH;
volatile int * green_LED_ptr = (int *) GREEN_LED;
volatile int * red_LED_ptr = (int *) RED_LED;
volatile int * seg_LED_ptr = (int *) SEG_LED;
volatile int * interrupt_mask_ptr = (int *) INTERRUPT_MASK_REGISTER;
volatile int * edge_capture_ptr = (int *) EDGE_CAPTURE_REGISTER;

/* Switch value and sum initialization */
int sw_value = 0;
int sum = 0;

/* Code needed to implement the reset funcionality */
void the_reset (void)__attribute__((section(".reset")));
void the_reset (void)
{
	asm ("movia r2, _start");	/* Go to the start of  */
	asm ("jmp r2");		  	/*   the program.      */
}

/* Code needed to deal with exceptions */
void the_exception (void)__attribute__((section(".exceptions")));
void the_exception (void)
{
	#define ASCII_NUMERIC_START 48
	asm ("subi sp, sp, 4");		/* Save the contents of  */ 
	asm ("stw et, (sp)");	  	/*   the et register.    */
	asm ("rdctl et, ipending");	   /* If external interrupt, */ 	
	asm ("beq et, r0, SKIP_EA_DEC"); /*   then decrement et    */ 
	asm ("subi ea, ea, 4");		   /*   by 4.                */ 
	asm ("SKIP_EA_DEC:");

	/* Save the registers */
	saveRegisters();
	/* Call the interrupt handler */
	asm ("call INT_HANDLER");
   	/* Load the registers  */
	loadRegisters();
	asm ("eret");		    /* Return from exception. */
} 

/* Displays value to the 7-segment LED */
inline void write_7seg(int value){
	unsigned int divisor = 1000;
	unsigned digit[] = {0, 0, 0, 0};
	/* decimal values for 0, 1, 2, 3, 4, 5, 6, 7, 8, and 9 on the 7-seg LED */
	int bytes[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 103};
	unsigned int i;
	for (i = 0; i < 3; i++) {
		digit[i] = value/divisor;
		digit[i] %= 10;
		divisor /= 10;
	}
	digit[3] = value % 10;
	/* Get the decimal values of the shifted bits */
	digit[0] = bytes[digit[0]] << 24;
	digit[1] = bytes[digit[1]] << 16;
	digit[2] = bytes[digit[2]] << 8;
	digit[3] = bytes[digit[3]];
	/* Add up to get the total value that should be saved to the 7-seg led memory location */
	int total = digit[0] + digit[1] + digit[2] + digit[3];
	*(seg_LED_ptr) = total;
}

/* Test function for 7-seg LED */
void test_segments(){
		volatile int *LED_ptr_copy = seg_LED_ptr;
		*(LED_ptr_copy) = 26191;
		printf("%x", LED_ptr_copy);
}

/* The interrupt handler */
void INT_HANDLER()
{
	/* Get the IRQ of the interrupt */
	int IRQ;
	NIOS2_READ_IPENDING(IRQ);
	/* If the interrupt is from a button, update sum value */
	if(IRQ == 2){
		if(*(edge_capture_ptr)){
			sum += sw_value;
			*(edge_capture_ptr) = 0;
			/* Display new sum onto the 7-segment LED */
			display_segments();
		}
	}
}

main()
{
	/* Enable interrupts on the board */
	NIOS2_WRITE_IENABLE(0x3);
	NIOS2_WRITE_STATUS(1);
	*(interrupt_mask_ptr) = 0xE;
	while(1){
		/* Get the switch value and update the green/red LED */
		sw_value = *(slider_switch_ptr);
		*(green_LED_ptr) = sw_value;
		*(red_LED_ptr) = sum;
		
	}
}

/* Saves the registers to the stack */
inline void saveRegisters(){
	/* General registers */
	asm ("subi sp, sp, 4");	
	asm ("stw r1, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r2, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r3, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r4, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r5, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r6, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r7, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r8, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r9, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r10, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r11, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r12, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r13, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r14, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r15, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r16, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r17, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r18, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r19, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r20, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r21, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r22, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r23, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw r23, (sp)");

	/* Special Registers
	asm ("subi sp, sp, 4");	
	asm ("stw bt, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw gp, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw fp, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw ea, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw ba, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw ra, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw status, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw estatus, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw bstatus, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw ienable, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw ipending, (sp)");
	asm ("subi sp, sp, 4");	
	asm ("stw cpuid, (sp)");
	*/
}

//Loads the registers from the stack
inline void loadRegisters(){
	/* Special registers
	asm("ldw cpuid, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw ipending, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw ienable, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw bstatus, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw estatus, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw status, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw ra, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw ba, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw ea, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw fp, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw gp, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw bt, (sp)");
	asm ("addi sp, sp, 4");	
	*/

	/* General registers */
	asm("ldw r23, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r22, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r21, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r20, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r19, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r18, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r17, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r16, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r15, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r14, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r13, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r12, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r11, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r10, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r9, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r8, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r7, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r6, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r5, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r4, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r3, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r2, (sp)");
	asm ("addi sp, sp, 4");	
	asm("ldw r1, (sp)");
	asm ("addi sp, sp, 4");	
}
