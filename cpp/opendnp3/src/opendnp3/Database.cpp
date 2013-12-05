#include "Database.h"

#include <assert.h>

#include <opendnp3/DNPConstants.h>
#include <opendnp3/DeviceTemplate.h>

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

Database::Database(Logger aLogger) :
	Loggable(aLogger),
	mpEventBuffer(nullptr)
{

}

Database::~Database() {}

////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////

void Database::Configure(DataTypes aType, size_t aNumPoints, bool aStartOnline)
{
	switch(aType) {
	case(DT_BINARY):
		this->mBinaryVec.resize(aNumPoints);
		this->AssignIndices(mBinaryVec);
		if ( aStartOnline )
			this->SetAllOnline(mBinaryVec);
		break;
	case(DT_ANALOG):
		this->mAnalogVec.resize(aNumPoints);
		this->AssignIndices(mAnalogVec);
		if ( aStartOnline )
			this->SetAllOnline(mAnalogVec);
		break;
	case(DT_COUNTER):
		this->mCounterVec.resize(aNumPoints);
		this->AssignIndices(mCounterVec);
		if ( aStartOnline )
			this->SetAllOnline(mCounterVec);
		break;
	case(DT_CONTROL_STATUS):
		this->mControlStatusVec.resize(aNumPoints);
		this->AssignIndices(mControlStatusVec);
		if ( aStartOnline )
			this->SetAllOnline(mControlStatusVec);
		break;
	case(DT_SETPOINT_STATUS):
		this->mSetpointStatusVec.resize(aNumPoints);
		this->AssignIndices(mSetpointStatusVec);
		if ( aStartOnline )
			this->SetAllOnline(mSetpointStatusVec);
		break;
	}
}

void Database::Configure(const DeviceTemplate& arTmp)
{
	size_t numBinary = arTmp.mBinary.size();
	size_t numAnalog = arTmp.mAnalog.size();
	size_t numCounter = arTmp.mCounter.size();
	size_t numControlStatus = arTmp.mControlStatus.size();
	size_t numSetpointStatus = arTmp.mSetpointStatus.size();

	//configure the database for these objects
	this->Configure(DT_BINARY, numBinary, arTmp.mStartOnline);
	this->Configure(DT_ANALOG, numAnalog, arTmp.mStartOnline);
	this->Configure(DT_COUNTER, numCounter, arTmp.mStartOnline);
	this->Configure(DT_CONTROL_STATUS, numControlStatus, arTmp.mStartOnline);
	this->Configure(DT_SETPOINT_STATUS, numSetpointStatus, arTmp.mStartOnline);

	for(size_t i = 0; i < arTmp.mBinary.size(); ++i) {
		this->SetClass(DT_BINARY, i, arTmp.mBinary[i].EventClass);
	}

	for(size_t i = 0; i < arTmp.mCounter.size(); ++i) {
		this->SetClass(DT_COUNTER, i, arTmp.mCounter[i].EventClass);
	}

	for(size_t i = 0; i < arTmp.mAnalog.size(); ++i) {
		this->SetClass(DT_ANALOG, i, arTmp.mAnalog[i].EventClass);
		this->SetDeadband(DT_ANALOG, i, arTmp.mAnalog[i].Deadband);
	}

	/*for(size_t i=0; i<arTmp.mControlStatus.size(); ++i)
	{ this->SetClass(DT_CONTROL_STATUS, i, arTmp.mControlStatus[i].EventClass); }

	for(size_t i=0; i<arTmp.mSetpointStatus.size(); ++i)
	{ this->SetClass(DT_SETPOINT_STATUS, i, arTmp.mSetpointStatus[i].EventClass); }*/
}

void Database::SetClass(DataTypes aType, PointClass aClass)
{
	switch(aType) {
	case(DT_BINARY):
		for(size_t i = 0; i < mBinaryVec.size(); ++i) mBinaryVec[i].mClass = aClass;
		break;
	case(DT_ANALOG):
		for(size_t i = 0; i < mAnalogVec.size(); ++i) mAnalogVec[i].mClass = aClass;
		break;
	case(DT_COUNTER):
		for(size_t i = 0; i < mCounterVec.size(); ++i) mCounterVec[i].mClass = aClass;
		break;
	case(DT_CONTROL_STATUS):
		for(size_t i = 0; i < mControlStatusVec.size(); ++i) mControlStatusVec[i].mClass = aClass;
		break;
	case(DT_SETPOINT_STATUS):
		for(size_t i = 0; i < mSetpointStatusVec.size(); ++i) mSetpointStatusVec[i].mClass = aClass;
		break;
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Class cannot be assigned for this type");
		break;
	}
}

void Database::SetClass(DataTypes aType, size_t aIndex, PointClass aClass)
{
	switch(aType) {
	case(DT_BINARY):
		if(aIndex >= mBinaryVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mBinaryVec[aIndex].mClass = aClass;
		break;
	case(DT_ANALOG):
		if(aIndex >= mAnalogVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mAnalogVec[aIndex].mClass = aClass;
		break;
	case(DT_COUNTER):
		if(aIndex >= mCounterVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mCounterVec[aIndex].mClass = aClass;
		break;
	case(DT_CONTROL_STATUS):
		if(aIndex >= mControlStatusVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mControlStatusVec[aIndex].mClass = aClass;
		break;
	case(DT_SETPOINT_STATUS):
		if(aIndex >= mSetpointStatusVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mSetpointStatusVec[aIndex].mClass = aClass;
		break;
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Class cannot be assigned for this type");
	}
}

void Database::SetDeadband(DataTypes aType, size_t aIndex, double aDeadband)
{
	switch(aType) {
	case(DT_ANALOG):
		if(aIndex >= mAnalogVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mAnalogVec[aIndex].mDeadband = aDeadband;
		break;
	case(DT_COUNTER):
		if(aIndex >= mCounterVec.size()) MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ERR_INDEX_OUT_OF_BOUNDS);
		mCounterVec[aIndex].mDeadband = aDeadband;
		break;
	default:
		MACRO_THROW_EXCEPTION(ArgumentException, "Deadband cannot be assigned for this type");
	}
}

void Database::SetEventBuffer(IEventBuffer* apEventBuffer)
{
	assert(apEventBuffer != nullptr);
	assert(mpEventBuffer == nullptr);
	mpEventBuffer = apEventBuffer;
}

////////////////////////////////////////////////////
// IDataObserver interfae - Private NVII functions -
////////////////////////////////////////////////////

void Database::_Update(const Binary& arPoint, size_t aIndex)
{
	if(UpdateValue<Binary>(mBinaryVec, arPoint, aIndex)) {
		LOG_BLOCK(LogLevel::Debug, "Binary Change: " << arPoint.ToString() << " Index: " << aIndex);
		BinaryInfo& v = mBinaryVec[aIndex];
		if(mpEventBuffer) mpEventBuffer->Update(v.mValue, v.mClass, aIndex);
	}
}

void Database::_Update(const Analog& arPoint, size_t aIndex)
{
	if(UpdateValue<Analog>(mAnalogVec, arPoint, aIndex)) {
		LOG_BLOCK(LogLevel::Debug, "Analog Change: " << arPoint.ToString() << " Index: " << aIndex);
		mAnalogVec[aIndex].mLastEventValue = mAnalogVec[aIndex].mValue.GetValue();
		AnalogInfo& v = mAnalogVec[aIndex];
		if(mpEventBuffer) mpEventBuffer->Update(v.mValue, v.mClass, aIndex);
	}
}

void Database::_Update(const Counter& arPoint, size_t aIndex)
{
	if(UpdateValue<Counter>(mCounterVec, arPoint, aIndex)) {
		LOG_BLOCK(LogLevel::Debug, "Counter Change: " << arPoint.ToString() << " Index: " << aIndex);
		mCounterVec[aIndex].mLastEventValue = mCounterVec[aIndex].mValue.GetValue();
		CounterInfo& v = mCounterVec[aIndex];
		if(mpEventBuffer) mpEventBuffer->Update(v.mValue, v.mClass, aIndex);
	}
}

void Database::_Update(const ControlStatus& arPoint, size_t aIndex)
{
	UpdateValue<ControlStatus>(mControlStatusVec, arPoint, aIndex);
}

void Database::_Update(const SetpointStatus& arPoint, size_t aIndex)
{
	UpdateValue<SetpointStatus>(mSetpointStatusVec, arPoint, aIndex);
}

////////////////////////////////////////////////////
// misc public functions
////////////////////////////////////////////////////

size_t Database::MaxIndex(DataTypes aType)
{
	size_t num = NumType(aType);
	if(num == 0) {
		MACRO_THROW_EXCEPTION(ArgumentException, "No points for datatype");
	}
	else return (num - 1);
}

size_t Database::NumType(DataTypes aType)
{
	switch(aType) {
	case(DT_BINARY):
		return mBinaryVec.size();
	case(DT_ANALOG):
		return mAnalogVec.size();
	case(DT_COUNTER):
		return mCounterVec.size();
	case(DT_CONTROL_STATUS):
		return mControlStatusVec.size();
	case(DT_SETPOINT_STATUS):
		return mSetpointStatusVec.size();
	}

	return 0;
}

}

