

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/NewOutstation.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

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
	
	
	// 5 static binaries, 0 others
	StaticallyAllocatedDatabase<5, 0, 0, 0, 0, 0, 0> staticBuffers;
	
	Database database(staticBuffers.GetFacade());
	
	// 10 binary events, 0 others
	StaticallyAllocatedEventBuffer<10, 0, 0, 0, 0, 0, 0> eventBuffers;
	auto facade = eventBuffers.GetFacade();
	
	SimpleCommandHandler handler(CommandStatus::SUCCESS);
				
	NewOutstation outstation(exe, root, stack.transport, handler, NullTimeWriteHandler::Inst(), database, facade);
		
	stack.transport.SetAppLayer(&outstation);
			
	AVRLinkParser parser(root, exe, stack.link);
	stack.link.SetRouter(&parser);	
	stack.link.OnLowerLayerUp();
	
	// enable timer interrupts at 100Hz	
	exe.Init();
	
	// enable usart rx & tx interrupts
	parser.Init();
	
	// begin all interrupts
	sei();
	
	// Set LED as output
	DDRB |= (7 << 0);
	
	// Use a repeating software timer to toggle binary index 0
	Update(true, false, &exe, &database);	
				
	for (;;)
	{ 								
		exe.Run();	 // run all pending events		
		exe.Sleep(); // and then sleep until an interrupt occurs
	}

	return 0;
}

