#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BIT(x)	(1<<x)
#define SET(reg, bits) reg |= bits
#define CLEAR(reg, bits) reg &= ~bits
#define TOGGLE(reg, bits) reg ^= bits

extern "C" void abort()
{
	cli(); // disable interrupts
	
	SET(DDRB, BIT(7));					// Set PORTB, pin 7 as output
	SET(TCCR1B, BIT(CS10)|BIT(CS12));	// Set clock source, x1
	
	for (;;)
	{
		// Check timer value in if statement, true when count matches 1/20 of a second
		if (TCNT1 >= 500)
		{
			TOGGLE(PORTB, BIT(7)); 	// Toggle the LED
			TCNT1 = 0;		// Zero the timer
		}
	}
}