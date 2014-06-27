#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

#include <util/delay.h>

#define BIT(x)	(1<<x)
#define SET(reg, bits) reg |= bits
#define CLEAR(reg, bits) reg &= ~bits
#define TOGGLE(reg, bits) reg ^= bits

extern "C" void abort()
{
	cli(); // disable interrupts
	
	SET(DDRB, BIT(7));					// Set PORTB, pin 7 as output
		
	for (;;)
	{
		TOGGLE(PORTB, BIT(7)); 	// Toggle the LED
		_delay_ms(100);
	}
}