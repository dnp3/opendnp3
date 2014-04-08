
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

