#ifndef __CONSTANT_COMMAND_PROCESSOR_H_
#define __CONSTANT_COMMAND_PROCESSOR_H_


#include <opendnp3/ICommandProcessor.h>
#include <openpal/Visibility.h>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

class DLL_LOCAL ConstantCommandProcessor : public ICommandProcessor
{
public:

	ConstantCommandProcessor(openpal::IExecutor* apExecutor, const CommandResponse& arResponse);

	// Implement the ICommandProcessor interface

	void SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

private:
	openpal::IExecutor* mpExecutor;
	CommandResponse mResponse;
};

}

#endif

