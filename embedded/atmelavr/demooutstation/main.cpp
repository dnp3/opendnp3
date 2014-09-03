

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/IOutstationApplication.h>

#include <openpal/logging/LogRoot.h>

#include <avr/io.h> 
#include <avr/interrupt.h>

#include "AVRExecutor.h"
#include "AVRLinkParser.h"
#include "AVRCommandHandler.h"
#include "Macros.h"

using namespace opendnp3;
using namespace arduino;
using namespace openpal;

void ToggleValuesEvery3Seconds(IExecutor* pExecutor, Database* pDatabase);

const uint16_t NUM_BINARY = 5;
int index = 0;
bool value = true;

int GetFreeRAM () {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

int main()
{	
	const uint32_t MAX_APDU_SIZE = 249;
	
	cli();
	
	AVRExecutor exe(5,5); // maximum of 5 async operations and 5 timers
	
	LogRoot root(nullptr, "root", 0);
		
	TransportStack stack(root, &exe, MAX_APDU_SIZE, nullptr, LinkConfig(false, false));
		
	// 5 static binaries, 0 others
	DynamicallyAllocatedDatabase staticBuffers(DatabaseTemplate(5,0,1));
	
	// allow a max of 2 events
	DynamicallyAllocatedEventBuffer eventBuffers(5);
	Database database(staticBuffers.GetFacade());	
		
	OutstationConfig config;
	config.params.maxTxFragSize = MAX_APDU_SIZE;
	config.params.maxRxFragSize = MAX_APDU_SIZE;
	config.eventBufferConfig = EventBufferConfig(5,0,2);
	config.params.allowUnsolicited = true;	
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;
	config.defaultEventResponses.analog = EventAnalogResponse::Group32Var1;
	
	// Object that handles command (CROB / analog output) requests
	// This example can toggle an LED on the Arduino board
	AVRCommandHandler commandHandler;						
						
	Outstation outstation(config, exe, root, stack.transport, commandHandler, DefaultOutstationApplication::Instance(), database, eventBuffers.GetFacade());
		
	stack.transport.SetAppLayer(&outstation);
			
	AVRLinkParser parser(root, exe, stack.link, 8); // buffer up to 8 bytes
	stack.link.SetRouter(&parser);	
	stack.link.OnLowerLayerUp();
	
	// enable timer interrupts at 100Hz	
	exe.Init();
	
	// enable USART RX/TX interrupts
	parser.Init();
	
	// enable all interrupts
	sei();
	
	// Set LED as output
	SET(DDRB, BIT(7));	
	
	ToggleValuesEvery3Seconds(&exe, &database);
				
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

void ToggleValuesEvery3Seconds(IExecutor* pExecutor, Database* pDatabase)
{			
	auto freeRAM = GetFreeRAM();
	auto currentIndex = index;
	auto currentValue = value;
	index = (index + 1) % NUM_BINARY;
	value = !value;
	auto time = pExecutor->GetTime().milliseconds;
				
	{
		Transaction tx(pDatabase);
		pDatabase->Update(Analog(freeRAM, 0x01, time), 0);	
		pDatabase->Update(Binary(currentValue, 0x01, time), currentIndex);
	}	
		
	auto lambda = [pExecutor, pDatabase]() { ToggleValuesEvery3Seconds(pExecutor, pDatabase); };
	pExecutor->Start(TimeDuration::Seconds(10), Action0::Bind(lambda));			
}