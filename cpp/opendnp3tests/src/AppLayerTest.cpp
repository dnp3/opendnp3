#include "AppLayerTest.h"

#include "BufferHelpers.h"

namespace opendnp3
{

AppLayerTest::AppLayerTest(bool aIsMaster, size_t aNumRetry, LogLevel aLevel, bool aImmediate) :
	log(),
	user(aIsMaster),
	lower(Logger(&log, aLevel, "lower")),
	mts(),
	app(Logger(&log, aLevel, "app"), &mts, AppConfig(TimeDuration::Seconds(1), aNumRetry))
{
	lower.SetUpperLayer(&app);
	app.SetUser(&user);
}

void AppLayerTest::SendUp(const std::string& aBytes)
{
	HexSequence hs(aBytes);
	lower.SendUp(hs.ToReadOnly());
}

void AppLayerTest::SendUp(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ)
{
	APDU f;
	f.SetFunction(aCode);
	f.SetControl(aFIR, aFIN, aCON, aUNS, aSEQ);
	lower.SendUp(f.ToReadOnly());
}

void AppLayerTest::SendRequest(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);

	app.SendRequest(mFragment);
}

void AppLayerTest::SendResponse(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);
	if(aCode == FunctionCode::RESPONSE) { //write a nullptr IIN so that the buffers will match
		IINField iin;
		mFragment.SetIIN(iin);
	}
	app.SendResponse(mFragment);
}

void AppLayerTest::SendUnsolicited(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);
	if(aCode == FunctionCode::UNSOLICITED_RESPONSE) { //write a nullptr IIN so that the buffers will match
		IINField iin;
		mFragment.SetIIN(iin);
	}
	app.SendUnsolicited(mFragment);
}

bool AppLayerTest::CheckSentAPDU(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ)
{
	APDU f;
	f.SetFunction(aCode);
	f.SetControl(aFIR, aFIN, aCON, aUNS, aSEQ);
	if(aCode == FunctionCode::UNSOLICITED_RESPONSE || aCode == FunctionCode::RESPONSE) {
		IINField iin;
		f.SetIIN(iin);
	}
	bool ret = lower.BufferEquals(f.ToReadOnly());
	if(ret) lower.ClearBuffer();
	return ret;
}

}

