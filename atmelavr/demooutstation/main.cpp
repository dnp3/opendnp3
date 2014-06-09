

#include <opendnp3/app/PointClass.h>
#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>

#include <openpal/LogRoot.h>

#include <avr/io.h> 
#include <avr/interrupt.h>

#include "AVRExecutor.h"
#include "AVRLinkParser.h"
#include "AVRCommandHandler.h"
#include "Macros.h"

using namespace opendnp3;
using namespace arduino;
using namespace openpal;

void ToggleBinaryIndex0Every(uint16_t milliseconds, IExecutor* pExecutor, Database* pDatabase, bool value = true, bool update = false);

int main()
{	
	cli();
	
	AVRExecutor exe;
	
	LogRoot root(nullptr, "root", 0);
		
	TransportStack stack(root, &exe, LinkConfig(false, false));
		
	// 5 static binaries, 0 others
	StaticallyAllocatedDatabase<5, 0, 0, 0, 0, 0, 0> staticBuffers;
	
	Database database(staticBuffers.GetFacade());
	
	// 5 binary events, 0 others
	StaticallyAllocatedEventBuffer<5, 0, 0, 0, 0, 0, 0> eventBuffers;
	OutstationConfig config;
	//config.params.allowUnsolicited = true;	
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;
	
	// Object that handles command (CROB / analog output) requests
	// This example can toggle an LED on the Arduino board
	AVRCommandHandler commandHandler;						
						
	Outstation outstation(config, exe, root, stack.transport, commandHandler, NullTimeWriteHandler::Inst(), database, eventBuffers.GetFacade());
		
	stack.transport.SetAppLayer(&outstation);
			
	AVRLinkParser parser(root, exe, stack.link);
	stack.link.SetRouter(&parser);	
	stack.link.OnLowerLayerUp();
	
	// enable timer interrupts at 100Hz	
	exe.Init();
	
	// enable USART RX/TX interrupts
	parser.Init();
	
	// begin all interrupts
	sei();
	
	// Set LED as output
	SET(DDRB, BIT(7));	
	
	ToggleBinaryIndex0Every(3000, &exe, &database);
				
	for (;;)
	{ 	
		// process any bytes that were received on the interrupt		
		parser.ProcessRx();

		// run all pending events or expired timers							
		exe.Run();
		
		// sleep until an interrupt occurs
		exe.Sleep(); 
	}

	return 0;
}

void ToggleBinaryIndex0Every(uint16_t milliseconds, IExecutor* pExecutor, Database* pDatabase, bool value, bool update)
{	
	if(update)
	{	
		Transaction tx(pDatabase);
		pDatabase->Update(Binary(value, BQ_ONLINE, pExecutor->GetTime().milliseconds), 0);
	}
	
	auto lambda = [pExecutor, pDatabase, value, milliseconds]() { ToggleBinaryIndex0Every(milliseconds, pExecutor, pDatabase, !value, true); };
	pExecutor->Start(TimeDuration::Milliseconds(milliseconds), Bind(lambda));	
}