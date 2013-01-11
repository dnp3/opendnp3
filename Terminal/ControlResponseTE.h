#ifndef __CONTROL_RESPONSE_TERMINAL_EXTENSION_H_
#define __CONTROL_RESPONSE_TERMINAL_EXTENSION_H_

#include <APL/Loggable.h>
#include <APL/DataInterfaces.h>
#include <APL/CommandInterfaces.h>

#include "TerminalInterfaces.h"

#include <mutex>

namespace apl
{

/** Terminal extension that allows you to set the response to controlsor setpoints from a master.
*/
class CommandResponder : public ICommandAcceptor, public Loggable
{
public:

	CommandResponder(Logger* apLogger, bool aLinkStatuses = false, IDataObserver* apObs = NULL);

	void AcceptCommand(const BinaryOutput&, size_t, int aSequence, IResponseAcceptor* apRspAcceptor);
	void AcceptCommand(const Setpoint&, size_t, int aSequence, IResponseAcceptor* apRspAcceptor);

	CommandStatus HandleControl(const BinaryOutput& aControl, size_t aIndex);
	CommandStatus HandleControl(const Setpoint& aControl, size_t aIndex);

	void SetResponseCode(bool aType, size_t aIndex, CommandStatus aCode);

	CommandStatus GetResponseCode(bool aType, size_t aIndex);

private:

	std::mutex mMutex;
	typedef std::map<size_t, CommandStatus> CommandMap;
	CommandMap mBinaryResponses;
	CommandMap mSetpointResponses;

	ICommandSource* mpSource;
	IDataObserver* mpObs;
	bool mLinkStatuses;
};

class ControlResponseTE : public ITerminalExtension
{
public:

	ControlResponseTE(Logger* apLogger, bool aLinkStatuses = false, IDataObserver* apObs = NULL);

	std::string Name() {
		return "ControlResponseTE";
	}
	ICommandAcceptor* GetCmdAcceptor() {
		return &mHandler;
	}

private:

	void _BindToTerminal(ITerminal* apTerminal);

	retcode HandleSetResponse(std::vector<std::string>& arArgs);

	CommandResponder mHandler;
};
}

#endif
