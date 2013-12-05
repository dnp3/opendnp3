#include "AsyncPhysTestObject.h"

using namespace openpal;

namespace opendnp3
{

AsyncPhysTestObject::AsyncPhysTestObject(LogLevel aLevel, bool aImmediate, bool aAutoRead) :
	AsyncTestObjectASIO(),
	log(),
	logger(&log, aLevel, "test"),
	mTCPClient(Logger(&log, aLevel, "TCPClient"), this->GetService(), "127.0.0.1", 50000),
	mTCPServer(Logger(&log, aLevel, "TCPSever"), this->GetService(), "127.0.0.1", 50000),
	mClientAdapter(Logger(&log, aLevel, "ClientAdapter"), &mTCPClient, aAutoRead),
	mServerAdapter(Logger(&log, aLevel, "ServerAdapter"), &mTCPServer, aAutoRead),
	mClientUpper(Logger(&log, aLevel, "MockUpperClient")),
	mServerUpper(Logger(&log, aLevel, "MockUpperServer"))
{
	mClientAdapter.SetUpperLayer(&mClientUpper);
	mServerAdapter.SetUpperLayer(&mServerUpper);
}

}
