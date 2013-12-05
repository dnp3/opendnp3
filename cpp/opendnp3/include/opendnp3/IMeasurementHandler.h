#ifndef __I_MEASUREMENT_HANDLER_H_
#define	__I_MEASUREMENT_HANDLER_H_

#include "IMeasurementUpdate.h"

#include <functional>
#include <iostream>

namespace opendnp3
{

class IMeasurementHandler
{
public:

	virtual ~IMeasurementHandler() {}	
	virtual void Load(const IMeasurementUpdate& arUpdate) = 0;
};

class ProxiedMeasurementHandler : public IMeasurementHandler
{
public:
	ProxiedMeasurementHandler(const std::function<void (const IMeasurementUpdate&)>& arProxy) : mProxy(arProxy)
	{}

	void Load(const IMeasurementUpdate& arUpdate)
	{
		mProxy(arUpdate);
	}

private:
	std::function<void (const IMeasurementUpdate&)> mProxy;
};

class NullMeasurementHandler : public ProxiedMeasurementHandler
{

public:
	static IMeasurementHandler* Inst() { return &msInstance; }

private:
	NullMeasurementHandler() : ProxiedMeasurementHandler([](const IMeasurementUpdate&){})
	{}
	
	static NullMeasurementHandler msInstance;
};

class PrintingMeasurementHandler : public ProxiedMeasurementHandler
{

public:
	static IMeasurementHandler* Inst() { return &msInstance; }

private:
	PrintingMeasurementHandler() : ProxiedMeasurementHandler(Print)
	{}

	static void Print(const IMeasurementUpdate& arUpdate);

	template<class T>
	static void PrintAny(const T&, uint32_t aIndex);
	
	static PrintingMeasurementHandler msInstance;
};

template<class T>
void PrintingMeasurementHandler::PrintAny(const T& arPoint, uint32_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	std::cout << oss.str() << std::endl;
}



}

#endif
