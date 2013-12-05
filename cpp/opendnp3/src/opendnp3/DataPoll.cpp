#include "DataPoll.h"


#include <openpal/Exception.h>
#include <opendnp3/PointClass.h>

#include "APDU.h"
#include "ResponseLoader.h"

using namespace openpal;

namespace opendnp3
{

/* DataPoll - base class */

DataPoll::DataPoll(Logger& arLogger, const std::function<void (MeasurementUpdate&)>& aUpdate) :
	MasterTaskBase(arLogger),
	mUpdateCallback(aUpdate)
{}

TaskResult DataPoll::_OnPartialResponse(const APDU& f)
{
	this->ReadData(f);
	return TR_CONTINUE;
}

TaskResult DataPoll::_OnFinalResponse(const APDU& f)
{
	this->ReadData(f);
	return TR_SUCCESS;
}

void DataPoll::ReadData(const APDU& f)
{
	ResponseLoader loader(mLogger, mUpdateCallback);
	HeaderReadIterator hdr = f.BeginRead();
	for ( ; !hdr.IsEnd(); ++hdr) {
		loader.Process(hdr);
	}
}

/* Class Poll */

ClassPoll::ClassPoll(Logger& arLogger, const std::function<void (MeasurementUpdate&)>& aUpdate) :
	DataPoll(arLogger, aUpdate),
	mClassMask(PC_INVALID)
{}

void ClassPoll::Set(int aClassMask)
{
	mClassMask = aClassMask;
}

void ClassPoll::ConfigureRequest(APDU& arAPDU)
{
	if (mClassMask == PC_INVALID) {
		MACRO_THROW_EXCEPTION(InvalidStateException, "Class mask has not been set");
	}

	arAPDU.Set(FunctionCode::READ);
	if (mClassMask & PC_CLASS_0) arAPDU.DoPlaceholderWrite(Group60Var1::Inst());
	if (mClassMask & PC_CLASS_1) arAPDU.DoPlaceholderWrite(Group60Var2::Inst());
	if (mClassMask & PC_CLASS_2) arAPDU.DoPlaceholderWrite(Group60Var3::Inst());
	if (mClassMask & PC_CLASS_3) arAPDU.DoPlaceholderWrite(Group60Var4::Inst());
}


} //end ns

/* vim: set ts=4 sw=4: */
