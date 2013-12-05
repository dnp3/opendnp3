#include "ResponseLoader.h"

#include <opendnp3/Util.h>

#include <openpal/LoggableMacros.h>

#include "HeaderReadIterator.h"
#include "Objects.h"

using namespace openpal;

namespace opendnp3
{

ResponseLoader::ResponseLoader(openpal::Logger& arLogger, const std::function<void (MeasurementUpdate&)>& aUpdate) :
	Loggable(arLogger),
	mCallback(aUpdate),
	mUpdate()
{}

ResponseLoader::~ResponseLoader()
{
	if(mUpdate.HasUpdates()) mCallback(mUpdate);
}

void ResponseLoader::Process(HeaderReadIterator& arIter)
{
	int grp = arIter->GetGroup();
	int var = arIter->GetVariation();

	this->ProcessData(arIter, grp, var);

	mCTO.NextHeader();
}

void ResponseLoader::ProcessData(HeaderReadIterator& arIter, int aGrp, int aVar)
{
	/*
	 * These objects require matching on both the aGrp and aVar fields.
	 */
	switch (MACRO_DNP_RADIX(aGrp, aVar)) {
		// Control Status
	case (MACRO_DNP_RADIX(10, 2)): this->Read(arIter, Group10Var2::Inst()); break;

		// Binary
	case (MACRO_DNP_RADIX(1, 1)): this->ReadBitfield<Group1Var1>(arIter); break;

	case (MACRO_DNP_RADIX(1, 2)): this->Read(arIter, Group1Var2::Inst()); break;
	case (MACRO_DNP_RADIX(2, 1)): this->Read(arIter, Group2Var1::Inst()); break;
	case (MACRO_DNP_RADIX(2, 2)): this->Read(arIter, Group2Var2::Inst()); break;
	case (MACRO_DNP_RADIX(2, 3)): this->Read(arIter, Group2Var3::Inst()); break;

		// Counters
	case (MACRO_DNP_RADIX(20, 1)): this->Read(arIter, Group20Var1::Inst()); break;
	case (MACRO_DNP_RADIX(20, 2)): this->Read(arIter, Group20Var2::Inst()); break;
	case (MACRO_DNP_RADIX(20, 3)): this->Read(arIter, Group20Var3::Inst()); break;
	case (MACRO_DNP_RADIX(20, 4)): this->Read(arIter, Group20Var4::Inst()); break;
	case (MACRO_DNP_RADIX(20, 5)): this->Read(arIter, Group20Var5::Inst()); break;
	case (MACRO_DNP_RADIX(20, 6)): this->Read(arIter, Group20Var6::Inst()); break;
	case (MACRO_DNP_RADIX(20, 7)): this->Read(arIter, Group20Var7::Inst()); break;
	case (MACRO_DNP_RADIX(20, 8)): this->Read(arIter, Group20Var8::Inst()); break;

	case (MACRO_DNP_RADIX(22, 1)): this->Read(arIter, Group22Var1::Inst()); break;
	case (MACRO_DNP_RADIX(22, 2)): this->Read(arIter, Group22Var2::Inst()); break;
	case (MACRO_DNP_RADIX(22, 3)): this->Read(arIter, Group22Var3::Inst()); break;
	case (MACRO_DNP_RADIX(22, 4)): this->Read(arIter, Group22Var4::Inst()); break;

		// Analogs
	case (MACRO_DNP_RADIX(30, 1)): this->Read(arIter, Group30Var1::Inst()); break;
	case (MACRO_DNP_RADIX(30, 2)): this->Read(arIter, Group30Var2::Inst()); break;
	case (MACRO_DNP_RADIX(30, 3)): this->Read(arIter, Group30Var3::Inst()); break;
	case (MACRO_DNP_RADIX(30, 4)): this->Read(arIter, Group30Var4::Inst()); break;
	case (MACRO_DNP_RADIX(30, 5)): this->Read(arIter, Group30Var5::Inst()); break;
	case (MACRO_DNP_RADIX(30, 6)): this->Read(arIter, Group30Var6::Inst()); break;

	case (MACRO_DNP_RADIX(32, 1)): this->Read(arIter, Group32Var1::Inst()); break;
	case (MACRO_DNP_RADIX(32, 2)): this->Read(arIter, Group32Var2::Inst()); break;
	case (MACRO_DNP_RADIX(32, 3)): this->Read(arIter, Group32Var3::Inst()); break;
	case (MACRO_DNP_RADIX(32, 4)): this->Read(arIter, Group32Var4::Inst()); break;
	case (MACRO_DNP_RADIX(32, 5)): this->Read(arIter, Group32Var5::Inst()); break;
	case (MACRO_DNP_RADIX(32, 6)): this->Read(arIter, Group32Var6::Inst()); break;
	case (MACRO_DNP_RADIX(32, 7)): this->Read(arIter, Group32Var7::Inst()); break;
	case (MACRO_DNP_RADIX(32, 8)): this->Read(arIter, Group32Var8::Inst()); break;

		// Setpoint Status
	case (MACRO_DNP_RADIX(40, 1)): this->Read(arIter, Group40Var1::Inst()); break;
	case (MACRO_DNP_RADIX(40, 2)): this->Read(arIter, Group40Var2::Inst()); break;
	case (MACRO_DNP_RADIX(40, 3)): this->Read(arIter, Group40Var3::Inst()); break;
	case (MACRO_DNP_RADIX(40, 4)): this->Read(arIter, Group40Var4::Inst()); break;

		// CTO
	case (MACRO_DNP_RADIX(51, 1)): this->ReadCTO<Group51Var1>(arIter); break;
	case (MACRO_DNP_RADIX(51, 2)): this->ReadCTO<Group51Var2>(arIter); break;

	default:
		this->ProcessSizeByVariation(arIter, aGrp, aVar);
		break;
	}
}

void ResponseLoader::ProcessSizeByVariation(HeaderReadIterator& arIter, int aGrp, int aVar)
{
	/*
	 * These objects only require matching on the aGrp field.
	 */
	switch (aGrp) {
		/* Virtual Terminal Objects */
		//case (112): this->ReadVto(arIter, Group112Var0::Inst()); break;
		//case (113): this->ReadVto(arIter, Group113Var0::Inst()); break;


	case (110):  // static/event octet strings get processed the same way
	case (111):
			this->ReadOctetData(arIter, [this](const uint8_t* apBuffer, size_t aSize, uint32_t aIndex) {
				this->mUpdate.Add(OctetString(apBuffer, aSize), aIndex);
			});
		break;

	default:
		/*
		* If we reach this point, then we don't yet support this object type.
		*/
		ERROR_BLOCK(LogLevel::Warning,
		            "Group: " << aGrp << " "
		            "Var: " << aVar << " "
		            "does not map to a data type", MERR_UNSUPPORTED_OBJECT_TYPE);
		break;
	}
}

void ResponseLoader::ReadOctetData(HeaderReadIterator& arIter, const std::function<void (const uint8_t*, size_t, uint32_t)>& process)
{
	// Get an iterator to the object data
	ObjectReadIterator objIter = arIter.BeginRead();

	while(!objIter.IsEnd())
	{
		uint32_t index = objIter->Index();
		const uint8_t* pData = *objIter;
		size_t size = arIter->GetVariation();
		process(pData, size, index);
		++objIter;
	}
}


}

/* vim: set ts=4 sw=4: */
