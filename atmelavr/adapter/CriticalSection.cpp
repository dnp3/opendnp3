
#include "CriticalSection.h"

#include <avr/interrupt.h>

CriticalSection::CriticalSection()
{
	cli();
}

CriticalSection::~CriticalSection()
{
	sei();
}
	

