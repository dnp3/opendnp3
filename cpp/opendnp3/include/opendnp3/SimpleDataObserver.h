#ifndef __SIMPLE_DATA_OBSERVER_H_
#define	__SIMPLE_DATA_OBSERVER_H_

#include "IDataObserver.h"
#include "Singleton.h"

#include <iostream>
#include <functional>

namespace opendnp3
{

/**
* Mock data observer that sends string representation of measurements to a functor
*/
class SimpleDataObserver : public IDataObserver
{
public:
	/**
	* Primary constructor
	* @param aOutputFunc functor that will receive messages
	*/
	SimpleDataObserver(std::function<void (const std::string&)> aOutputFunc);

protected:

	//concrete class will implement these
	void _Start();
	void _Update(const Binary& arPoint, size_t);
	void _Update(const Analog& arPoint, size_t);
	void _Update(const Counter& arPoint, size_t);
	void _Update(const ControlStatus& arPoint, size_t);
	void _Update(const SetpointStatus& arPoint, size_t);
	void _End();

private:
	std::function<void (std::string)> mOutputFunc;

};

/// Singleton IDataObserver that ignores all messages
class NullDataObserver : public SimpleDataObserver
{
private:
	static NullDataObserver mInstance;
protected:
	NullDataObserver() : SimpleDataObserver([](const std::string&) {})
	{}
public:
	static NullDataObserver* Inst() {
		return &mInstance;
	}
};

/// Singleton IDataObserver that prints all measurements to the console
class PrintingDataObserver : public SimpleDataObserver
{
private:
	static PrintingDataObserver mInstance;
protected:
	PrintingDataObserver() : SimpleDataObserver([](const std::string& arStr) {
		std::cout << arStr << std::endl;
	})
	{}
public:
	static PrintingDataObserver* Inst() {
		return &mInstance;
	}
};

}

#endif
