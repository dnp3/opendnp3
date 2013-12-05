#include <opendnp3/SimpleDataObserver.h>

#include <sstream>

namespace opendnp3
{

SimpleDataObserver::SimpleDataObserver(std::function<void (const std::string&)> aOutputFunc) :
	mOutputFunc(aOutputFunc)
{}

void SimpleDataObserver::_Start()
{}

void SimpleDataObserver::_End()
{}

void SimpleDataObserver::_Update(const Binary& arPoint, size_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	mOutputFunc(oss.str());
}

void SimpleDataObserver::_Update(const Analog& arPoint, size_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	mOutputFunc(oss.str());
}

void SimpleDataObserver::_Update(const Counter& arPoint, size_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	mOutputFunc(oss.str());
}


void SimpleDataObserver::_Update(const ControlStatus& arPoint, size_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	mOutputFunc(oss.str());
}

void SimpleDataObserver::_Update(const SetpointStatus& arPoint, size_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	mOutputFunc(oss.str());
}

NullDataObserver NullDataObserver::mInstance;

PrintingDataObserver PrintingDataObserver::mInstance;

}
