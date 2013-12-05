#include <opendnp3/ControlRelayOutputBlock.h>


namespace opendnp3
{

ControlRelayOutputBlock::ControlRelayOutputBlock(ControlCode aCode, uint8_t aCount, uint32_t aOnTime, uint32_t aOffTime) :
	mRawCode(ControlCodeToType(aCode)),
	mCount(aCount),
	mOnTimeMS(aOnTime),
	mOffTimeMS(aOffTime),
	mStatus(CommandStatus::SUCCESS)
{

}

ControlCode ControlRelayOutputBlock::GetCode() const
{
	return ControlCodeFromType(mRawCode);
}


}

