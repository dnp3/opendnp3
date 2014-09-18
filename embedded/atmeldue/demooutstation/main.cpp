/*
 * demooutstation.cpp
 *
 * Created: 8/27/2014 7:55:11 AM
 *  Author: jadamcrain
 */ 

#include "sam.h"
#include "sam3x8e.h"


#include "ExecutorImpl.h"
#include "LinkParserImpl.h"
#include "CommandHandlerImpl.h"
#include "Macros.h"

#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/IOutstationApplication.h>

#include <openpal/logging/LogRoot.h>

using namespace opendnp3;
using namespace openpal;

void ToggleLEDEvery3Seconds(IExecutor* pExecutor);

uint32_t led = (1u << 27); 

const uint32_t MAX_FRAG_SIZE = 2048;

ExecutorImpl* gpExecutor = nullptr;
bool gValue = true;
uint32_t gTicks = 0;

void SysTick_Handler(void)
{
	gpExecutor->Tick();
	
	++gTicks;
	if(gTicks == 100)
	{
		gTicks = 0;
		if(gValue)
		{
			REG_PIOB_SODR = led; // Set Output Data Register, turns LED on
		}
		else
		{							
			REG_PIOB_CODR = led; // Clear Output Data Register, turns LED off						
		}
		gValue = !gValue;
	}
}

/**
 * \brief Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    // Initialize the SAM system
    SystemInit();
		
	// Output Enable Register
	REG_PIOB_OER = led; 	
		
	ExecutorImpl exe(5,5);
	gpExecutor = &exe;
	

	LogRoot root(nullptr, "root", 0);
	
	TransportStack stack(root, &exe, MAX_FRAG_SIZE, nullptr, LinkConfig(false, false));
	
	// 5 static binaries, 0 others
	DynamicallyAllocatedDatabase buffers(5);
	// allow a max of 5 events
	DynamicallyAllocatedEventBuffer eventBuffers(5);
		
	Database database(buffers.GetFacade());
	
	OutstationConfig config;
	config.params.maxRxFragSize = MAX_FRAG_SIZE;
	config.params.maxTxFragSize = MAX_FRAG_SIZE;
	config.eventBufferConfig = EventBufferConfig(5);
	config.params.allowUnsolicited = true;
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;	

	// Object that handles command (CROB / analog output) requests
	// This example can toggle an LED
	CommandHandlerImpl commandHandler;
	
	DefaultOutstationApplication application;

	Outstation outstation(config, exe, root, stack.transport, commandHandler, application, database, eventBuffers.GetFacade());

	stack.transport.SetAppLayer(&outstation);
	
	LinkParserImpl parser(root, exe, stack.link);
	stack.link.SetRouter(&parser);
	stack.link.OnLowerLayerUp();
		
	// Setup SysTick Timer for 1 msec interrupts
	if (SysTick_Config(84000000 / 1000)) {
		while (1) {  //
		}
	}
	
	// enable USART RX/TX interrupts
	parser.Init();	
	
	ToggleLEDEvery3Seconds(&exe);				
	
	for (;;)
	{		
		
	}

	return 0;
}

void ToggleLEDEvery3Seconds(IExecutor* pExecutor)
{	
	// TODO - toggle LED here ...
		
	// schedule the next toggle
	auto lambda = [pExecutor]() { ToggleLEDEvery3Seconds(pExecutor); };
	pExecutor->Start(TimeDuration::Seconds(3), Action0::Bind(lambda));
}

