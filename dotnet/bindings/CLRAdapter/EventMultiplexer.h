#ifndef __EVENT_MULTIPLEXER_H_
#define __EVENT_MULTIPLEXER_H_

using namespace System::Collections::ObjectModel;

#include <openpal/IEventHandler.h>

#include <vector>
#include <functional>

namespace DNP3
{
namespace Adapter
{

template <class T, class U>
private class EventMultiplexer : public openpal::IEventHandler<T>
{

public:


	EventMultiplexer(const std::function<U (T)>& convert_) : convert(convert_)
	{}

	~EventMultiplexer()
	{
		for (auto cb : callbacks)
		{
			delete cb;
		}
	}

	virtual void OnEvent(T value) override final
	{
		for (auto cb : callbacks)
		{
			(*cb)->Invoke(convert(value));
		}
	}

	void AddListener(System::Action<U>^ listener)
	{
		auto pRoot = new gcroot < System::Action<U> ^ >(listener);
		callbacks.push_back(pRoot);
	}

private:

	std::function<U (T)> convert;
	std::vector<gcroot < System::Action<U> ^ >*> callbacks;
};

}
}

#endif
