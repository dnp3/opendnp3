#include "QueuedCommandProcessor.h"

namespace opendnp3
{

QueuedCommandProcessor::QueuedCommandProcessor() :
	SubjectBase()
{

}

void QueuedCommandProcessor::SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

bool QueuedCommandProcessor::Dispatch(ICommandProcessor* apProcessor)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mRequestQueue.empty()) return false;
	else {
		mRequestQueue.front()(apProcessor);
		mRequestQueue.pop();
		return true;
	}
}

}

