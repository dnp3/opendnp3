#ifndef __COMMAND_HELPERS_H_
#define __COMMAND_HELPERS_H_

#include <opendnp3/APDUConstants.h>
#include <opendnp3/ObjectInterfaces.h>
#include <openpal/Visibility.h>

#include "APDU.h"
#include "ObjectReadIterator.h"

#include <functional>

namespace opendnp3
{

// Static helper functions for formatting/validating command requests
class DLL_LOCAL CommandHelpers
{
public:
	template <class T>
	static std::function<CommandStatus (const APDU&)> ConfigureRequest(APDU& arAPDU, FunctionCode aCode, const T& arCommand, size_t aIndex, CommandObject<T>* apObj);

private:
	template <class T>
	static CommandStatus ValidateCommandResponse(const APDU& arAPDU, CommandObject<T>* apObj, const CopyableBuffer& arData, size_t aIndex);
};

template <class T>
std::function<CommandStatus (const APDU&)> CommandHelpers::ConfigureRequest(APDU& arAPDU, FunctionCode aCode, const T& arCommand, size_t aIndex, CommandObject<T>* apObj)
{
	arAPDU.Set(aCode, true, true, false, false);
	IndexedWriteIterator i = arAPDU.WriteIndexed(apObj, 1, aIndex);
	i.SetIndex(aIndex);
	apObj->Write(*i, arCommand);
	CopyableBuffer buffer = apObj->GetValueBytes(*i);
	return [ = ](const APDU & arResponse) {
		return CommandHelpers::ValidateCommandResponse(arResponse, apObj, buffer, aIndex);
	};
}

template <class T>
CommandStatus CommandHelpers::ValidateCommandResponse(const APDU& arAPDU, CommandObject<T>* apObj, const CopyableBuffer& arData, size_t aIndex)
{
	HeaderReadIterator hdr = arAPDU.BeginRead();
	if(hdr.Count() == 1) {

		int grp = hdr->GetGroup();
		int var = hdr->GetVariation();

		if(grp == apObj->GetGroup() && var == apObj->GetVariation()) {
			ObjectReadIterator obj = hdr.BeginRead();
			if(obj.Count() == 1 && obj->Index() == aIndex) { //compare what was written to what was received
				T cmd = apObj->Read(*obj);
				if(arData == apObj->GetValueBytes(*obj)) return cmd.mStatus;
				else return CommandStatus::FORMAT_ERROR;
			}
		}
	}

	return CommandStatus::UNDEFINED;
}

}

#endif
