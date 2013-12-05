#ifndef __MOCK_COMMAND_HANDLER_H_
#define __MOCK_COMMAND_HANDLER_H_

#include <opendnp3/SimpleCommandHandler.h>

namespace opendnp3
{

class MockCommandHandler : public SimpleCommandHandler
{
public:

	MockCommandHandler(CommandStatus aStatus = CommandStatus::SUCCESS) :
		SimpleCommandHandler([&]() {
		++mNumInvocations;
		return mStatus;
	}),
	mNumInvocations(0),
	mStatus(aStatus)
	{}

	void SetResponse(CommandStatus aStatus) {
		mStatus = aStatus;
	}

	int mNumInvocations;

private:

	CommandStatus mStatus;

};

}

#endif

