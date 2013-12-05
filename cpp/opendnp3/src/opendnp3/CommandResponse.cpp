#include <opendnp3/CommandResponse.h>

#include <sstream>

namespace opendnp3
{
	
CommandResponse::CommandResponse(CommandResult aResult, CommandStatus aStatus) : 
	mResult(CommandResult::NO_COMMS), 
	mStatus(aStatus) 
{}

CommandResponse CommandResponse::OK(CommandStatus aStatus)
{
	return CommandResponse(CommandResult::RESPONSE_OK, aStatus); 
}
	
CommandResult CommandResponse::GetResult()
{ return mResult; }

CommandStatus CommandResponse::GetStatus()
{ return mStatus; }

bool CommandResponse::operator==(const CommandResponse& arRHS)
{
	return (mResult == arRHS.mResult) && (mStatus == arRHS.mStatus);
}

std::string CommandResponse::ToString() const
{
	std::ostringstream oss;
	if(mResult == CommandResult::RESPONSE_OK) oss << "Response: " << CommandStatusToString(mStatus);
	else oss << "Failure: " << CommandResultToString(mResult);
	return oss.str();
}

}

/* vim: set ts=4 sw=4: */

