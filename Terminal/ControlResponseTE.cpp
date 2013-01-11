#include "ControlResponseTE.h"

#include <APL/Logger.h>
#include <APL/LoggableMacros.h>
#include <APL/Exception.h>
#include <APL/Parsing.h>

using namespace std::placeholders;

namespace apl
{

CommandResponder :: CommandResponder(Logger* apLogger, bool aLinkStatuses, IDataObserver* apObs)
	:
	Loggable(apLogger),
	mpObs(apObs),
	mLinkStatuses(aLinkStatuses)
{

}

void CommandResponder :: AcceptCommand(const BinaryOutput& ctrl, size_t i, int aSequence, IResponseAcceptor* apRspAcceptor)
{
	std::lock_guard<std::mutex> lock(mMutex);
	CommandResponse cr;
	cr.mResult = this->HandleControl(ctrl, i);
	apRspAcceptor->AcceptResponse(cr, aSequence);
}

void CommandResponder :: AcceptCommand(const Setpoint& ctrl, size_t i, int aSequence, IResponseAcceptor* apRspAcceptor)
{
	std::lock_guard<std::mutex> lock(mMutex);
	CommandResponse cr;
	cr.mResult = this->HandleControl(ctrl, i);
	apRspAcceptor->AcceptResponse(cr, aSequence);
}

CommandStatus CommandResponder :: HandleControl(const BinaryOutput& aControl, size_t aIndex)
{
	CommandStatus cs = CS_TOO_MANY_OPS;
	if ( mLinkStatuses && (aControl.GetCode() == CC_LATCH_ON || aControl.GetCode() == CC_LATCH_OFF)) {
		try {
			Transaction t(mpObs);
			bool val = aControl.GetCode() == CC_LATCH_ON ? true : false;
			mpObs->Update(ControlStatus(val, ControlStatus::ONLINE), aIndex);
			cs = CS_SUCCESS;
			LOG_BLOCK(LEV_INFO, "Updated ControlStatus " << aIndex << " with " << val << "." );
		}
		catch (Exception& ex) {
			LOG_BLOCK(LEV_WARNING, "Failure trying to update point in response to control. " << ex.GetErrorString());
			cs = CS_FORMAT_ERROR;
		}
	}
	else {
		cs = GetResponseCode(true, aIndex);
	}
	LOG_BLOCK(LEV_INFO, "[" << aIndex << "] - " <<  aControl.ToString() << " returning " << ToString(cs));
	return cs;
}

CommandStatus CommandResponder :: HandleControl(const Setpoint& aControl, size_t aIndex)
{
	CommandStatus cs = CS_TOO_MANY_OPS;
	if ( mLinkStatuses ) {
		try {
			Transaction t(mpObs);
			mpObs->Update(SetpointStatus(aControl.GetValue(), SetpointStatus::ONLINE), aIndex);
			cs = CS_SUCCESS;
			LOG_BLOCK(LEV_INFO, "Updated SetpointStatus " << aIndex << " with " << aControl.GetValue() << "." );
		}
		catch (Exception& ex) {
			LOG_BLOCK(LEV_WARNING, "Failure trying to update point in response to control. " << ex.GetErrorString());
			cs = CS_FORMAT_ERROR;
		}
	}
	else {
		cs = GetResponseCode(false, aIndex);
	}
	LOG_BLOCK(LEV_INFO, "[" << aIndex << "] - " <<  aControl.ToString() << " returning " << ToString(cs));
	return cs;
}

void CommandResponder :: SetResponseCode(bool aType, size_t aIndex, CommandStatus aCode)
{
	std::lock_guard<std::mutex> lock(mMutex);

	CommandMap& m = (aType) ? mBinaryResponses : mSetpointResponses;

	CommandMap::iterator iter = m.find(aIndex);

	if(iter != m.end()) iter->second = aCode;
	else m[aIndex] = aCode;
}

CommandStatus CommandResponder :: GetResponseCode(bool aType, size_t aIndex)
{
	CommandMap& m = (aType) ? mBinaryResponses : mSetpointResponses;

	// check for specific return code for this index.
	CommandMap::iterator iter = m.find(aIndex);
	if(iter != m.end()) return iter->second;

	CommandMap::iterator defaultValue = m.find(-1);
	if(defaultValue != m.end()) return defaultValue->second;

	return CS_SUCCESS; //return default
}


ControlResponseTE :: ControlResponseTE(Logger* apLogger, bool aLinkStatuses, IDataObserver* apObs)
	:
	mHandler(apLogger, aLinkStatuses, apObs)
{

}

void ControlResponseTE :: _BindToTerminal(ITerminal* apTerminal)
{
	CommandNode cmd;

	cmd.mName = "response";
	cmd.mUsage = "response [bo|st|all] [all|#] [code]";
	cmd.mDesc  = "Sets the response code we will use to respond to the incoming commands.\n";
	cmd.mDesc += "You can use return code name or index: SUCCESS, TIMEOUT, NO_SELECT,\n";
	cmd.mDesc += "FORMAT_ERROR, NOT_SUPPORTED, ALREADY_ACTIVE, HARDWARE_ERROR, LOCAL,\n";
	cmd.mDesc += "TOO_MANY_OPS, NOT_AUTHORIZED.";
	cmd.mHandler = std::bind(&ControlResponseTE::HandleSetResponse, this, _1);
	apTerminal->BindCommand(cmd, cmd.mName);
}

bool LookupCommandStatus(const std::string& arArg, CommandStatus& arStatus)
{
#define MACRO_CHECK_STRING(name) if(arArg == #name) {arStatus = CS_##name; return true;}

	//this one can't be used in the macro because of a naming collision with the
	//terminal macro SUCCESS.
	if(arArg == "SUCCESS") {
		arStatus = CS_SUCCESS;
		return true;
	}
	MACRO_CHECK_STRING(TIMEOUT);
	MACRO_CHECK_STRING(NO_SELECT);
	MACRO_CHECK_STRING(FORMAT_ERROR);
	MACRO_CHECK_STRING(NOT_SUPPORTED);
	MACRO_CHECK_STRING(ALREADY_ACTIVE);
	MACRO_CHECK_STRING(HARDWARE_ERROR);
	MACRO_CHECK_STRING(LOCAL);
	MACRO_CHECK_STRING(TOO_MANY_OPS);
	MACRO_CHECK_STRING(NOT_AUTHORIZED);
	return false;
}

retcode ControlResponseTE :: HandleSetResponse(std::vector<std::string>& arArgs)
{
	if(arArgs.size() != 3) return BAD_ARGUMENTS;

	int type;
	if(arArgs[0] == "all") type = 2;
	else if(arArgs[0] == "bo") type = 0;
	else if(arArgs[0] == "st") type = 1;
	else return BAD_ARGUMENTS;

	int index;
	if(arArgs[1] == "all") index = -1;
	else if(!Parsing::Get(arArgs[1], index)) return BAD_ARGUMENTS;

	CommandStatus response;
	int temp;
	if(Parsing::Get(arArgs[2], temp)) {
		response = static_cast<CommandStatus>(temp);
	}
	else if(!LookupCommandStatus(arArgs[2], response)) return BAD_ARGUMENTS;

	if(type == 0 || type == 2) mHandler.SetResponseCode(true, index, response);
	if(type == 1 || type == 2) mHandler.SetResponseCode(false, index, response);

	return SUCCESS;
}

}
