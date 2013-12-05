#ifndef __QUEUED_COMMAND_PROCESSOR_H_
#define __QUEUED_COMMAND_PROCESSOR_H_

#include <opendnp3/ICommandProcessor.h>
#include <opendnp3/SubjectBase.h>
#include <openpal/Visibility.h>

#include <queue>
#include <mutex>

namespace opendnp3
{

class DLL_LOCAL QueuedCommandProcessor : public ICommandProcessor, public SubjectBase
{
public:

	QueuedCommandProcessor();

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

	// Function used to marshall calls another ICommandProcessor

	bool Dispatch(ICommandProcessor* apProcessor);

private:
	std::mutex mMutex;
	std::queue<std::function<void (ICommandProcessor*)>> mRequestQueue;

	template <class T>
	void SelectAndOperateT(const T& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) {
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mRequestQueue.push([arCommand, aIndex, aCallback](ICommandProcessor * pProcessor) {
				pProcessor->SelectAndOperate(arCommand, aIndex, aCallback);
			});
		}
		this->NotifyObservers();
	}

	template <class T>
	void DirectOperateT(const T& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback) {
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mRequestQueue.push([arCommand, aIndex, aCallback](ICommandProcessor * pProcessor) {
				pProcessor->DirectOperate(arCommand, aIndex, aCallback);
			});
		}
		this->NotifyObservers();
	}
};

}

#endif

