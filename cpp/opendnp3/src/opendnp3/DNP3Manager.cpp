#include <opendnp3/DNP3Manager.h>

#include "DNP3Channel.h"

using namespace openpal;

namespace opendnp3
{

DNP3Manager::DNP3Manager()
{

}

DNP3Manager::~DNP3Manager()
{
	this->Shutdown();
}

void DNP3Manager::Shutdown()
{
	std::set<DNP3Channel*> copy(mChannels);
	for(auto pChannel: copy) pChannel->Shutdown();
}

IChannel* DNP3Manager::CreateChannel(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, IPhysicalLayerAsync* apPhys)
{
	auto pChannel = new DNP3Channel(aLogger, aOpenRetry, apPhys, [this](DNP3Channel * apChannel) {
		mChannels.erase(apChannel);
		delete apChannel;
	});
	mChannels.insert(pChannel);
	return pChannel;
}


}


