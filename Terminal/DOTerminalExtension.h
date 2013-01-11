#ifndef __DO_TERMINAL_EXTENSION_H_
#define __DO_TERMINAL_EXTENSION_H_

#include "TerminalInterfaces.h"

#include <APL/DataInterfaces.h>
#include <APL/Parsing.h>
#include <APL/ChangeBuffer.h>

#include <queue>
#include <vector>
#include <string>

namespace apl
{
/** Terminal extension that allows a user to manually update points on a data observer.
*/
class DOTerminalExtension : public ITerminalExtension
{
public:
	DOTerminalExtension(IDataObserver* apObserver) : mpObserver(apObserver) {}

	virtual ~DOTerminalExtension() {}

private:

	retcode HandleQueueBinary(std::vector<std::string>& arArgs) {
		return HandleQueue<Binary>(arArgs);
	}

	retcode HandleQueueAnalog(std::vector<std::string>& arArgs) {
		return HandleQueue<Analog>(arArgs);
	}

	retcode HandleQueueCounter(std::vector<std::string>& arArgs) {
		return HandleQueue<Counter>(arArgs);
	}

	template <class T>
	retcode HandleQueue(std::vector<std::string>& arArgs);
	retcode HandleDoTransaction(std::vector<std::string>& arArgs);
	IDataObserver* mpObserver;

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);

	ChangeBuffer mBuffer;
};

template<class T>
bool InterpretAsQualityString(const std::string& aStr, size_t& aQuality)
{
	aQuality = 0;
	for(auto c: aStr) {
		uint8_t m = T::QualConverter::GetMask(c);
		if (m == 0) {
			aQuality = T::ONLINE;
			return false;
		}
		aQuality |= m;
	}
	if (aQuality == 0)
		aQuality = T::ONLINE;
	return true;
}

template <class T>
retcode DOTerminalExtension::HandleQueue(std::vector<std::string>& arArgs)
{
	size_t index;
	typename T::ValueType value;
	size_t quality = AQ_ONLINE; //boost::uint8_t is interpreted as a '1' instead of 0x01

	switch(arArgs.size()) {
	case(3):
		if(!Parsing::Get(arArgs[2], quality)) {
			if (!InterpretAsQualityString<T>(arArgs[2], quality))
				return BAD_ARGUMENTS;
		}
	case(2):
		if(!Parsing::Get(arArgs[0], index)) return BAD_ARGUMENTS;
		if(!Parsing::Get(arArgs[1], value)) return BAD_ARGUMENTS;
		break;
	default:
		return BAD_ARGUMENTS;
	}

	T p(value, static_cast<uint8_t>(quality));
	p.SetToNow();
	Transaction tr(&mBuffer);
	mBuffer.Update(p, index);

	return SUCCESS;
}
}

#endif
