

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/NewOutstation.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>

#include <openpal/LogRoot.h>

#define F_CPU 16000000UL

#include <avr/io.h> 
#include <avr/interrupt.h>

#include "AVRExecutor.h"
#include "AVRLinkParser.h"
#include "AVRCommandHandler.h"

using namespace opendnp3;
using namespace arduino;
using namespace openpal;

void ToggleBinaryIndex0Every(uint16_t milliseconds, IExecutor* pExecutor, Database* pDatabase, bool value, bool update);

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
	StaticallyAllocatedEventBuffer<5, 0, 0, 0, 0, 0, 0> eventBuffers;
	NewOutstationConfig config;
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;
						
	NewOutstation outstation(config, exe, root, stack.transport, AVRCommandHandler::Inst(), NullTimeWriteHandler::Inst(), database, eventBuffers.GetFacade());
		
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
	
	ToggleBinaryIndex0Every(3000, &exe, &database, true, false);
				
	for (;;)
	{ 								
		exe.Run();	 // run all pending events		
		exe.Sleep(); // and then sleep until an interrupt occurs
	}

	return 0;
}

void ToggleBinaryIndex0Every(uint16_t milliseconds, IExecutor* pExecutor, Database* pDatabase, bool value, bool update)
{
	if(update)
	{		
		pDatabase->Update(Binary(value, BQ_ONLINE, pExecutor->GetTime().milliseconds), 0);
	}
	
	auto lambda = [pExecutor, pDatabase, value, milliseconds]() { ToggleBinaryIndex0Every(milliseconds, pExecutor, pDatabase, !value, true); };
	pExecutor->Start(TimeDuration::Milliseconds(milliseconds), Bind(lambda));
}