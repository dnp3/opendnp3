
#include "TerminalInterfaces.h"

#include <sstream>

namespace apl
{
const char ITerminal::CLEAR_SRC[2] = "\f";
const char ITerminal::EOL[3] = "\r\n";

std::string CommandNode::ToString() const
{
	std::ostringstream oss;
	oss << "Command: " << mName << "\r\n";
	oss << mDesc << "\r\nUsage: " << mUsage;
	return oss.str();
}
}

