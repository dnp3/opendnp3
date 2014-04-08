
#include <opendnp3/app/ApplicationStack.h>
#include <opendnp3/link/LinkLayerRouter.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>

#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

#include <opendnp3/link/DNPCrc.h>

#include <openpal/LogRoot.h>

#include <avr/io.h> 
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BIT(x)	(1<<x)
#define SET(reg, bits) reg |= bits
#define CLEAR(reg, bits) reg &= ~bits
#define TOGGLE(reg, bits) reg ^= bits

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void InitUSART()
{	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}

void CheckUSART(void)
{	
	if(UCSR0A & (1<<RXC0))
	{
		auto byte = UDR0;
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = byte;
	}	
}

#include "AVRExecutor.h"
#include "AVRLinkParser.h"

using namespace opendnp3;
using namespace arduino;

int main()
{	
	AVRExecutor exe;
	LogRoot root(nullptr, "root", 0);
	
	LinkConfig config(false, false);				
	ApplicationStack stack(root, &exe, AppConfig(false), config);
				
	StaticallyAllocatedDatabase<1> db;
	StaticallyAllocatedEventBuffer<1> eb;
	Database database(db.GetFacade());
	
	Outstation outstation(
		root,
		&stack.application, 
		&exe,
		NullTimeWriteHandler::Inst(),
		&database,
		eb.GetFacade(),
		SuccessCommandHandler::Inst(), 
		OutstationConfig());
		
	stack.application.SetUser(&outstation);
		
	
	AVRLinkParser parser(root, stack.link);
	stack.link.SetRouter(&parser);
	
	stack.link.OnLowerLayerUp();		

	//InitUSART();
		
	SET(DDRB, BIT(7));					// Set PORTB, pin 7 as output
	SET(TCCR1B, BIT(CS10)|BIT(CS12));	// Set clock source, x1
				
	for (;;)
	{ 		
		exe.RunOne();
			
		parser.Tick();
		
		//CheckUSART();
				
		// Check timer value in if statement, true when count matches 1/20 of a second 
		if (TCNT1 >= 5000) 
		{ 
			TOGGLE(PORTB, BIT(7)); 	// Toggle the LED 
			TCNT1 = 0;				// Zero the timer
/*
			openpal::StaticBuffer<292> buffer;
			auto write = buffer.GetWriteBuffer();
			auto wrapper = LinkFrame::FormatRequestLinkStatus(write, false, 1, 1024);
			
			while(wrapper.Size() > 0)
			{
				while(!(UCSR0A & (1<<UDRE0)));
				UDR0 = wrapper[0];
				wrapper.Advance(1);
			}
*/		
		}		
	}

	return 0;
}

