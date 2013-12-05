#include "TransportScalabilityTestObject.h"
#include <sstream>

#include <boost/asio.hpp>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportScalabilityTestObject::TransportScalabilityTestObject(
        LinkConfig aClientCfg,
        LinkConfig aServerCfg,
        boost::uint16_t aPortStart,
        boost::uint16_t aNumPair,
        LogLevel aLevel,
        bool aImmediate) :

	AsyncTestObjectASIO(),
	log()
{
	const boost::uint16_t START = aPortStart;
	const boost::uint16_t STOP = START + aNumPair;

	for(boost::uint16_t port = START; port < STOP; ++port) {
		ostringstream oss;
		oss << "pair" << port;
		TransportStackPair* pPair = new TransportStackPair(aClientCfg, aServerCfg, log.mTestLogger, this->GetService(), port);
		mPairs.push_back(pPair);
	}
}

TransportScalabilityTestObject::~TransportScalabilityTestObject()
{
for(auto pPair: mPairs) {
		pPair->mClientStack.mRouter.Shutdown();
		pPair->mServerStack.mRouter.Shutdown();
	}
	this->GetService()->run();
for(auto pPair: mPairs) delete pPair;
}

bool TransportScalabilityTestObject::AllLayersUp()
{
for(TransportStackPair * pPair: mPairs) {
		if(!pPair->BothLayersUp()) return false;
	}

	return true;
}

bool TransportScalabilityTestObject::AllLayerEqual(const openpal::ReadOnlyBuffer& arBuffer)
{
for(TransportStackPair * pPair: mPairs) {
		if(! pPair->mServerStack.mUpper.BufferEquals(arBuffer)) return false;
		if(! pPair->mClientStack.mUpper.BufferEquals(arBuffer)) return false;
	}

	return true;
}

bool TransportScalabilityTestObject::AllLayerReceived(size_t aNumBytes)
{
for(TransportStackPair * pPair: mPairs) {
		if(pPair->mServerStack.mUpper.Size() != aNumBytes) return false;
		if(pPair->mClientStack.mUpper.Size() != aNumBytes) return false;
	}

	return true;
}

void TransportScalabilityTestObject::SendToAll(const openpal::ReadOnlyBuffer& arBuffer)
{
for(TransportStackPair * pPair: mPairs) {
		pPair->mClientStack.mUpper.SendDown(arBuffer);
		pPair->mServerStack.mUpper.SendDown(arBuffer);
	}
}

void TransportScalabilityTestObject::Start()
{
for(TransportStackPair * pPair: mPairs) {
		pPair->Start();
	}
}

}


