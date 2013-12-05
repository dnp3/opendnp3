#include "ConstantCommandProcessor.h"

#include <openpal/IExecutor.h>

using namespace openpal;

namespace opendnp3
{

ConstantCommandProcessor::ConstantCommandProcessor(IExecutor* apExecutor, const CommandResponse& arResponse) :
	mpExecutor(apExecutor),
	mResponse(arResponse)
{

}


void ConstantCommandProcessor::SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

}

