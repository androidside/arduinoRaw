/**********************************************************
*   Phoenix FSW - Main
*
*   25 Jan 2016
*
*   Joe Gibson 						(joseph.gibson@nasa.gov)
*	Jordi Vila Hernandez de Lorenzo	(jordi.vila.hdl@gmail.com)
*
*   License: MIT (http://user.mit-license.org)
*
*   https://github.com/gibsjose/phoenix
**********************************************************/

#include "phoenix.h"

//Don't forget `volatile`!
volatile bool sensors = false;

//Timer 1 Compare Interrupt Vector (1s CTC Timer)
ISR(TIMER1_COMPA_vect) {
	//Blink LED
	PORTB ^= _BV(LED_PIN);

	//Initiate a sensor reading
	sensors = true;
}


int main(void) {

	char message[128];


	int ret = 0;

	memset(message, 0, 128);

	//Initialize UART at 9600 baud
	uart_init(UART_BAUD_SELECT(BAUD, F_CPU));

	//Initialization
	uart_puts("Initializing...\r\n");

	ret = init();

	if(ret) {
		uart_puts("<-- ERROR: Initialization failed -->\r\n");
	} else {
		uart_puts("Initialization successful\r\n");
	}

	//Main Loop
	while(true) {
		uart_puts("Loop Running\r\n");
		PORTB |= PIN_13; //LED_ON
		delay_us(1000);
		PORTB &= ~PIN_13; //LED_off
		return 0;

	}


}

int init(void) {
	int ret = 0;

	//Clear interrupts
	cli();

	//Initialize ports
	ret = port_init();

	//Initialize peripherals


	//Initialize devices

	//Enable interrupts
	sei();

	return ret;
}

int port_init(void) {
	//Initialize LED as output
	DDRB |= PIN_13; 	//init_LEDs_as_outputs

	return 0;
}

int timer1_init(void) {
	//Initialize Timer 1
	TCCR1A = 0;
	TCCR1B = 0;

	//Set CTC compare value (1 second)
	OCR1A = TIMER1_COUNT;

	//Enable CTC mode
	TCCR1B |= (1 << WGM12);

	//Enable 1024 prescaler
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);

	//Enable Timer 1 output compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	return 0;
}
