

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

int main()
{	
	AVRExecutor exe;
	LogRoot root(nullptr, "root", 0);
	
	LinkConfig config(false, false);
	TransportStack stack(root, &exe, config);
	
	StaticallyAllocatedDatabase<1> staticBuffers;
	
	Database database(staticBuffers.GetFacade());
	
	StaticallyAllocatedEventBuffer<1> eventBuffers;
	auto facade = eventBuffers.GetFacade();
				
	NewOutstation outstation(exe, stack.transport, database, facade);
		
	stack.transport.SetAppLayer(&outstation);
			
	AVRLinkParser parser(root, stack.link);
	stack.link.SetRouter(&parser);	
	stack.link.OnLowerLayerUp();
	
	// start timer interrupts at 100Hz
	// LED will toggle at ~1HZ
	exe.Init();
				
	for (;;)
	{ 		
		exe.RunOne();			
		parser.Tick();		
	}

	return 0;
}

