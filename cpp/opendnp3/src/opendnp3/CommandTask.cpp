#include "CommandTask.h"

#include <openpal/Exception.h>

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

CommandTask::CommandTask(Logger aLogger) : MasterTaskBase(aLogger)
{

}

void CommandTask::Configure(const Formatter& arFormatter, const Responder& arResponder)
{
	mFormatter = arFormatter;
	mResponder = arResponder;
	mCodes.clear();
}

void CommandTask::AddCommandCode(FunctionCode aCode)
{
	this->mCodes.push_back(aCode);
}

void CommandTask::ConfigureRequest(APDU& arAPDU)
{
	if(mCodes.empty()) MACRO_THROW_EXCEPTION(InvalidStateException, "No more functions in sequence");
	mValidator = mFormatter(arAPDU, mCodes.front());
	mCodes.pop_front();
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string CommandTask::Name() const
{
	return "CommandTask";
}
#endif

void CommandTask::OnFailure()
{
	mResponder(CommandResponse(CommandResult::TIMEOUT));
}

TaskResult CommandTask::_OnPartialResponse(const APDU& arAPDU)
{
	LOG_BLOCK(LogLevel::Error, "Non fin responses not allowed for control tasks");
	return TR_CONTINUE;
}

TaskResult CommandTask::_OnFinalResponse(const APDU& arAPDU)
{
	CommandStatus cs = mValidator(arAPDU);
	if(cs == CommandStatus::SUCCESS) {
		if(mCodes.empty()) {
			mResponder(CommandResponse(CommandResult::RESPONSE_OK, cs));
			return TR_SUCCESS;
		}
		else return TR_CONTINUE;
	}
	else {
		mResponder(CommandResponse(CommandResult::RESPONSE_OK, cs));
		return TR_SUCCESS;
	}
}

} //ens ns


