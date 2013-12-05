#include "OutstationSBOHandler.h"

using namespace openpal;

namespace opendnp3
{

OutstationSBOHandler::OutstationSBOHandler(TimeDuration aSelectTimeout, ICommandHandler* apCmdHandler, IExecutor* apExecutor) :
	mSelectTimeout(aSelectTimeout),
	mpCmdHandler(apCmdHandler),
	mpExecutor(apExecutor),
	mCurrentSequenceNum(0)
{

}

void OutstationSBOHandler::ClearAll()
{
	mCROBSelectMap.clear();
	mAnalog16SelectMap.clear();
	mAnalog32SelectMap.clear();
	mAnalogFloatSelectMap.clear();
	mAnalogDoubleSelectMap.clear();
}

CommandStatus OutstationSBOHandler::Select(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mCROBSelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mCROBSelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalog16SelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalog16SelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalog32SelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalog32SelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalogFloatSelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalogFloatSelectMap);
}

CommandStatus OutstationSBOHandler::Select(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Select(arCommand, aIndex, aSequence, aCode, mAnalogDoubleSelectMap);
}

CommandStatus OutstationSBOHandler::Operate(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence, QualifierCode aCode)
{
	return Operate(arCommand, aIndex, aSequence, aCode, mAnalogDoubleSelectMap);
}

}
