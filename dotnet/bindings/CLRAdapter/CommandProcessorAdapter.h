#ifndef __COMMAND_PROCESSOR_ADAPTER_H_
#define __COMMAND_PROCESSOR_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <openpal/util/Uncopyable.h>
#include <opendnp3/master/ICommandProcessor.h>

#include "Conversions.h"

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

class CommandCallbackAdapter : public opendnp3::ICommandCallback, openpal::Uncopyable
{
	public:
	CommandCallbackAdapter(Future<CommandResponse>^ future, bool autoDelete_) : autoDelete(autoDelete_), root(future)
	{}

	virtual void OnComplete(const opendnp3::CommandResponse& response)
	{
		auto result = Conversions::ConvertCommandResponse(response);

		root->Set(result);
		if (autoDelete)
		{
			delete this;
		}
	}

	bool autoDelete;
	gcroot < Future<CommandResponse> ^ > root;
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
		auto cmd = Conversions::ConvertCommand(command);
		auto pCallback = new CommandCallbackAdapter(future, true);		
		mpProxy->SelectAndOperate(cmd, index, *pCallback);
		return future;
	}

	template <class T>
	IFuture<CommandResponse>^ DirectOperateT(T^ command, System::UInt32 index)
	{
		auto future = gcnew Future<CommandResponse>();
		auto cmd = Conversions::ConvertCommand(command);
		auto pCallback = new CommandCallbackAdapter(future, true);
		mpProxy->DirectOperate(cmd, index, *pCallback);
		return future;
	}

	opendnp3::ICommandProcessor* mpProxy;
};

}
}

#endif
