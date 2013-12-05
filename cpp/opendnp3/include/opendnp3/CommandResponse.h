#ifndef __COMMAND_RESPONSE_H_
#define __COMMAND_RESPONSE_H_

#include "gen/CommandStatus.h"
#include "gen/CommandResult.h"

namespace opendnp3
{

/**
* Represents the result of a command request
*/
class CommandResponse
{
public:
	
	CommandResponse(CommandResult aResult = CommandResult::NO_COMMS, CommandStatus aStatus = CommandStatus::UNDEFINED);

	static CommandResponse OK(CommandStatus aStatus);
	
	///  The result of the operation, should be examined before looking at the status code
	CommandResult GetResult();

	/// The command status enumeration received from the outstation, if applicable
	CommandStatus GetStatus();

	bool operator==(const CommandResponse& arRHS);

	std::string ToString() const;

private:
	
	CommandResult mResult;	
	CommandStatus mStatus;
};

}

/* vim: set ts=4 sw=4: */

#endif
