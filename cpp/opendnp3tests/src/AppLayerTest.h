#ifndef __APP_LAYER_TEST_H_
#define __APP_LAYER_TEST_H_

#include <opendnp3/AppLayer.h>

#include "LogTester.h"
#include "MockLowerLayer.h"
#include "MockExecutor.h"

#include "MockAppUser.h"


namespace opendnp3
{


class AppLayerTest
{
public:
	AppLayerTest(bool aIsMaster = false, size_t aNumRetry = 0, LogLevel aLevel = LogLevel::Warning, bool aImmediate = false);

	void SendUp(const std::string& aBytes);
	void SendUp(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ);

	void SendRequest(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);
	void SendResponse(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);
	void SendUnsolicited(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);

	bool CheckSentAPDU(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ);

	LogTester log;
	MockAppUser user;
	MockLowerLayer lower;
	MockExecutor mts;
	AppLayer app;

	MockAppUser::State state;

private:
	APDU mFragment;
};

}

#endif
