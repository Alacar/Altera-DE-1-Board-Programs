/* Robert Tomcik
 * Fall 2015
 *
 * This program is a simple polling adder machine that reads the value from the slider 
 * switches and adds it to the sum each time a button is pushed. The total is displayed
 * on the red LEDs and the current switch value on the green LEDs.
 */
 
#define SLIDER_SWITCH 0x10000040
#define GREEN_LED 0x10000010
#define RED_LED 0x10000000
#define EDGE_CAPTURE_REGISTER 0x1000005C

int main(void) {

	// Variables
	int sw_value = 0;
	int sum = 0;

	// Pointer references
	volatile int * slider_switch_ptr = (int *) SLIDER_SWITCH;
	volatile int * green_LED_ptr = (int *) GREEN_LED;
	volatile int * red_LED_ptr = (int *) RED_LED;
	volatile int * edge_capture_ptr = (int *) EDGE_CAPTURE_REGISTER;

	// Main loop
	while(1){
		// Get the switch value and update the green LED
		sw_value = *(slider_switch_ptr);
		*(green_LED_ptr) = sw_value;
		// If the button flag is set, then add the current number to the sum and update the red LED
		if(*(edge_capture_ptr)) {
			sum += sw_value;
			*(red_LED_ptr) = sum;
			*(edge_capture_ptr) = 0;
		}
	}

}
