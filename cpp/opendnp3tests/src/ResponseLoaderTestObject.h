#ifndef __RESPONSE_LOADER_TEST_OBJECT_H_
#define __RESPONSE_LOADER_TEST_OBJECT_H_

#include <asiopal/Log.h>

#include <openpal/Logger.h>

#include "FlexibleDataObserver.h"

namespace opendnp3
{

class ResponseLoaderTestObject
{
public:

	ResponseLoaderTestObject();

	void CheckBinaries(const std::string& arAPDU);
	void CheckCounters(const std::string& arAPDU);
	void CheckAnalogs(const std::string& arAPDU);
	void CheckSetpointStatii(const std::string& arAPDU);

	void Load(const std::string& arAPDU);

	FlexibleDataObserver fdo;

private:
	asiopal::EventLog log;
	openpal::Logger mLogger;

};

}

#endif

