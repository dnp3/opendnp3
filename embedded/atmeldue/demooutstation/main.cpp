/*
 * demooutstation.cpp
 *
 * Created: 8/27/2014 7:55:11 AM
 *  Author: jadamcrain
 */ 

#include "sam.h"
#include "sam3x8e.h"

#include "ExecutorImpl.h"

/* commented out until PAL works
#include <opendnp3/transport/TransportStack.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>
#include <opendnp3/outstation/IOutstationApplication.h>

#include <openpal/logging/LogRoot.h>


#include "LinkParserImpl.h"
#include "CommandHandlerImpl.h"
#include "Macros.h"

using namespace opendnp3;
*/

using namespace openpal;

void ToggleLEDEvery3Seconds(IExecutor* pExecutor);

uint32_t led = (1u << 27); 

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

/*	
	LogRoot root(nullptr, "root", 0);
	
	TransportStack stack(root, &exe, 2048, nullptr, LinkConfig(false, false));
	
	// 5 static binaries, 0 others
	DynamicallyAllocatedDatabase buffers(5);
	// allow a max of 5 events
	DynamicallyAllocatedEventBuffer eventBuffers(5);
	
	
	Database database(buffers.GetFacade());
	
	OutstationConfig config;
	config.eventBufferConfig = EventBufferConfig(5);
	config.params.allowUnsolicited = true;
	config.defaultEventResponses.binary = EventBinaryResponse::Group2Var2;
	
	// Object that handles command (CROB / analog output) requests
	// This example can toggle an LED
	CommandHandlerImpl commandHandler;
	
	Outstation outstation(config, exe, root, stack.transport, commandHandler, DefaultOutstationApplication::Instance(), database, eventBuffers.GetFacade());
	
	stack.transport.SetAppLayer(&outstation);
	
	LinkParserImpl parser(root, exe, stack.link);
	stack.link.SetRouter(&parser);
	stack.link.OnLowerLayerUp();
	
	// enable timer interrupts at 100Hz
	exe.Init();
	
	// enable USART RX/TX interrupts
	parser.Init();	
*/	
	
	ToggleLEDEvery3Seconds(&exe);
	
	
	
	for (;;)
	{
		// process any bytes that were received on the interrupt
		// commented out until timers work
		// parser.ProcessRx();

		// run all pending events or expired timers
		//exe.Run();
	
		REG_PIOB_SODR = led; // Set Output Data Register, turns LED on
		
		for(int i=0; i < 10000000; ++i);
		
		REG_PIOB_CODR = led; // Clear Output Data Register, turns LED off
		
		for(int i=0; i < 10000000; ++i);
		
		// sleep until an interrupt occurs
		//exe.Sleep();
		
		    
		    //delay(1000);         // wait for a second
		    
		    //delay(1000);         // wait for a second
		
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

