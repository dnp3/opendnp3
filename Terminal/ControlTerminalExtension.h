#ifndef __CONTROL_TERMINAL_EXTENSION_H_
#define __CONTROL_TERMINAL_EXTENSION_H_

#include "TerminalInterfaces.h"

#include <APL/DataInterfaces.h>
#include <APL/CommandResponseQueue.h>

#include <string>

namespace apl
{
class ICommandAcceptor;


class ControlTerminalExtension : public ITerminalExtension
{
public:

	ControlTerminalExtension(ICommandAcceptor* apCmdAcceptor) :
		mSequence(0),
		mpCmdAcceptor(apCmdAcceptor),
		mRspQueue() {

	}

	std::string Name() {
		return "ControlTerminalExtension";
	}

	virtual ~ControlTerminalExtension() {}

	static ControlCode ParseControlCode( const std::string& arString );

private:

	int mSequence;

	ICommandAcceptor* mpCmdAcceptor;
	CommandResponseQueue mRspQueue;

	void WaitForResponse();
	retcode HandleIssueBO(std::vector<std::string>& arArgs);
	retcode HandleIssueST(std::vector<std::string>& arArgs);

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);


};
}

#endif
