#include "DOTerminalExtension.h"

#include <APL/Util.h>
#include <APL/Exception.h>
#include <APL/Parsing.h>

#include <sstream>
#include <functional>

using namespace std;
using namespace std::placeholders;

namespace apl
{
void DOTerminalExtension::_BindToTerminal(ITerminal* apTerminal)
{
	CommandNode cmd;

	ostringstream oss;


	cmd.mName = "bi";
	oss << "queue bi <index> <0|1> <quality: #|" << Binary::QualConverter::GetAllSymbols() << ">";
	cmd.mUsage = oss.str();
	cmd.mDesc = "Queues a binary input value into the transaction buffer";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleQueueBinary, this, _1);
	apTerminal->BindCommand(cmd, "queue bi");

	cmd.mName = "ai";
	oss.str("");
	oss << "queue ai <index> <#.#> <quality: #|" << Analog::QualConverter::GetAllSymbols() << ">";
	cmd.mUsage = oss.str();
	cmd.mDesc = "Queues an analog input value into the transaction buffer";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleQueueAnalog, this, _1);
	apTerminal->BindCommand(cmd, "queue ai");

	cmd.mName = "c";
	oss.str("");
	oss << "queue c <index> <0|1> <quality: #|" << Counter::QualConverter::GetAllSymbols() << ">";
	cmd.mUsage = oss.str();
	cmd.mDesc = "Queues an counter value into the transaction buffer";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleQueueCounter, this, _1);
	apTerminal->BindCommand(cmd, "queue c");

	cmd.mName = "flush";
	cmd.mUsage = "flush";
	cmd.mDesc = "Flushes the output queues to the data observer (an optional number of times).";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleDoTransaction, this, _1);
	apTerminal->BindCommand(cmd, "flush");
}

retcode DOTerminalExtension::HandleDoTransaction(std::vector<std::string>& arArgs)
{
	if(arArgs.size() > 0) return BAD_ARGUMENTS;
	mBuffer.FlushUpdates(mpObserver);
	return SUCCESS;
}

}

