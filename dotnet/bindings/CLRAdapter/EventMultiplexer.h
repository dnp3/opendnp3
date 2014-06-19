#ifndef __EVENT_MULTIPLEXER_H_
#define __EVENT_MULTIPLEXER_H_

using namespace System::Collections::ObjectModel;

#include <vector>
#include <functional>

namespace DNP3
{
namespace Adapter
{

template <class T, class U>
private class EventMultiplexer
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

	/// Event trigger for the native side
	std::function<void(T)> GetEventTrigger()
	{
		auto lambda = [this](T value) { this->OnEvent(value);  };
		return lambda;
	}	

	/// How to subscribe on the managed side
	void AddListener(System::Action<U>^ listener)
	{
		auto pRoot = new gcroot < System::Action<U> ^ >(listener);
		callbacks.push_back(pRoot);
	}

private:

	void OnEvent(T value)
	{
		for (auto cb : callbacks)
		{
			(*cb)->Invoke(convert(value));
		}
	}

	std::function<U (T)> convert;
	std::vector<gcroot < System::Action<U> ^ >*> callbacks;
};

}
}

#endif
