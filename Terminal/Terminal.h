#ifndef __TERMINAL_H_
#define __TERMINAL_H_

#include "TokenNode.h"
#include "LineReader.h"
#include "TerminalInterfaces.h"

#include <APL/Loggable.h>
#include <APL/Threadable.h>

#include <map>
#include <queue>

namespace apl
{

class IPhysicalLayerAsync;
class IExecutor;
class ITimer;

/** A threadable class for interacting with communication stacks.
*/
class Terminal : private LineReader, public ITerminal
{
public:

	Terminal(Logger* apLogger, IPhysicalLayerAsync* apPhysical, IExecutor* apExecutor, const std::string& arBanner = "Terminal", bool aIOMode = false);

	void BindCommand(const CommandNode& arNode, const std::string& arFullCommand);

	void AddExtension(ITerminalExtension* apExtension) {
		apExtension->BindToTerminal(this);
	}

	void QueueSend(const std::string& arData, bool aLog = true);

	void AcceptLine(const std::string& arLine);
	void _Up();
	void _Down();

	retcode ProcessLine(const std::string& arLine);

	void Post(const std::function<void ()>& arHandler);

	void SetRedirect(const RedirectFunc&);
	void ClearRedirect();

	void _OnSendSuccess();
	void _OnSendFailure();

	/*
	void Init() {
		this->Start();
	}
	*/


private:

	//overridden from IUpperLayer
	bool LogReceive() {
		return false;
	}

	void QueueSendRecursive(const std::string& arData);
	void CheckForSend();

	std::deque<std::string> mSendQueue;
	CopyableBuffer mSendBuffer;

	//The hierarchical map of commands and callbacks
	TokenNode<apl::CommandNode> mCmdRoot;
	IPhysicalLayerAsync* mpPhysical;
	IExecutor* mpExecutor;

	ITimer* mpInfiniteTimer;

	std::string mBanner;
	bool mIOMode;
	RedirectFunc mRedirectFunc;

	retcode HandleDefault(std::vector<std::string>& arTokens);
	retcode HandleHelp(std::vector<std::string>& arTokens);
	retcode HandleQuit(std::vector<std::string>& );
	retcode HandleBye(std::vector<std::string>& );
	retcode HandleEcho(std::vector<std::string>& );

	void InitCmdHandlers(); //initialize the command handlers

	void SendPrompt();

	// private helper functions
	void PrintSubCommands(const std::string& arCmdName, const std::vector<std::string>& arSubCmds);
	void PrintClearScreen();
};
}

#endif
