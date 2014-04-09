

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/NewOutstation.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>

#include <openpal/LogRoot.h>

#include <avr/io.h> 
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BIT(x)	(1<<x)
#define SET(reg, bits) reg |= bits
#define CLEAR(reg, bits) reg &= ~bits
#define TOGGLE(reg, bits) reg ^= bits

#include "AVRExecutor.h"
#include "AVRLinkParser.h"

using namespace opendnp3;
using namespace arduino;
using namespace openpal;

void Update(bool value, bool update, IExecutor* pexe, Database* pDatabase)
{
	if(value)
	{
		SET(PORTB, BIT(7));
	}
	else
	{
		CLEAR(PORTB, BIT(7));
	}
		
	if(update) 
	{
		pDatabase->Update(Binary(value), 0);
	}
	
	auto lambda = [value, update, pexe, pDatabase](){ Update(!value, true, pexe, pDatabase); };
	pexe->Start(TimeDuration::Seconds(1), Bind(lambda));
}

int main()
{	
	cli();
	
	AVRExecutor exe;
	LogRoot root(nullptr, "root", 0);
		
	TransportStack stack(root, &exe, LinkConfig(false, false));
	
	StaticallyAllocatedDatabase<1> staticBuffers;
	
	Database database(staticBuffers.GetFacade());
	
	StaticallyAllocatedEventBuffer<1> eventBuffers;
	auto facade = eventBuffers.GetFacade();
				
	NewOutstation outstation(exe, root, stack.transport, database, facade);
		
	stack.transport.SetAppLayer(&outstation);
			
	AVRLinkParser parser(root, exe, stack.link);
	stack.link.SetRouter(&parser);	
	stack.link.OnLowerLayerUp();
	
	// start timer interrupts at 100Hz	
	exe.Init();
	
	// begin interrupts
	sei();
	
	// Set LED as output
	DDRB |= (7 << 0);
	
	// Use a repeating software timer to toggle binary index 0
	Update(true, false, &exe, &database);	
				
	for (;;)
	{ 								
		exe.RunOne();			
		parser.Tick();		
		//exe.Sleep();
	}

	return 0;
}

