#include "MasterTestObject.h"

#include <memory>
#include "BufferHelpers.h"

#include <openpal/ToHex.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{

MasterTestObject::MasterTestObject(MasterConfig cfg, openpal::LogLevel aLevel, bool aImmediate) :
	log(),
	fixedUTC(0),
	mts(),
	group(&mts),
	meas(),
	app(Logger(&log, aLevel, "MockAppLayer")),
	master(Logger(&log, aLevel, "master"), cfg, &app, &meas, &group, &mts, &fixedUTC)
{
	app.SetUser(&master);
}

void MasterTestObject::BindStateListener()
{
	master.AddStateListener([&](StackState state) {
		states.push_back(state);
	});
}

void MasterTestObject::RespondToMaster(const std::string& arData, bool aFinal)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs.ToReadOnly());
	mAPDU.Interpret();
	if(aFinal) master.OnFinalResponse(mAPDU);
	else master.OnPartialResponse(mAPDU);
}

void MasterTestObject::SendUnsolToMaster(const std::string& arData)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs.ToReadOnly());
	mAPDU.Interpret();
	master.OnUnsolResponse(mAPDU);
}

std::string MasterTestObject::Read()
{
	mAPDU = app.Read();
	std::string hex = toHex(mAPDU.ToReadOnly(), true);
	return hex;
}

} //end ns

