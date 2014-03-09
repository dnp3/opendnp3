#ifndef __COMMAND_PROCESSOR_ADAPTER_H_
#define __COMMAND_PROCESSOR_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>
#include <map>

#include <opendnp3/master/ICommandProcessor.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private class ResponseRouter
{
public:
	static void Set(gcroot < Future<CommandResponse> ^ >* apFuture, opendnp3::CommandResponse cr);
};

private ref class CommandProcessorAdapter : public ICommandProcessor
{
public:

	CommandProcessorAdapter(opendnp3::ICommandProcessor* apProxy);

	virtual IFuture<CommandResponse>^ SelectAndOperate(ControlRelayOutputBlock^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputInt32^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputInt16^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputFloat32^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ SelectAndOperate(AnalogOutputDouble64^ command, System::UInt32 index);

	virtual IFuture<CommandResponse>^ DirectOperate(ControlRelayOutputBlock^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputInt32^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputInt16^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputFloat32^ command, System::UInt32 index);
	virtual IFuture<CommandResponse>^ DirectOperate(AnalogOutputDouble64^ command, System::UInt32 index);

private:

	template <class T>
	IFuture<CommandResponse>^ SelectAndOperateT(T^ command, System::UInt32 index)
	{
		auto future = gcnew Future<CommandResponse>();
		auto cmd = Conversions::convertCommand(command);
		auto pWrapper = new gcroot < Future<CommandResponse> ^ > (future);
		mpProxy->SelectAndOperate(cmd, index, std::bind(&ResponseRouter::Set, pWrapper, std::placeholders::_1));
		return future;
	}

	template <class T>
	IFuture<CommandResponse>^ DirectOperateT(T^ command, System::UInt32 index)
	{
		auto future = gcnew Future<CommandResponse>();
		auto cmd = Conversions::convertCommand(command);
		auto pWrapper = new gcroot < Future<CommandResponse> ^ > (future);
		mpProxy->DirectOperate(cmd, index, std::bind(&ResponseRouter::Set, pWrapper, std::placeholders::_1));
		return future;
	}

	opendnp3::ICommandProcessor* mpProxy;
};

}
}

#endif
