
#include "sam.h"

/*
#include <opendnp3/app/ApplicationStack.h>
#include <opendnp3/link/LinkLayerRouter.h>
#include <opendnp3/outstation/StaticallyAllocatedDatabase.h>
#include <opendnp3/outstation/StaticallyAllocatedEventBuffer.h>

#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

#include <opendnp3/link/DNPCrc.h>

#include <openpal/LogRoot.h>

using namespace opendnp3;
*/

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();

/*
    //AVRExecutor exe;
    LogRoot root(nullptr, "root", 0);

    LinkConfig config(false, false);
    ApplicationStack stack(root, nullptr, AppConfig(false), config);

    StaticallyAllocatedDatabase<1> db;
    StaticallyAllocatedEventBuffer<1> eb;
    Database database(db.GetFacade());

    Outstation outstation(
    root,
    &stack.application,
    nullptr,
    NullTimeWriteHandler::Inst(),
    &database,
    eb.GetFacade(),
    SuccessCommandHandler::Inst(),
    OutstationConfig());

    stack.application.SetUser(&outstation);


    AVRLinkParser parser(root, stack.link);
    stack.link.SetRouter(&parser);
    stack.link.OnLowerLayerUp();

    
    // exe.Init();
*/

    for (;;)
    {

    }    
}
